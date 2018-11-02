// microscope.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "microscope.h"
#include "measure.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "microscopeDoc.h"
#include "microscopeView.h"
#include "BCAdjust.h"
#include "Show3d.h"
#include "math.h"
#include <afxadv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeApp

BEGIN_MESSAGE_MAP(CMicroscopeApp, CWinApp)
	//{{AFX_MSG_MAP(CMicroscopeApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_MEASURE, OnMeasure)
	ON_COMMAND(IDC_SETCOLOR, OnSetcolor)
	ON_COMMAND(ID_3D, On3d)
	ON_COMMAND(ID_BLUR, OnBlur)
	ON_COMMAND(IDC_PSEUDOCOLOR, OnPseudocolor)
	ON_COMMAND(IDC_BCADJUST, OnBCAdjust)
	ON_COMMAND(ID_ATOMFORCE, OnAtomforce)
	ON_COMMAND(ID_ROUGHNESS, OnRoughness)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeApp construction

CMicroscopeApp::CMicroscopeApp()
{
	// TODO: add construction code here,
	m_OptionColor=RGB(0,0,64);
	voltagetonanometer=300.0; // 高度/电压系数，此值改动时，三维显示的系数也需随之改动, Show3d.cpp文件中sprintf(timechar,"%f",(float)((zmax2-zmin2)*500.0)); //
    // AD值1V等于高压77V，恒力模式时，等于压电陶瓷伸缩77×6nm＝450nm
	
	ceramics=(float)0.07;  // 校正系数，此数值越大，校正量越大，即右下角颗粒越大 // 20140506
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMicroscopeApp object

CMicroscopeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeApp initialization

BOOL CMicroscopeApp::InitInstance()
{
    //下面代码是让本程序只能执行一次，即：如果本程序已经在运行，就再不能启动其副本了
    //*********************
	//调用CreateMutex()函数
    CreateMutex(NULL,TRUE,"AFM2008");
    //调用GetLastError()函数并判断返回值,若返回值为ERROR_ALREADY_EXISTS,则表示
    //应用程序的实例已经调用
    if (GetLastError()==ERROR_ALREADY_EXISTS)
	{
	  return FALSE;
	}                           
	//代码结束
	//**********************

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_MICROSTYPE,
		RUNTIME_CLASS(CMicroscopeDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMicroscopeView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	
	//Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//no empty document window on startup
	if (cmdInfo.m_nShellCommand==CCommandLineInfo::FileNew)
	{
		cmdInfo.m_nShellCommand=CCommandLineInfo::FileNothing;
	}

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	//一启动程序就最大化
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);//m_nCmdShow);
	pMainFrame->UpdateWindow();
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMicroscopeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeApp message handlers


void CMicroscopeApp::OnMeasure() 
{
	measuredlg.DoModal();
}



void CMicroscopeApp::Bmpcapture()
{
	POSITION posTemplate = GetFirstDocTemplatePosition();
    CDocTemplate* pDocTemplate = GetNextDocTemplate(posTemplate);
    CMicroscopeDoc* pDocument = (CMicroscopeDoc*) pDocTemplate->OpenDocumentFile(NULL);
	HDIB hNewDIB;
	LPSTR pNewDIB;   

	CTime now=CTime::GetCurrentTime();
	int year=now.GetYear();
	int month=now.GetMonth();
	int day=now.GetDay();
	CString measuretimehms;
	measuretimehms=now.Format("%H:%M:%S");
	pDocument->ReplaceFlag(TRUE);
	//判断是大范围扫描，还是小范围扫描 并把扫描图像信息传过去，而且写入实际代表的扫描范围和电压值到BMP信息头中去
	if (measuredlg.m_radiorange==0)
	{
	
	pDocument->ReplaceData(year,month,day,measuretimehms,measuredlg.m_radiobmpsize,measuredlg.timelast,measuredlg.m_smallrangex,measuredlg.m_smallrangey,measuredlg.zave,measuredlg.zmax,measuredlg.zmin,measuredlg.lightnessthresholdlast,measuredlg.Ra,measuredlg.Ry,measuredlg.Rz);

	//判断是大图还是小图
	if (measuredlg.m_radiobmpsize==0)
		{
		measuredlg.smallbmpinfoheader.biSizeImage=(DWORD)(measuredlg.m_smallrangex);
		measuredlg.smallbmpinfoheader.biXPelsPerMeter=(LONG)(measuredlg.m_smallrangey);
		measuredlg.smallbmpinfoheader.biClrImportant=(DWORD)((measuredlg.zmax)*1000);
		measuredlg.smallbmpinfoheader.biYPelsPerMeter=(LONG)((measuredlg.zmin)*1000);
		}
	if (measuredlg.m_radiobmpsize==1)
		{
		measuredlg.largebmpinfoheader.biSizeImage=(DWORD)(measuredlg.m_smallrangex);
		measuredlg.largebmpinfoheader.biXPelsPerMeter=(LONG)(measuredlg.m_smallrangey);
		measuredlg.largebmpinfoheader.biClrImportant=(DWORD)((measuredlg.zmax)*1000);
		measuredlg.largebmpinfoheader.biYPelsPerMeter=(LONG)((measuredlg.zmin)*1000);
		}
	}

	if (measuredlg.m_radiorange==1)
	{
	
	pDocument->ReplaceData(year,month,day,measuretimehms,measuredlg.m_radiobmpsize,measuredlg.timelast,measuredlg.m_largerangex,measuredlg.m_largerangey,measuredlg.zave,measuredlg.zmax,measuredlg.zmin,measuredlg.lightnessthresholdlast,measuredlg.Ra,measuredlg.Ry,measuredlg.Rz);

	//判断是大图还是小图
	if (measuredlg.m_radiobmpsize==0)
		{
		measuredlg.smallbmpinfoheader.biSizeImage=(DWORD)(measuredlg.m_largerangex);
		measuredlg.smallbmpinfoheader.biXPelsPerMeter=(LONG)(measuredlg.m_largerangey);
		measuredlg.smallbmpinfoheader.biClrImportant=(DWORD)((measuredlg.zmax)*1000);
		measuredlg.smallbmpinfoheader.biYPelsPerMeter=(LONG)((measuredlg.zmin)*1000);
		}
	if (measuredlg.m_radiobmpsize==1)
		{
		measuredlg.largebmpinfoheader.biSizeImage=(DWORD)(measuredlg.m_largerangex);
		measuredlg.largebmpinfoheader.biXPelsPerMeter=(LONG)(measuredlg.m_largerangey);
		measuredlg.largebmpinfoheader.biClrImportant=(DWORD)((measuredlg.zmax)*1000);
		measuredlg.largebmpinfoheader.biYPelsPerMeter=(LONG)((measuredlg.zmin)*1000);
		}
	}


//写小图的dib句柄
	if (measuredlg.m_radiobmpsize==0)
	{
	
	hNewDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, 12500);

	if (hNewDIB == 0)
		{
		return ;
		}
	pNewDIB = (LPSTR) ::GlobalLock((HGLOBAL) hNewDIB);

	::memcpy(pNewDIB,&(measuredlg.smallbmpinfoheader),40);
	::memcpy(pNewDIB+40,measuredlg.bmpcolorpalette,1024);
	::memcpy(pNewDIB+1064,measuredlg.smallbmpbits,10000);
	}
//写大图的dib句柄
	if (measuredlg.m_radiobmpsize==1)
	{

	hNewDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, 200000);
	if (hNewDIB == 0)
		{
		return ;
		}
	pNewDIB = (LPSTR) ::GlobalLock((HGLOBAL) hNewDIB);

	::memcpy(pNewDIB,&(measuredlg.largebmpinfoheader),40);
	::memcpy(pNewDIB+40,measuredlg.bmpcolorpalette,1024);
	::memcpy(pNewDIB+1064,measuredlg.largebmpbits,160000);
	}

	pDocument->ReplaceHDIB(hNewDIB); 
	pDocument->InitDIBData();    // set up new size & palette
	pDocument->SetModifiedFlag(TRUE);
	pDocument->UpdateAllViews(NULL);
	pDocument->DrawRectTrackerflag=TRUE;  //画橡皮区矩形标志为真

	::GlobalUnlock((HGLOBAL)hNewDIB);
}

void CMicroscopeApp::OnSetcolor() 
{
	//定义一个颜色选择对话框
	CColorDialog dlg(m_OptionColor,CC_FULLOPEN);  
 	
	if (dlg.DoModal() != IDOK)
		return;

	m_OptionColor = dlg.GetColor();
	
	return;
}



void CMicroscopeApp::On3d() 
{
	//得到MDI程序主窗口指针
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //得到MDI程序活动子窗口指针
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
	{
		return; // no active MDI child frame 
	}

	//得到MDI程序活动文档指针
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //如果没有图像，就使返回
		{
		return; 
		}

	//显示三维立体对话框
	CShow3d* dlg=new CShow3d;

	dlg->holdDIB=hDIB;  //传递图像句柄

	dlg->DoModal();

	delete dlg;
}



void CMicroscopeApp::OnBlur() 
{

	BeginWaitCursor();

	//得到MDI程序主窗口指针
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //得到MDI程序活动子窗口指针
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
		{
		return; // no active MDI child frame 
		}

	//得到MDI程序活动文档指针
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //如果没有图像，就使菜单变灰
		{
		return; 
		}

	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	HDIB  hnewDIB;           //blur后的DIB图像
	LPSTR pnewDIB;           //blur后DIB图像的指针
	LONG width;               //图像宽度
	LONG height;              //图像高度
	LONG factwidth;           //图像DIB数据宽度实际使用的字节数 DIB图像的宽度要是4byte的倍数
	int i, j;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
 
	/* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;


	//得到图像的高度和宽度
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;

	//计算DIB图像数据宽度实际使用的字节数
	factwidth=WIDTHBYTES(width*8);

	//分配DIB图像的内存空间
	hnewDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, factwidth*height+1064);
	//得到其指针
	pnewDIB = (LPSTR) ::GlobalLock((HGLOBAL) hnewDIB);
	
	//复制DIB图像的信息头及调色板
	::memcpy(pnewDIB,lpbi,1064);

	//动态申请用来临时存储DIB图像数据的数组
	BYTE **bmpoldbits;      //原图像数组
	bmpoldbits=new BYTE*[factwidth];
	for (i=0;i<factwidth;i++)
		bmpoldbits[i]=new BYTE[height];

	BYTE **bmpnewbits;     //处理后新图像数组
	bmpnewbits=new BYTE*[factwidth];
	for (i=0;i<factwidth;i++)
		bmpnewbits[i]=new BYTE[height];

   	//读取原DIB图像的dib数据
	//拷贝原图像的dib数据到原图像数组
	for (j=0;j<height;j++)
		for (i=0;i<factwidth;i++)
			::memcpy(&bmpoldbits[i][j],lpbi+1064+i+j*factwidth,1); 
	
	//进行blur处理，用的矩阵为：
	// 1 1 1
	// 1 5 1
	// 1 1 1
	//对于边缘点用的矩阵为
	// 1 5 1
	// 1 1 1
	//顶点矩阵为
	// 7 1
	// 1 1
	for (i=0;i<width;i++)
			{
		for (j=0;j<height;j++)
				{
			//四个顶点
			if (i==0&&j==0)		{
				bmpnewbits[i][j]=(BYTE)(0.1*(bmpoldbits[i+1][j]+
	                                           bmpoldbits[i+1][j+1]+
											   bmpoldbits[i][j+1])+
										  0.7*bmpoldbits[i][j]);
								}
			if (i==(width-1)&&j==0)	{
				bmpnewbits[i][j]=(BYTE)(0.1*(bmpoldbits[i-1][j]+
	                                           bmpoldbits[i-1][j+1]+
											   bmpoldbits[i][j+1])+
										  0.7*bmpoldbits[i][j]);
								}
			if (i==0&&j==(height-1))   {
				bmpnewbits[i][j]=(BYTE)(0.1*(bmpoldbits[i+1][j]+
	                                           bmpoldbits[i+1][j-1]+
											   bmpoldbits[i][j-1])+
										  0.7*bmpoldbits[i][j]);
								}
			if (i==(width-1)&&j==(height-1)) {
				bmpnewbits[i][j]=(BYTE)(0.1*(bmpoldbits[i-1][j-1]+
	                                           bmpoldbits[i-1][j]+
											   bmpoldbits[i][j-11])+
										  0.7*bmpoldbits[i][j]);
								}
            //四个边缘线
			if (i==0&&j!=0&&j!=(height-1)) {
				bmpnewbits[i][j]=(BYTE)(0.1*(bmpoldbits[i][j-1]+
											   bmpoldbits[i+1][j-1]+
											   bmpoldbits[i+1][j]+
											   bmpoldbits[i+1][j+1]+
											   bmpoldbits[i][j+1])+
										   0.5*bmpoldbits[i][j]);
									}
			if (i==(width-1)&&j!=0&&j!=(height-1)){
				bmpnewbits[i][j]=(BYTE)(0.1*(bmpoldbits[i][j-1]+
											   bmpoldbits[i-1][j-1]+
											   bmpoldbits[i-1][j]+
											   bmpoldbits[i-1][j+1]+
											   bmpoldbits[i][j+1])+
										   0.5*bmpoldbits[i][j]);
									}
			if (j==0&&i!=0&&i!=(width-1)) {
				bmpnewbits[i][j]=(BYTE)(0.1*(bmpoldbits[i][j+1]+
											   bmpoldbits[i-1][j+1]+
											   bmpoldbits[i+1][j+1]+
											   bmpoldbits[i-1][j]+
											   bmpoldbits[i+1][j])+
										   0.5*bmpoldbits[i][j]);
									}
			if (j==(height-1)&&i!=0&&i!=(width-1)) {
				bmpnewbits[i][j]=(BYTE)(0.1*(bmpoldbits[i][j-1]+
											   bmpoldbits[i-1][j-1]+
											   bmpoldbits[i+1][j-1]+
											   bmpoldbits[i-1][j]+
											   bmpoldbits[i+1][j])+
										   0.5*bmpoldbits[i][j]);
									}
			if (i!=0&&i!=(width-1)&&j!=0&&j!=(height-1)) {
			bmpnewbits[i][j]=(BYTE)(0.0625*(bmpoldbits[i+1][j-1]+
				                              bmpoldbits[i+1][j]+
									          bmpoldbits[i+1][j+1]+
									          bmpoldbits[i][j-1]+
											  bmpoldbits[i][j+1]+
											  bmpoldbits[i-1][j-1]+
											  bmpoldbits[i-1][j]+
											  bmpoldbits[i-1][j+1])+
										  0.5*bmpoldbits[i][j]);
											}
					}
			}
	//把处理后的新图像数组写入新图像的DIB里
	for (j=0;j<height;j++)
		for (i=0;i<factwidth;i++)
			::memcpy(pnewDIB+1064+i+j*factwidth,&bmpnewbits[i][j],1);

	//开启新的文档
	POSITION posTemplate = GetFirstDocTemplatePosition();
    CDocTemplate* pDocTemplate = GetNextDocTemplate(posTemplate);
    CMicroscopeDoc* pDocument = (CMicroscopeDoc*) pDocTemplate->OpenDocumentFile(NULL);
	
	//传递HDIB，并且更新视图
	pDocument->ReplaceFlag(FALSE); //不写txt信息文件
	pDocument->ReplaceHDIB(hnewDIB); 
	pDocument->InitDIBData();    // set up new size & palette
	pDocument->SetModifiedFlag(TRUE);
	pDocument->UpdateAllViews(NULL);
	pDocument->DrawRectTrackerflag=TRUE;  //画橡皮区矩形标志为真

	::GlobalUnlock((HGLOBAL)hnewDIB);
	::GlobalUnlock((HGLOBAL)hDIB);
	//释放动态申请的内存
	for (i=0;i<factwidth;i++)
	{
		delete bmpoldbits[i];
		delete bmpnewbits[i];
	}
	
	delete bmpoldbits;
	delete bmpnewbits;

	EndWaitCursor();

	return;
}



void CMicroscopeApp::OnPseudocolor() 
{
	BeginWaitCursor();

	//得到MDI程序主窗口指针
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //得到MDI程序活动子窗口指针
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
		{
		return; // no active MDI child frame 
		}

	//得到MDI程序活动文档指针
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //如果没有图像，就使菜单变灰
		{
		return; 
		}

	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	HDIB  hnewDIB;           //pseudocolor后的DIB图像
	LPSTR pnewDIB;           //pseudocolor后DIB图像的指针
	LONG width;               //图像宽度
	LONG height;              //图像高度
	LONG factwidth;           //图像DIB数据宽度实际使用的字节数 DIB图像的宽度要是4byte的倍数
	RGBQUAD bmpcolorpalette[256]; //要写入的新调色板数据
	BYTE  red,green,blue;    //用户在颜色对话框中选择的RGB颜色值
    int i;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
 
	/* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;


	//得到图像的高度和宽度
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;

	//计算DIB图像数据宽度实际使用的字节数
	factwidth=WIDTHBYTES(width*8);

	//伪彩色处理既是把原图像的调色板换成自己定义的调色板
	
	//分配处理后图像的内存空间
	hnewDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, factwidth*height+1064);

	//得到其指针
	pnewDIB = (LPSTR) ::GlobalLock((HGLOBAL) hnewDIB);

	//复制图像文件的信息头
	::memcpy(pnewDIB,lpbi,40);

	//改变调色板
	//得到用户选择的RGB值
	red=GetRValue(m_OptionColor);
	green=GetGValue(m_OptionColor);
	blue=GetBValue(m_OptionColor);

	//产生新调色板
	for (i=0;i<=255;i++)
		{
		bmpcolorpalette[i].rgbRed=(BYTE)(red+i*(255.0-red)/255.0);
		bmpcolorpalette[i].rgbGreen=(BYTE)(green+i*(255.0-green)/255.0);
		bmpcolorpalette[i].rgbBlue=(BYTE)(blue+i*(255.0-blue)/255.0);
		bmpcolorpalette[i].rgbReserved=0;
		}

	//写入新调色板
	::memcpy(pnewDIB+40,bmpcolorpalette,1024);
    //复制原图像的数据
	::memcpy(pnewDIB+1064,lpbi+1064,factwidth*height);

	//开启新的文档
	POSITION posTemplate = GetFirstDocTemplatePosition();
    CDocTemplate* pDocTemplate = GetNextDocTemplate(posTemplate);
    CMicroscopeDoc* pDocument = (CMicroscopeDoc*) pDocTemplate->OpenDocumentFile(NULL);
	
	//传递HDIB，并且更新视图
	pDocument->ReplaceFlag(FALSE); //不写txt信息文件
	pDocument->ReplaceHDIB(hnewDIB); 
	pDocument->InitDIBData();    // set up new size & palette
	pDocument->SetModifiedFlag(TRUE);
	pDocument->UpdateAllViews(NULL);
	pDocument->DrawRectTrackerflag=TRUE;  //画橡皮区矩形标志为真

	::GlobalUnlock((HGLOBAL)hnewDIB);
	::GlobalUnlock((HGLOBAL)hDIB);

	EndWaitCursor();

	return;
}





void CMicroscopeApp::OnBCAdjust() 
{
	BeginWaitCursor();

/***********************************************
    首先复制原图像到一新的文档中
************************************************/
	
	//得到MDI程序主窗口指针
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //得到MDI程序活动子窗口指针
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
		{
		return; // no active MDI child frame 
		}

	//得到MDI程序活动文档指针
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //如果没有图像，就返回
		{
		return; 
		}

	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	HDIB  hnewDIB;           //复制后的DIB图像
	LPSTR pnewDIB;           //复制后DIB图像的指针
	LONG width;               //图像宽度
	LONG height;              //图像高度
	LONG factwidth;           //图像DIB数据宽度实际使用的字节数 DIB图像的宽度要是4byte的倍数

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
 
	/* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;


	//得到图像的高度和宽度
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;

	//计算DIB图像数据宽度实际使用的字节数
	factwidth=WIDTHBYTES(width*8);

	//分配处理后图像的内存空间
	hnewDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, factwidth*height+1064);

	//得到其指针
	pnewDIB = (LPSTR) ::GlobalLock((HGLOBAL) hnewDIB);

	//复制整个图像数据，包括信息头，调色板和索引值
	::memcpy(pnewDIB,lpbi,factwidth*height+1064);

	//开启新的文档
	POSITION posTemplate = GetFirstDocTemplatePosition();
    CDocTemplate* pDocTemplate = GetNextDocTemplate(posTemplate);
    CMicroscopeDoc* pDocument = (CMicroscopeDoc*) pDocTemplate->OpenDocumentFile(NULL);
	
	//传递HDIB，并且更新视图
	pDocument->ReplaceFlag(FALSE); //不写txt信息文件
	pDocument->ReplaceHDIB(hnewDIB); 
	pDocument->InitDIBData();    // set up new size & palette
	pDocument->SetModifiedFlag(TRUE);
	pDocument->UpdateAllViews(NULL);
	pDocument->DrawRectTrackerflag=TRUE;  //画橡皮区矩形标志为真

	::GlobalUnlock((HGLOBAL)hnewDIB);
	::GlobalUnlock((HGLOBAL)hDIB);
	
	EndWaitCursor();

/****************************************
    原图像复制完成
*****************************************/

/*****************************************
	显示亮度/对比度调整对话框，
	并对复制后的图像进行调节
******************************************/

	CBCAdjust* dlg=new CBCAdjust;
	
	//传递复制后图像句柄
	dlg->hDIB=hnewDIB;

	dlg->DoModal();
    
	delete dlg;

	return;
}

void CMicroscopeApp::OnAtomforce() 
{
	atomforcedlg.DoModal();
	
}




///////////////////////////////////////////////////////////////////////////////////////

void CMicroscopeApp::OnRoughness() 
{
	// TODO: Add your command handler code here

	//得到MDI程序主窗口指针
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //得到MDI程序活动子窗口指针
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
		{
		return; // no active MDI child frame 
		}

	//得到MDI程序活动文档指针
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //如果没有图像，就返回
		{
		return; 
		}

	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LONG width;               //图像宽度
	LONG height;              //图像高度
	LONG factwidth;           //图像DIB数据宽度实际使用的字节数 DIB图像的宽度要是4byte的倍数

	float zmax;        //图像Z向最大电压值
	float zmin;        //图像Z向最小电压值
	int i;
	int j;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
 
	/* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;
	
	CRoughness* dlg=new CRoughness;

	//得到图像的高度和宽度
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;
	
	//计算DIB图像数据宽度实际使用的字节数
	factwidth=WIDTHBYTES(width*8);
	
	//传递图像的高度和宽度
    dlg->imagesizex=lpbmi->bmiHeader.biWidth;
	dlg->imagesizey=lpbmi->bmiHeader.biHeight;

	//传递图像的扫描范围
	dlg->scanrangex=lpbmi->bmiHeader.biSizeImage;
	dlg->scanrangey=lpbmi->bmiHeader.biXPelsPerMeter;

	//得到图像Z向最大和最小电压值
	zmin=(float)((lpbmi->bmiHeader.biYPelsPerMeter)/1000.0);
	zmax=(float)((lpbmi->bmiHeader.biClrImportant)/1000.0);

	//动态申请用来临时存储原DIB图像数据的数组
	BYTE **bmpbits;      //图像数组
	bmpbits=new BYTE*[factwidth];
	for (i=0;i<factwidth;i++)
		bmpbits[i]=new BYTE[height];

	//读取原DIB图像的dib数据
	//拷贝原图像的dib数据到图像数组
	for (j=0;j<height;j++)
		for (i=0;i<factwidth;i++)
			::memcpy(&bmpbits[i][j],lpbi+1064+i+j*factwidth,1); 

	/////////////////////////////////////////////
	//计算粗糙度Ra，Ry和Rz
	//Ra计算
	//由bmpbits数组计算得到其代表的电压值，并计算其中的平均值和最大值，最小值
    float Raavetemp; //计算Ra时储存Z方向电压平均值的临时变量
	float Rmaxtemp;  //计算Ry和Rz时储存Z方向电压最大值的临时变量
	float Rmintemp;  //计算Ry和Rz时储存Z方向电压最小值的临时变量
	float Ratemp;     //Ra的临时变量
	float Rytemp;     //Ry的临时变量
	float Rztemp;     //Rz的临时变量

	Raavetemp=0;
	Rmaxtemp=Rmintemp=bmpbits[0][0];
	for (j=0;j<height;j++)
	{
		for (i=0;i<width;i++)
		{
			Raavetemp=Raavetemp+(zmax-zmin)*(float)(bmpbits[i][j]/255.0)+zmin;
		if (Rmaxtemp<=bmpbits[i][j])
			{
			Rmaxtemp=bmpbits[i][j];
			}
		if (Rmintemp>=bmpbits[i][j])
			{
			Rmintemp=bmpbits[i][j];
			}
		}
	}
	Raavetemp=(float)(Raavetemp/(height*width));

	Rytemp=(float)((zmax-zmin)*(Rmaxtemp-Rmintemp)/255.0)*voltagetonanometer;
	Rztemp=(float)(fabs((zmax-zmin)*Rmaxtemp/255.0+zmin-Raavetemp)+fabs((zmax-zmin)*Rmintemp/255.0+zmin-Raavetemp))*voltagetonanometer;

	Ratemp=0;
	for (j=0;j<height;j++)
	{
		for (i=0;i<width;i++)
		{
			Ratemp=Ratemp+(float)fabs((zmax-zmin)*(float)(bmpbits[i][j]/255.0)+zmin-Raavetemp);
		}
	}
	Ratemp=(float)(Ratemp/(height*width))*voltagetonanometer;

    //传递Ra,Ry和Rz
	dlg->Ra=Ratemp;
	dlg->Ry=Rytemp;
	dlg->Rz=Rztemp;

	//释放动态申请的内存
	for (i=0;i<factwidth;i++)
	{
		delete bmpbits[i];
	}

	dlg->DoModal();

 	delete dlg;

	return;

}
