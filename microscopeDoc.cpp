// microscopeDoc.cpp : implementation of the CMicroscopeDoc class
//

#include "stdafx.h"
#include "microscope.h"

#include "microscopeDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeDoc

IMPLEMENT_DYNCREATE(CMicroscopeDoc, CDocument)

BEGIN_MESSAGE_MAP(CMicroscopeDoc, CDocument)
	//{{AFX_MSG_MAP(CMicroscopeDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeDoc construction/destruction

CMicroscopeDoc::CMicroscopeDoc()
{
	m_hDIB = NULL;
	m_palDIB = NULL;
	m_sizeDoc = CSize(1,1);     // dummy value to make CScrollView happy
	
	writetxtflag=FALSE; //设置写txt信息文件的标示为假
	
	RectTracker.m_nStyle=CRectTracker::dottedLine; //初始化橡皮区矩形样式	
	DrawRectTrackerflag=FALSE;
}

CMicroscopeDoc::~CMicroscopeDoc()
{
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
	}
	if (m_palDIB != NULL)
	{
		delete m_palDIB;
	}

}

BOOL CMicroscopeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	writetxtflag=FALSE; //设置写txt信息文件的标示为假
	
	DrawRectTrackerflag=FALSE;
	//m_bFromClipboard=FALSE;
	m_nFromClip=0;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMicroscopeDoc serialization

void CMicroscopeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeDoc diagnostics

#ifdef _DEBUG
void CMicroscopeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMicroscopeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeDoc commands

BOOL CMicroscopeDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	DeleteContents();
	BeginWaitCursor();

	// replace calls to Serialize with ReadDIBFile function
	TRY
	{
		m_hDIB = ::ReadDIBFile(file);
	}
	CATCH (CFileException, eLoad)
	{
		file.Abort(); // will not throw an exception
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eLoad,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		m_hDIB = NULL;
		return FALSE;
	}
	END_CATCH
	//m_bFromClipboard=FALSE;
	//m_strOrgPath="";
	m_nFromClip=0;

	InitDIBData();
	EndWaitCursor();

	if (m_hDIB == NULL)
	{
		// may not be DIB format
		CString strMsg;
		strMsg.LoadString(IDS_CANNOT_LOAD_DIB);
		MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}
	writetxtflag=FALSE; //设置写txt信息文件的标示为假
	SetPathName(lpszPathName);
	SetModifiedFlag(FALSE);     // start off with unmodified

	//*****************************************
	//判断此图像是否支持用鼠标画橡皮区矩形
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPSTR lpbi;              // pointer to packed-DIB
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	WORD wNumColors;         // number of colors in color table
	int i;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);

	/* is this a Win 3.0 DIB? */
	bWinStyleDIB = IS_WIN30_DIB(lpbi);
	if (!bWinStyleDIB)
		{
		DrawRectTrackerflag=FALSE; //如果不是Win3.0 DIB格式的图像，标志为假
		::GlobalUnlock((HGLOBAL) m_hDIB);
		return TRUE;  
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//    if (wNumColors!=256)
//		{
//		DrawRectTrackerflag=FALSE; //如果不是256色图像，标志为假
//		::GlobalUnlock((HGLOBAL) m_hDIB);
//		return TRUE; 
//		}
	
	DrawRectTrackerflag=TRUE;   //先设置标志为真，然后检查调色板
	//检查图像的调色板是否符合要求，只要有一点不符合要求，就设DrawRectTrackerflag为假，退出循环
	for (i = 1; i < (int)wNumColors; i++)
		{
		if (lpbmi->bmiColors[i].rgbRed<lpbmi->bmiColors[i-1].rgbRed)
			{
			DrawRectTrackerflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbGreen<lpbmi->bmiColors[i-1].rgbGreen)
			{
			DrawRectTrackerflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbBlue<lpbmi->bmiColors[i-1].rgbBlue)
			{
			DrawRectTrackerflag=FALSE;
			break;
			}
		}

	::GlobalUnlock((HGLOBAL) m_hDIB);

	//*****************************************

	return TRUE;}

BOOL CMicroscopeDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;

	if (!file.Open(lpszPathName, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		ReportSaveLoadException(lpszPathName, &fe,
			TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}

	// replace calls to Serialize with SaveDIB function
	BOOL bSuccess = FALSE;
	TRY
	{
		BeginWaitCursor();
		bSuccess = ::SaveDIB(m_hDIB, file);
		file.Close();
	}
	CATCH (CException, eSave)
	{
		file.Abort(); // will not throw an exception
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eSave,
			TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	END_CATCH

	EndWaitCursor();
	SetModifiedFlag(FALSE);     // back to unmodified
///////////////////////////////
	if (bSuccess&&writetxtflag)
	{
		CStdioFile filetxt;
		CString txtname=lpszPathName;
		txtname=txtname+".txt";
		if (!filetxt.Open(txtname, CFile::modeCreate |
			CFile::modeReadWrite | CFile::shareExclusive | CFile::typeText, &fe))
		{
		ReportSaveLoadException(lpszPathName, &fe,
			TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
		}
		char temp[100];
		CString bmpname=lpszPathName; //bmp的文件名
		int bmpnameindex;  //bmp文件名在整个带路径的文件名字符串中的位置
		bmpnameindex=bmpname.GetLength()-bmpname.ReverseFind('\\')-1;
		bmpname=bmpname.Right(bmpnameindex); //得到bmp文件名

		//写bmp文件名
		sprintf(temp,"图像文件： ");
		CString line=temp;
		filetxt.Write(line,line.GetLength());
		filetxt.Write(bmpname,bmpname.GetLength());
		sprintf(temp,"\n\n");
		line=temp;
		filetxt.Write(line,line.GetLength());
		//写测量时间年月日
		sprintf(temp,"测量时间： %d年%d月%d日  ",year,month,day);
		line=temp;
		filetxt.Write(line,line.GetLength());
		//写测量时间小时分钟秒
		filetxt.Write(measuretimehms,measuretimehms.GetLength());
		//写第一行的回车符
		sprintf(temp," \n\n");
		line=temp;
		filetxt.Write(line,line.GetLength());
		//写图像大小
		//判断图像是100象素还是400象素
		if (bmpsize==0)
		{
		sprintf(temp,"图像大小： 100×100 像素\n\n");
		}
		if (bmpsize==1)
		{
		sprintf(temp,"图像大小： 400×400 像素\n\n");
		}
		line=temp;
		filetxt.Write(line,line.GetLength());
		//写扫描范围
		sprintf(temp,"扫描范围： X：%d nm ； Y：%d nm\n\n",scanrangex,scanrangey);
		line=temp;
		filetxt.Write(line,line.GetLength());
		//写扫描时间
		sprintf(temp,"扫描时间： %d s\n\n",scantime);
		line=temp;
		filetxt.Write(line,line.GetLength());
		//写图像亮度阈值
//		sprintf(temp,"亮度阈值： %1.2f\n\n",(float)(lthreshold/100.0));
//		line=temp;
//		filetxt.Write(line,line.GetLength());
		//写z的三个值：z平均，z最大和z最小
		sprintf(temp,"Z最大值： %1.2f V ； Z最小值：%1.2f V ； Z平均值： %1.2f V\n\n",zmax,zmin,zaverage);
		line=temp;
		filetxt.Write(line,line.GetLength());
		//写Ra,Ry和Rz
		sprintf(temp,"粗糙度  Ra： %.1f nm ； Ry：%.1f nm ； Rz： %.1f nm\n",Ra,Ry,Rz);
		line=temp;
		filetxt.Write(line,line.GetLength());
		filetxt.Close();
	}
///////////////////////////////////
	if (!bSuccess)
	{
		// may be other-style DIB (load supported but not save)
		//  or other problem in SaveDIB
		CString strMsg;
		strMsg.LoadString(IDS_CANNOT_SAVE_DIB);
		MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION | MB_OK);
	}

	return bSuccess;
}
void CMicroscopeDoc::InitDIBData()
{
	if (m_palDIB != NULL)
	{
		delete m_palDIB;
		m_palDIB = NULL;
	}
	if (m_hDIB == NULL)
	{
		return;
	}
	// Set up document size
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	if (::DIBWidth(lpDIB) > INT_MAX ||::DIBHeight(lpDIB) > INT_MAX)
	{
		::GlobalUnlock((HGLOBAL) m_hDIB);
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
		CString strMsg;
		strMsg.LoadString(IDS_BMP_TOO_BIG);
		MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION | MB_OK);
		return;
	}
	m_sizeDoc = CSize((int) ::DIBWidth(lpDIB), (int) ::DIBHeight(lpDIB));
	::GlobalUnlock((HGLOBAL) m_hDIB);
	// Create copy of palette
	m_palDIB = new CPalette;
	if (m_palDIB == NULL)
	{
		// we must be really low on memory
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
		return;
	}
	if (::CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
	{
		// DIB may not have a palette
		delete m_palDIB;
		m_palDIB = NULL;
		return;
	}
}
void CMicroscopeDoc::ReplaceHDIB(HDIB hDIB)
{
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
	}
	m_hDIB = hDIB;
}

void CMicroscopeDoc::ReplaceData(int syear,int smonth,int sday,CString shms,int ssize,int stime,int sx,int sy,float za,float zma,float zmi,int lt, float ra1, float ry1, float rz1)
{
	//syear,smonth,sday：测量时间年月日；shms：测量时间小时分钟秒；ssize：图像大小；stime：扫描一次的时间；sx：扫描的横向范围；sy：扫描的纵向范围；za：一幅图的z平均值；zma：一幅图的z最大值；zmi：一幅图的z最小值; lt：测量时图像的亮度阈值；ra1：Ra粗糙度；ry1：Ry粗糙度；rz1：Rz粗糙度
	year=syear;
	month=smonth;
	day=sday;
	measuretimehms=shms;
	bmpsize=ssize;
	scantime=stime;
	scanrangex=sx;
	scanrangey=sy;
	zaverage=za;
	zmax=zma;
	zmin=zmi;
	Ra=ra1;
	Ry=ry1;
	Rz=rz1;
	lthreshold=lt;

}

void CMicroscopeDoc::ReplaceFlag(BOOL flag)
{
    writetxtflag=flag;
}
