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
	voltagetonanometer=300.0; // �߶�/��ѹϵ������ֵ�Ķ�ʱ����ά��ʾ��ϵ��Ҳ����֮�Ķ�, Show3d.cpp�ļ���sprintf(timechar,"%f",(float)((zmax2-zmin2)*500.0)); //
    // ADֵ1V���ڸ�ѹ77V������ģʽʱ������ѹ���մ�����77��6nm��450nm
	
	ceramics=(float)0.07;  // У��ϵ��������ֵԽ��У����Խ�󣬼����½ǿ���Խ�� // 20140506
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMicroscopeApp object

CMicroscopeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeApp initialization

BOOL CMicroscopeApp::InitInstance()
{
    //����������ñ�����ֻ��ִ��һ�Σ���������������Ѿ������У����ٲ��������丱����
    //*********************
	//����CreateMutex()����
    CreateMutex(NULL,TRUE,"AFM2008");
    //����GetLastError()�������жϷ���ֵ,������ֵΪERROR_ALREADY_EXISTS,���ʾ
    //Ӧ�ó����ʵ���Ѿ�����
    if (GetLastError()==ERROR_ALREADY_EXISTS)
	{
	  return FALSE;
	}                           
	//�������
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
	//һ������������
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
	//�ж��Ǵ�Χɨ�裬����С��Χɨ�� ����ɨ��ͼ����Ϣ����ȥ������д��ʵ�ʴ����ɨ�跶Χ�͵�ѹֵ��BMP��Ϣͷ��ȥ
	if (measuredlg.m_radiorange==0)
	{
	
	pDocument->ReplaceData(year,month,day,measuretimehms,measuredlg.m_radiobmpsize,measuredlg.timelast,measuredlg.m_smallrangex,measuredlg.m_smallrangey,measuredlg.zave,measuredlg.zmax,measuredlg.zmin,measuredlg.lightnessthresholdlast,measuredlg.Ra,measuredlg.Ry,measuredlg.Rz);

	//�ж��Ǵ�ͼ����Сͼ
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

	//�ж��Ǵ�ͼ����Сͼ
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


//дСͼ��dib���
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
//д��ͼ��dib���
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
	pDocument->DrawRectTrackerflag=TRUE;  //����Ƥ�����α�־Ϊ��

	::GlobalUnlock((HGLOBAL)hNewDIB);
}

void CMicroscopeApp::OnSetcolor() 
{
	//����һ����ɫѡ��Ի���
	CColorDialog dlg(m_OptionColor,CC_FULLOPEN);  
 	
	if (dlg.DoModal() != IDOK)
		return;

	m_OptionColor = dlg.GetColor();
	
	return;
}



void CMicroscopeApp::On3d() 
{
	//�õ�MDI����������ָ��
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
	{
		return; // no active MDI child frame 
	}

	//�õ�MDI�����ĵ�ָ��
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //���û��ͼ�񣬾�ʹ����
		{
		return; 
		}

	//��ʾ��ά����Ի���
	CShow3d* dlg=new CShow3d;

	dlg->holdDIB=hDIB;  //����ͼ����

	dlg->DoModal();

	delete dlg;
}



void CMicroscopeApp::OnBlur() 
{

	BeginWaitCursor();

	//�õ�MDI����������ָ��
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
		{
		return; // no active MDI child frame 
		}

	//�õ�MDI�����ĵ�ָ��
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //���û��ͼ�񣬾�ʹ�˵����
		{
		return; 
		}

	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	HDIB  hnewDIB;           //blur���DIBͼ��
	LPSTR pnewDIB;           //blur��DIBͼ���ָ��
	LONG width;               //ͼ����
	LONG height;              //ͼ��߶�
	LONG factwidth;           //ͼ��DIB���ݿ��ʵ��ʹ�õ��ֽ��� DIBͼ��Ŀ��Ҫ��4byte�ı���
	int i, j;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
 
	/* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;


	//�õ�ͼ��ĸ߶ȺͿ��
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;

	//����DIBͼ�����ݿ��ʵ��ʹ�õ��ֽ���
	factwidth=WIDTHBYTES(width*8);

	//����DIBͼ����ڴ�ռ�
	hnewDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, factwidth*height+1064);
	//�õ���ָ��
	pnewDIB = (LPSTR) ::GlobalLock((HGLOBAL) hnewDIB);
	
	//����DIBͼ�����Ϣͷ����ɫ��
	::memcpy(pnewDIB,lpbi,1064);

	//��̬����������ʱ�洢DIBͼ�����ݵ�����
	BYTE **bmpoldbits;      //ԭͼ������
	bmpoldbits=new BYTE*[factwidth];
	for (i=0;i<factwidth;i++)
		bmpoldbits[i]=new BYTE[height];

	BYTE **bmpnewbits;     //�������ͼ������
	bmpnewbits=new BYTE*[factwidth];
	for (i=0;i<factwidth;i++)
		bmpnewbits[i]=new BYTE[height];

   	//��ȡԭDIBͼ���dib����
	//����ԭͼ���dib���ݵ�ԭͼ������
	for (j=0;j<height;j++)
		for (i=0;i<factwidth;i++)
			::memcpy(&bmpoldbits[i][j],lpbi+1064+i+j*factwidth,1); 
	
	//����blur�����õľ���Ϊ��
	// 1 1 1
	// 1 5 1
	// 1 1 1
	//���ڱ�Ե���õľ���Ϊ
	// 1 5 1
	// 1 1 1
	//�������Ϊ
	// 7 1
	// 1 1
	for (i=0;i<width;i++)
			{
		for (j=0;j<height;j++)
				{
			//�ĸ�����
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
            //�ĸ���Ե��
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
	//�Ѵ�������ͼ������д����ͼ���DIB��
	for (j=0;j<height;j++)
		for (i=0;i<factwidth;i++)
			::memcpy(pnewDIB+1064+i+j*factwidth,&bmpnewbits[i][j],1);

	//�����µ��ĵ�
	POSITION posTemplate = GetFirstDocTemplatePosition();
    CDocTemplate* pDocTemplate = GetNextDocTemplate(posTemplate);
    CMicroscopeDoc* pDocument = (CMicroscopeDoc*) pDocTemplate->OpenDocumentFile(NULL);
	
	//����HDIB�����Ҹ�����ͼ
	pDocument->ReplaceFlag(FALSE); //��дtxt��Ϣ�ļ�
	pDocument->ReplaceHDIB(hnewDIB); 
	pDocument->InitDIBData();    // set up new size & palette
	pDocument->SetModifiedFlag(TRUE);
	pDocument->UpdateAllViews(NULL);
	pDocument->DrawRectTrackerflag=TRUE;  //����Ƥ�����α�־Ϊ��

	::GlobalUnlock((HGLOBAL)hnewDIB);
	::GlobalUnlock((HGLOBAL)hDIB);
	//�ͷŶ�̬������ڴ�
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

	//�õ�MDI����������ָ��
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
		{
		return; // no active MDI child frame 
		}

	//�õ�MDI�����ĵ�ָ��
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //���û��ͼ�񣬾�ʹ�˵����
		{
		return; 
		}

	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	HDIB  hnewDIB;           //pseudocolor���DIBͼ��
	LPSTR pnewDIB;           //pseudocolor��DIBͼ���ָ��
	LONG width;               //ͼ����
	LONG height;              //ͼ��߶�
	LONG factwidth;           //ͼ��DIB���ݿ��ʵ��ʹ�õ��ֽ��� DIBͼ��Ŀ��Ҫ��4byte�ı���
	RGBQUAD bmpcolorpalette[256]; //Ҫд����µ�ɫ������
	BYTE  red,green,blue;    //�û�����ɫ�Ի�����ѡ���RGB��ɫֵ
    int i;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
 
	/* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;


	//�õ�ͼ��ĸ߶ȺͿ��
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;

	//����DIBͼ�����ݿ��ʵ��ʹ�õ��ֽ���
	factwidth=WIDTHBYTES(width*8);

	//α��ɫ������ǰ�ԭͼ��ĵ�ɫ�廻���Լ�����ĵ�ɫ��
	
	//���䴦���ͼ����ڴ�ռ�
	hnewDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, factwidth*height+1064);

	//�õ���ָ��
	pnewDIB = (LPSTR) ::GlobalLock((HGLOBAL) hnewDIB);

	//����ͼ���ļ�����Ϣͷ
	::memcpy(pnewDIB,lpbi,40);

	//�ı��ɫ��
	//�õ��û�ѡ���RGBֵ
	red=GetRValue(m_OptionColor);
	green=GetGValue(m_OptionColor);
	blue=GetBValue(m_OptionColor);

	//�����µ�ɫ��
	for (i=0;i<=255;i++)
		{
		bmpcolorpalette[i].rgbRed=(BYTE)(red+i*(255.0-red)/255.0);
		bmpcolorpalette[i].rgbGreen=(BYTE)(green+i*(255.0-green)/255.0);
		bmpcolorpalette[i].rgbBlue=(BYTE)(blue+i*(255.0-blue)/255.0);
		bmpcolorpalette[i].rgbReserved=0;
		}

	//д���µ�ɫ��
	::memcpy(pnewDIB+40,bmpcolorpalette,1024);
    //����ԭͼ�������
	::memcpy(pnewDIB+1064,lpbi+1064,factwidth*height);

	//�����µ��ĵ�
	POSITION posTemplate = GetFirstDocTemplatePosition();
    CDocTemplate* pDocTemplate = GetNextDocTemplate(posTemplate);
    CMicroscopeDoc* pDocument = (CMicroscopeDoc*) pDocTemplate->OpenDocumentFile(NULL);
	
	//����HDIB�����Ҹ�����ͼ
	pDocument->ReplaceFlag(FALSE); //��дtxt��Ϣ�ļ�
	pDocument->ReplaceHDIB(hnewDIB); 
	pDocument->InitDIBData();    // set up new size & palette
	pDocument->SetModifiedFlag(TRUE);
	pDocument->UpdateAllViews(NULL);
	pDocument->DrawRectTrackerflag=TRUE;  //����Ƥ�����α�־Ϊ��

	::GlobalUnlock((HGLOBAL)hnewDIB);
	::GlobalUnlock((HGLOBAL)hDIB);

	EndWaitCursor();

	return;
}





void CMicroscopeApp::OnBCAdjust() 
{
	BeginWaitCursor();

/***********************************************
    ���ȸ���ԭͼ��һ�µ��ĵ���
************************************************/
	
	//�õ�MDI����������ָ��
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
		{
		return; // no active MDI child frame 
		}

	//�õ�MDI�����ĵ�ָ��
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //���û��ͼ�񣬾ͷ���
		{
		return; 
		}

	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	HDIB  hnewDIB;           //���ƺ��DIBͼ��
	LPSTR pnewDIB;           //���ƺ�DIBͼ���ָ��
	LONG width;               //ͼ����
	LONG height;              //ͼ��߶�
	LONG factwidth;           //ͼ��DIB���ݿ��ʵ��ʹ�õ��ֽ��� DIBͼ��Ŀ��Ҫ��4byte�ı���

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
 
	/* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;


	//�õ�ͼ��ĸ߶ȺͿ��
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;

	//����DIBͼ�����ݿ��ʵ��ʹ�õ��ֽ���
	factwidth=WIDTHBYTES(width*8);

	//���䴦���ͼ����ڴ�ռ�
	hnewDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, factwidth*height+1064);

	//�õ���ָ��
	pnewDIB = (LPSTR) ::GlobalLock((HGLOBAL) hnewDIB);

	//��������ͼ�����ݣ�������Ϣͷ����ɫ�������ֵ
	::memcpy(pnewDIB,lpbi,factwidth*height+1064);

	//�����µ��ĵ�
	POSITION posTemplate = GetFirstDocTemplatePosition();
    CDocTemplate* pDocTemplate = GetNextDocTemplate(posTemplate);
    CMicroscopeDoc* pDocument = (CMicroscopeDoc*) pDocTemplate->OpenDocumentFile(NULL);
	
	//����HDIB�����Ҹ�����ͼ
	pDocument->ReplaceFlag(FALSE); //��дtxt��Ϣ�ļ�
	pDocument->ReplaceHDIB(hnewDIB); 
	pDocument->InitDIBData();    // set up new size & palette
	pDocument->SetModifiedFlag(TRUE);
	pDocument->UpdateAllViews(NULL);
	pDocument->DrawRectTrackerflag=TRUE;  //����Ƥ�����α�־Ϊ��

	::GlobalUnlock((HGLOBAL)hnewDIB);
	::GlobalUnlock((HGLOBAL)hDIB);
	
	EndWaitCursor();

/****************************************
    ԭͼ�������
*****************************************/

/*****************************************
	��ʾ����/�Աȶȵ����Ի���
	���Ը��ƺ��ͼ����е���
******************************************/

	CBCAdjust* dlg=new CBCAdjust;
	
	//���ݸ��ƺ�ͼ����
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

	//�õ�MDI����������ָ��
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
		{
		return; // no active MDI child frame 
		}

	//�õ�MDI�����ĵ�ָ��
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //���û��ͼ�񣬾ͷ���
		{
		return; 
		}

	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LONG width;               //ͼ����
	LONG height;              //ͼ��߶�
	LONG factwidth;           //ͼ��DIB���ݿ��ʵ��ʹ�õ��ֽ��� DIBͼ��Ŀ��Ҫ��4byte�ı���

	float zmax;        //ͼ��Z������ѹֵ
	float zmin;        //ͼ��Z����С��ѹֵ
	int i;
	int j;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
 
	/* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;
	
	CRoughness* dlg=new CRoughness;

	//�õ�ͼ��ĸ߶ȺͿ��
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;
	
	//����DIBͼ�����ݿ��ʵ��ʹ�õ��ֽ���
	factwidth=WIDTHBYTES(width*8);
	
	//����ͼ��ĸ߶ȺͿ��
    dlg->imagesizex=lpbmi->bmiHeader.biWidth;
	dlg->imagesizey=lpbmi->bmiHeader.biHeight;

	//����ͼ���ɨ�跶Χ
	dlg->scanrangex=lpbmi->bmiHeader.biSizeImage;
	dlg->scanrangey=lpbmi->bmiHeader.biXPelsPerMeter;

	//�õ�ͼ��Z��������С��ѹֵ
	zmin=(float)((lpbmi->bmiHeader.biYPelsPerMeter)/1000.0);
	zmax=(float)((lpbmi->bmiHeader.biClrImportant)/1000.0);

	//��̬����������ʱ�洢ԭDIBͼ�����ݵ�����
	BYTE **bmpbits;      //ͼ������
	bmpbits=new BYTE*[factwidth];
	for (i=0;i<factwidth;i++)
		bmpbits[i]=new BYTE[height];

	//��ȡԭDIBͼ���dib����
	//����ԭͼ���dib���ݵ�ͼ������
	for (j=0;j<height;j++)
		for (i=0;i<factwidth;i++)
			::memcpy(&bmpbits[i][j],lpbi+1064+i+j*factwidth,1); 

	/////////////////////////////////////////////
	//����ֲڶ�Ra��Ry��Rz
	//Ra����
	//��bmpbits�������õ������ĵ�ѹֵ�����������е�ƽ��ֵ�����ֵ����Сֵ
    float Raavetemp; //����Raʱ����Z�����ѹƽ��ֵ����ʱ����
	float Rmaxtemp;  //����Ry��Rzʱ����Z�����ѹ���ֵ����ʱ����
	float Rmintemp;  //����Ry��Rzʱ����Z�����ѹ��Сֵ����ʱ����
	float Ratemp;     //Ra����ʱ����
	float Rytemp;     //Ry����ʱ����
	float Rztemp;     //Rz����ʱ����

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

    //����Ra,Ry��Rz
	dlg->Ra=Ratemp;
	dlg->Ry=Rytemp;
	dlg->Rz=Rztemp;

	//�ͷŶ�̬������ڴ�
	for (i=0;i<factwidth;i++)
	{
		delete bmpbits[i];
	}

	dlg->DoModal();

 	delete dlg;

	return;

}
