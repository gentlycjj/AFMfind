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
	
	writetxtflag=FALSE; //����дtxt��Ϣ�ļ��ı�ʾΪ��
	
	RectTracker.m_nStyle=CRectTracker::dottedLine; //��ʼ����Ƥ��������ʽ	
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

	writetxtflag=FALSE; //����дtxt��Ϣ�ļ��ı�ʾΪ��
	
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
	writetxtflag=FALSE; //����дtxt��Ϣ�ļ��ı�ʾΪ��
	SetPathName(lpszPathName);
	SetModifiedFlag(FALSE);     // start off with unmodified

	//*****************************************
	//�жϴ�ͼ���Ƿ�֧������껭��Ƥ������
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
		DrawRectTrackerflag=FALSE; //�������Win3.0 DIB��ʽ��ͼ�񣬱�־Ϊ��
		::GlobalUnlock((HGLOBAL) m_hDIB);
		return TRUE;  
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//    if (wNumColors!=256)
//		{
//		DrawRectTrackerflag=FALSE; //�������256ɫͼ�񣬱�־Ϊ��
//		::GlobalUnlock((HGLOBAL) m_hDIB);
//		return TRUE; 
//		}
	
	DrawRectTrackerflag=TRUE;   //�����ñ�־Ϊ�棬Ȼ�����ɫ��
	//���ͼ��ĵ�ɫ���Ƿ����Ҫ��ֻҪ��һ�㲻����Ҫ�󣬾���DrawRectTrackerflagΪ�٣��˳�ѭ��
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
		CString bmpname=lpszPathName; //bmp���ļ���
		int bmpnameindex;  //bmp�ļ�����������·�����ļ����ַ����е�λ��
		bmpnameindex=bmpname.GetLength()-bmpname.ReverseFind('\\')-1;
		bmpname=bmpname.Right(bmpnameindex); //�õ�bmp�ļ���

		//дbmp�ļ���
		sprintf(temp,"ͼ���ļ��� ");
		CString line=temp;
		filetxt.Write(line,line.GetLength());
		filetxt.Write(bmpname,bmpname.GetLength());
		sprintf(temp,"\n\n");
		line=temp;
		filetxt.Write(line,line.GetLength());
		//д����ʱ��������
		sprintf(temp,"����ʱ�䣺 %d��%d��%d��  ",year,month,day);
		line=temp;
		filetxt.Write(line,line.GetLength());
		//д����ʱ��Сʱ������
		filetxt.Write(measuretimehms,measuretimehms.GetLength());
		//д��һ�еĻس���
		sprintf(temp," \n\n");
		line=temp;
		filetxt.Write(line,line.GetLength());
		//дͼ���С
		//�ж�ͼ����100���ػ���400����
		if (bmpsize==0)
		{
		sprintf(temp,"ͼ���С�� 100��100 ����\n\n");
		}
		if (bmpsize==1)
		{
		sprintf(temp,"ͼ���С�� 400��400 ����\n\n");
		}
		line=temp;
		filetxt.Write(line,line.GetLength());
		//дɨ�跶Χ
		sprintf(temp,"ɨ�跶Χ�� X��%d nm �� Y��%d nm\n\n",scanrangex,scanrangey);
		line=temp;
		filetxt.Write(line,line.GetLength());
		//дɨ��ʱ��
		sprintf(temp,"ɨ��ʱ�䣺 %d s\n\n",scantime);
		line=temp;
		filetxt.Write(line,line.GetLength());
		//дͼ��������ֵ
//		sprintf(temp,"������ֵ�� %1.2f\n\n",(float)(lthreshold/100.0));
//		line=temp;
//		filetxt.Write(line,line.GetLength());
		//дz������ֵ��zƽ����z����z��С
		sprintf(temp,"Z���ֵ�� %1.2f V �� Z��Сֵ��%1.2f V �� Zƽ��ֵ�� %1.2f V\n\n",zmax,zmin,zaverage);
		line=temp;
		filetxt.Write(line,line.GetLength());
		//дRa,Ry��Rz
		sprintf(temp,"�ֲڶ�  Ra�� %.1f nm �� Ry��%.1f nm �� Rz�� %.1f nm\n",Ra,Ry,Rz);
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
	//syear,smonth,sday������ʱ�������գ�shms������ʱ��Сʱ�����룻ssize��ͼ���С��stime��ɨ��һ�ε�ʱ�䣻sx��ɨ��ĺ���Χ��sy��ɨ�������Χ��za��һ��ͼ��zƽ��ֵ��zma��һ��ͼ��z���ֵ��zmi��һ��ͼ��z��Сֵ; lt������ʱͼ���������ֵ��ra1��Ra�ֲڶȣ�ry1��Ry�ֲڶȣ�rz1��Rz�ֲڶ�
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
