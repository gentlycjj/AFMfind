// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "microscope.h"
#include "MainFrm.h"
#include "microscopeDoc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_PALETTECHANGED()
	ON_WM_QUERYNEWPALETTE()
	ON_UPDATE_COMMAND_UI(ID_3D, OnUpdate3d)
	ON_UPDATE_COMMAND_UI(ID_BLUR, OnUpdateBlur)
	ON_UPDATE_COMMAND_UI(IDC_PSEUDOCOLOR, OnUpdatePseudocolor)
	ON_UPDATE_COMMAND_UI(IDC_SETCOLOR, OnUpdateSetcolor)
	ON_UPDATE_COMMAND_UI(IDC_BCADJUST, OnUpdateBCAdjust)
	ON_UPDATE_COMMAND_UI(ID_ROUGHNESS, OnUpdateRoughness)
	ON_UPDATE_COMMAND_UI(ID_SIZE, OnUpdateSize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}
CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// Set the caption of the toolbar . 
	m_wndToolBar.SetWindowText ("������"); 

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CMDIFrameWnd::OnPaletteChanged(pFocusWnd);
	// always realize the palette for the active view
	CMDIChildWnd* pMDIChildWnd = MDIGetActive();
	if (pMDIChildWnd == NULL)
		return; // no active MDI child frame
	CView* pView = pMDIChildWnd->GetActiveView();
	ASSERT(pView != NULL);

	// notify all child windows that the palette has changed
	SendMessageToDescendants(WM_DOREALIZE, (WPARAM)pView->m_hWnd);	
}

BOOL CMainFrame::OnQueryNewPalette() 
{
	// always realize the palette for the active view
	CMDIChildWnd* pMDIChildWnd = MDIGetActive();
	if (pMDIChildWnd == NULL)
		return FALSE; // no active MDI child frame (no new palette)
	CView* pView = pMDIChildWnd->GetActiveView();
	ASSERT(pView != NULL);

	// just notify the target view
	pView->SendMessage(WM_DOREALIZE, (WPARAM)pView->m_hWnd);
	return TRUE;
}

void CMainFrame::OnUpdate3d(CCmdUI* pCmdUI) 
{
	BOOL bmp3dflag=FALSE; //bmp3d�ı�־
 
   //�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = MDIGetActive();

	if (pChild == NULL)
		{
		pCmdUI->Enable(bmp3dflag);
		return; // no active MDI child frame 
		}

	//�õ�MDI�����ĵ�ָ��
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //���û��ͼ�񣬾�ʹ�˵����
		{
		pCmdUI->Enable(bmp3dflag);
		return; 
		}

	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPSTR lpbi;              // pointer to packed-DIB
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	LONG width;               //ͼ����
	LONG height;              //ͼ��߶�
	WORD wNumColors;         // number of colors in color table
	int i;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	/* is this a Win 3.0 DIB? */
	bWinStyleDIB = IS_WIN30_DIB(lpbi);
	if (!bWinStyleDIB)
		{
		pCmdUI->Enable(bmp3dflag);
		::GlobalUnlock((HGLOBAL) hDIB);
		return;  //�������Win3.0 DIB��ʽ��ͼ���ʹ�˵����
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//   if (wNumColors!=256)
//		{
//		pCmdUI->Enable(bmp3dflag);
//		::GlobalUnlock((HGLOBAL) hDIB);
//		return; //�������256ɫͼ�񣬾�ʹ�˵����
//		}
    //��������Ƿ���С��400*400ͼ���Լ���ɫ���Ƿ��Ǵ�(0,0,0,0)��(255,255,255,0)������ͼ�񣨼��Ƿ��Ǳ�ɨ��̽����΢��������ɵ�ͼ�񣩣�����ǣ���ô�Ͱ�bmp3d��ʾ��Ϊ��
	//�õ�ͼ��ĸ߶ȺͿ��
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;

	if (width<=800&&height<=800)
		bmp3dflag=TRUE;  //�����С��400*400��ͼ��bmp3d��־Ϊ��

	if (bmp3dflag==FALSE)
		{
		pCmdUI->Enable(FALSE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return ;  //�������200��400��ͼ�񣬲˵���ң���������
		}	
	
	//���ͼ��ĵ�ɫ���Ƿ����Ҫ��ֻҪ��һ�㲻����Ҫ�󣬾���bmp3dΪ�٣��˳�ѭ��
	for (i = 1; i < (int)wNumColors; i++)
		{
		if (lpbmi->bmiColors[i].rgbRed<lpbmi->bmiColors[i-1].rgbRed)
			{
			bmp3dflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbGreen<lpbmi->bmiColors[i-1].rgbGreen)
			{
			bmp3dflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbBlue<lpbmi->bmiColors[i-1].rgbBlue)
			{
			bmp3dflag=FALSE;
			break;
			}
		}

    if (bmp3dflag==FALSE) //���bmp3d��־��Ϊ�٣���ô�˵�Ϊ��
		{
		pCmdUI->Enable(FALSE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return ;
		}
	else{
		pCmdUI->Enable(TRUE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return;
		}			
}

void CMainFrame::OnUpdateBlur(CCmdUI* pCmdUI) 
{
    BOOL blurflag=FALSE; //blur�ı�־

	//�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = MDIGetActive();
	
	if (pChild == NULL)
		{
		pCmdUI->Enable(blurflag);
		return; // no active MDI child frame 
		}

	//�õ�MDI�����ĵ�ָ��
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //���û��ͼ�񣬾�ʹ�˵����
		{
		pCmdUI->Enable(blurflag);
		return; 
		}

	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPSTR lpbi;              // pointer to packed-DIB
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	WORD wNumColors;         // number of colors in color table
	int i;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	/* is this a Win 3.0 DIB? */
	bWinStyleDIB = IS_WIN30_DIB(lpbi);
	if (!bWinStyleDIB)
		{
		pCmdUI->Enable(blurflag);
		::GlobalUnlock((HGLOBAL) hDIB);
		return;  //�������Win3.0 DIB��ʽ��ͼ���ʹ�˵����
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//    if (wNumColors!=256)
//		{
//		pCmdUI->Enable(blurflag);
//		::GlobalUnlock((HGLOBAL) hDIB);
//		return; //�������256ɫͼ�񣬾�ʹ�˵����
//		}
    //��������Լ���ɫ���Ƿ��Ǵ�(0,0,0,0)��(255,255,255,0)������ͼ�񣨼��Ƿ��Ǳ�ɨ��̽����΢��������ɵ�ͼ�񣩣�����ǣ���ô�Ͱ�blur��ʾ��Ϊ��	
		blurflag=TRUE;
	//���ͼ��ĵ�ɫ���Ƿ����Ҫ��ֻҪ��һ�㲻����Ҫ�󣬾���blurΪ�٣��˳�ѭ��
	for (i = 1; i < (int)wNumColors; i++)
		{
		if (lpbmi->bmiColors[i].rgbRed<lpbmi->bmiColors[i-1].rgbRed)
			{
			blurflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbGreen<lpbmi->bmiColors[i-1].rgbGreen)
			{
			blurflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbBlue<lpbmi->bmiColors[i-1].rgbBlue)
			{
			blurflag=FALSE;
			break;
			}
		}

    if (blurflag==FALSE) //���blur��־��Ϊ�٣���ô�˵�Ϊ��
		{
		pCmdUI->Enable(FALSE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return ;
		}
	else{
		pCmdUI->Enable(TRUE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return;
		}	
}

void CMainFrame::OnUpdatePseudocolor(CCmdUI* pCmdUI) 
{
	BOOL pseudocolorflag=FALSE; //pseudocolor�ı�־

    //�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = MDIGetActive();

	if (pChild == NULL)
		{
		pCmdUI->Enable(pseudocolorflag);
		return; // no active MDI child frame 
		}

	//�õ�MDI�����ĵ�ָ��
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //���û��ͼ�񣬾�ʹ�˵����
		{
		pCmdUI->Enable(pseudocolorflag);
		return; 
		}

	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPSTR lpbi;              // pointer to packed-DIB
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	WORD wNumColors;         // number of colors in color table
	int i;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	/* is this a Win 3.0 DIB? */
	bWinStyleDIB = IS_WIN30_DIB(lpbi);
	if (!bWinStyleDIB)
		{
		pCmdUI->Enable(pseudocolorflag);
		::GlobalUnlock((HGLOBAL) hDIB);
		return;  //�������Win3.0 DIB��ʽ��ͼ���ʹ�˵����
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//    if (wNumColors!=256)
//		{
//		pCmdUI->Enable(pseudocolorflag);
//		::GlobalUnlock((HGLOBAL) hDIB);
//		return; //�������256ɫͼ�񣬾�ʹ�˵����
//		}
    //������ݵ�ɫ���Ƿ��Ǵ�(0,0,0,0)��(255,255,255,0)������ͼ�񣨼��Ƿ��Ǳ�ɨ��̽����΢��������ɵ�ͼ�񣩣�����ǣ���ô�Ͱ�pseudocolor��ʾ��Ϊ��
		pseudocolorflag=TRUE;
	//���ͼ��ĵ�ɫ���Ƿ����Ҫ��ֻҪ��һ�㲻����Ҫ�󣬾���pseudocolorΪ�٣��˳�ѭ��
	for (i = 1; i < (int)wNumColors; i++)
		{
		if (lpbmi->bmiColors[i].rgbRed<lpbmi->bmiColors[i-1].rgbRed)
			{
			pseudocolorflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbGreen<lpbmi->bmiColors[i-1].rgbGreen)
			{
			pseudocolorflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbBlue<lpbmi->bmiColors[i-1].rgbBlue)
			{
			pseudocolorflag=FALSE;
			break;
			}
		}

    if (pseudocolorflag==FALSE) //���pseudocolor��־��Ϊ�٣���ô�˵�Ϊ��
		{
		pCmdUI->Enable(FALSE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return ;
		}
	else{
		pCmdUI->Enable(TRUE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return;
		}			
}

void CMainFrame::OnUpdateSetcolor(CCmdUI* pCmdUI) 
{
    //�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = MDIGetActive();
	
	if (pChild == NULL)
		{
		pCmdUI->Enable(FALSE);
		return; // no active MDI child frame 
		}
	else{
		pCmdUI->Enable(TRUE);
		return;
		}		
}

void CMainFrame::OnUpdateBCAdjust(CCmdUI* pCmdUI) 
{
	BOOL BCflag=FALSE; //BCAdjust�ı�־	

	//�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = MDIGetActive();
	
	if (pChild == NULL)
		{
		pCmdUI->Enable(BCflag);
		return; // no active MDI child frame 
		}

	//�õ�MDI�����ĵ�ָ��
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //���û��ͼ�񣬾�ʹ�˵����
		{
		pCmdUI->Enable(BCflag);
		return; 
		}

	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPSTR lpbi;              // pointer to packed-DIB
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	WORD wNumColors;         // number of colors in color table
	int i;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	/* is this a Win 3.0 DIB? */
	bWinStyleDIB = IS_WIN30_DIB(lpbi);
	if (!bWinStyleDIB)
		{
		pCmdUI->Enable(BCflag);
		::GlobalUnlock((HGLOBAL) hDIB);
		return;  //�������Win3.0 DIB��ʽ��ͼ���ʹ�˵����
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//    if (wNumColors!=256)
//		{
//		pCmdUI->Enable(BCflag);
//		::GlobalUnlock((HGLOBAL) hDIB);
//		return; //�������256ɫͼ�񣬾�ʹ�˵����
//		}
    //������ݵ�ɫ���Ƿ��Ǵ�(0,0,0,0)��(255,255,255,0)������ͼ�񣨼��Ƿ��Ǳ�ɨ��̽����΢��������ɵ�ͼ�񣩣�����ǣ���ô�Ͱ�BCflag��ʾ��Ϊ��
		BCflag=TRUE;
	//���ͼ��ĵ�ɫ���Ƿ����Ҫ��ֻҪ��һ�㲻����Ҫ�󣬾���BCAdjustΪ�٣��˳�ѭ��
	for (i = 1; i < (int)wNumColors; i++)
		{
		if (lpbmi->bmiColors[i].rgbRed<lpbmi->bmiColors[i-1].rgbRed)
			{
			BCflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbGreen<lpbmi->bmiColors[i-1].rgbGreen)
			{
			BCflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbBlue<lpbmi->bmiColors[i-1].rgbBlue)
			{
			BCflag=FALSE;
			break;
			}
		}

    if (BCflag==FALSE) //���BCAdjust��־��Ϊ�٣���ô�˵�Ϊ��
		{
		pCmdUI->Enable(FALSE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return ;
		}
	else{
		pCmdUI->Enable(TRUE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return;
		}	
}

void CMainFrame::OnUpdateRoughness(CCmdUI* pCmdUI) 
{
    BOOL roughnessflag=FALSE; //roughness�ı�־

	//�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = MDIGetActive();
	
	if (pChild == NULL)
		{
		pCmdUI->Enable(roughnessflag);
		return; // no active MDI child frame 
		}

	//�õ�MDI�����ĵ�ָ��
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //���û��ͼ�񣬾�ʹ�˵����
		{
		pCmdUI->Enable(roughnessflag);
		return; 
		}

	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPSTR lpbi;              // pointer to packed-DIB
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	WORD wNumColors;         // number of colors in color table
	int i;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	/* is this a Win 3.0 DIB? */
	bWinStyleDIB = IS_WIN30_DIB(lpbi);
	if (!bWinStyleDIB)
		{
		pCmdUI->Enable(roughnessflag);
		::GlobalUnlock((HGLOBAL) hDIB);
		return;  //�������Win3.0 DIB��ʽ��ͼ���ʹ�˵����
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//   if (wNumColors!=256)
//		{
//		pCmdUI->Enable(roughnessflag);
//		::GlobalUnlock((HGLOBAL) hDIB);
//		return; //�������256ɫͼ�񣬾�ʹ�˵����
//		}

	//���biClrImportantλ(���Ϊͼ��Z�����ѹֵ�����ֵ��Ϣ)������0��4096=5000֮�䣬��ʹ�˵����
	if ((lpbmi->bmiHeader.biClrImportant)<=0||(lpbmi->bmiHeader.biClrImportant)>5000)
		{
		pCmdUI->Enable(roughnessflag);
		::GlobalUnlock((HGLOBAL) hDIB);
		return; 
		}

    //��������Լ���ɫ���Ƿ��Ǵ�(0,0,0,0)��(255,255,255,0)������ͼ�񣨼��Ƿ��Ǳ�ɨ��̽����΢��������ɵ�ͼ�񣩣�����ǣ���ô�Ͱ�roughness��ʾ��Ϊ��	
		roughnessflag=TRUE;
	//���ͼ��ĵ�ɫ���Ƿ����Ҫ��ֻҪ��һ�㲻����Ҫ�󣬾���roughnessΪ�٣��˳�ѭ��
	for (i = 1; i < (int)wNumColors; i++)
		{
		if (lpbmi->bmiColors[i].rgbRed<lpbmi->bmiColors[i-1].rgbRed)
			{
			roughnessflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbGreen<lpbmi->bmiColors[i-1].rgbGreen)
			{
			roughnessflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbBlue<lpbmi->bmiColors[i-1].rgbBlue)
			{
			roughnessflag=FALSE;
			break;
			}
		}

    if (roughnessflag==FALSE) //���roughness��־��Ϊ�٣���ô�˵�Ϊ��
		{
		pCmdUI->Enable(FALSE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return ;
		}
	else{
		pCmdUI->Enable(TRUE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return;
		}	

}

void CMainFrame::OnUpdateSize(CCmdUI* pCmdUI) 
{
    BOOL labelflag=FALSE; //label�ı�־���ӱ�ߵı�־

	//�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = MDIGetActive();
	
	if (pChild == NULL)
		{
		pCmdUI->Enable(labelflag);
		return; // no active MDI child frame 
		}

	//�õ�MDI�����ĵ�ָ��
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //���û��ͼ�񣬾�ʹ�˵����
		{
		pCmdUI->Enable(labelflag);
		return; 
		}

	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPSTR lpbi;              // pointer to packed-DIB
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	WORD wNumColors;         // number of colors in color table
	int i;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	/* is this a Win 3.0 DIB? */
	bWinStyleDIB = IS_WIN30_DIB(lpbi);
	if (!bWinStyleDIB)
		{
		pCmdUI->Enable(labelflag);
		::GlobalUnlock((HGLOBAL) hDIB);
		return;  //�������Win3.0 DIB��ʽ��ͼ���ʹ�˵����
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//   if (wNumColors!=256)
//		{
//		pCmdUI->Enable(labelflag);
//		::GlobalUnlock((HGLOBAL) hDIB);
//		return; //�������256ɫͼ�񣬾�ʹ�˵����
//		}
    //��������Լ���ɫ���Ƿ��Ǵ�(0,0,0,0)��(255,255,255,0)������ͼ�񣨼��Ƿ��Ǳ�ɨ��̽����΢��������ɵ�ͼ�񣩣�����ǣ���ô�Ͱ�label��ʾ��Ϊ��	
		labelflag=TRUE;
	//���ͼ��ĵ�ɫ���Ƿ����Ҫ��ֻҪ��һ�㲻����Ҫ�󣬾���blurΪ�٣��˳�ѭ��
	for (i = 1; i < (int)wNumColors; i++)
		{
		if (lpbmi->bmiColors[i].rgbRed<lpbmi->bmiColors[i-1].rgbRed)
			{
			labelflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbGreen<lpbmi->bmiColors[i-1].rgbGreen)
			{
			labelflag=FALSE;
			break;
			}
		if (lpbmi->bmiColors[i].rgbBlue<lpbmi->bmiColors[i-1].rgbBlue)
			{
			labelflag=FALSE;
			break;
			}
		}

    if (labelflag==FALSE) //���label��־��Ϊ�٣���ô�˵�Ϊ��
		{
		pCmdUI->Enable(FALSE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return ;
		}
	else{
		pCmdUI->Enable(TRUE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return;
		}	
	
}
