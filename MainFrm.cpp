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
	m_wndToolBar.SetWindowText ("工具栏"); 

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
	BOOL bmp3dflag=FALSE; //bmp3d的标志
 
   //得到MDI程序活动子窗口指针
	CMDIChildWnd* pChild = MDIGetActive();

	if (pChild == NULL)
		{
		pCmdUI->Enable(bmp3dflag);
		return; // no active MDI child frame 
		}

	//得到MDI程序活动文档指针
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //如果没有图像，就使菜单变灰
		{
		pCmdUI->Enable(bmp3dflag);
		return; 
		}

	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPSTR lpbi;              // pointer to packed-DIB
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	LONG width;               //图像宽度
	LONG height;              //图像高度
	WORD wNumColors;         // number of colors in color table
	int i;

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	/* is this a Win 3.0 DIB? */
	bWinStyleDIB = IS_WIN30_DIB(lpbi);
	if (!bWinStyleDIB)
		{
		pCmdUI->Enable(bmp3dflag);
		::GlobalUnlock((HGLOBAL) hDIB);
		return;  //如果不是Win3.0 DIB格式的图像就使菜单变灰
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//   if (wNumColors!=256)
//		{
//		pCmdUI->Enable(bmp3dflag);
//		::GlobalUnlock((HGLOBAL) hDIB);
//		return; //如果不是256色图像，就使菜单变灰
//		}
    //下面根据是否是小于400*400图像，以及调色板是否是从(0,0,0,0)到(255,255,255,0)递增的图像（即是否是本扫描探针显微镜软件生成的图像），如果是，那么就把bmp3d标示设为真
	//得到图像的高度和宽度
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;

	if (width<=800&&height<=800)
		bmp3dflag=TRUE;  //如果是小于400*400的图像，bmp3d标志为真

	if (bmp3dflag==FALSE)
		{
		pCmdUI->Enable(FALSE);
		::GlobalUnlock((HGLOBAL) hDIB);
		return ;  //如果不是200或400的图像，菜单变灰，函数返回
		}	
	
	//检查图像的调色板是否符合要求，只要有一点不符合要求，就设bmp3d为假，退出循环
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

    if (bmp3dflag==FALSE) //如果bmp3d标志仍为假，那么菜单为灰
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
    BOOL blurflag=FALSE; //blur的标志

	//得到MDI程序活动子窗口指针
	CMDIChildWnd* pChild = MDIGetActive();
	
	if (pChild == NULL)
		{
		pCmdUI->Enable(blurflag);
		return; // no active MDI child frame 
		}

	//得到MDI程序活动文档指针
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //如果没有图像，就使菜单变灰
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
		return;  //如果不是Win3.0 DIB格式的图像就使菜单变灰
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//    if (wNumColors!=256)
//		{
//		pCmdUI->Enable(blurflag);
//		::GlobalUnlock((HGLOBAL) hDIB);
//		return; //如果不是256色图像，就使菜单变灰
//		}
    //下面根据以及调色板是否是从(0,0,0,0)到(255,255,255,0)递增的图像（即是否是本扫描探针显微镜软件生成的图像），如果是，那么就把blur标示设为真	
		blurflag=TRUE;
	//检查图像的调色板是否符合要求，只要有一点不符合要求，就设blur为假，退出循环
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

    if (blurflag==FALSE) //如果blur标志仍为假，那么菜单为灰
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
	BOOL pseudocolorflag=FALSE; //pseudocolor的标志

    //得到MDI程序活动子窗口指针
	CMDIChildWnd* pChild = MDIGetActive();

	if (pChild == NULL)
		{
		pCmdUI->Enable(pseudocolorflag);
		return; // no active MDI child frame 
		}

	//得到MDI程序活动文档指针
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //如果没有图像，就使菜单变灰
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
		return;  //如果不是Win3.0 DIB格式的图像就使菜单变灰
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//    if (wNumColors!=256)
//		{
//		pCmdUI->Enable(pseudocolorflag);
//		::GlobalUnlock((HGLOBAL) hDIB);
//		return; //如果不是256色图像，就使菜单变灰
//		}
    //下面根据调色板是否是从(0,0,0,0)到(255,255,255,0)递增的图像（即是否是本扫描探针显微镜软件生成的图像），如果是，那么就把pseudocolor标示设为真
		pseudocolorflag=TRUE;
	//检查图像的调色板是否符合要求，只要有一点不符合要求，就设pseudocolor为假，退出循环
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

    if (pseudocolorflag==FALSE) //如果pseudocolor标志仍为假，那么菜单为灰
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
    //得到MDI程序活动子窗口指针
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
	BOOL BCflag=FALSE; //BCAdjust的标志	

	//得到MDI程序活动子窗口指针
	CMDIChildWnd* pChild = MDIGetActive();
	
	if (pChild == NULL)
		{
		pCmdUI->Enable(BCflag);
		return; // no active MDI child frame 
		}

	//得到MDI程序活动文档指针
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //如果没有图像，就使菜单变灰
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
		return;  //如果不是Win3.0 DIB格式的图像就使菜单变灰
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//    if (wNumColors!=256)
//		{
//		pCmdUI->Enable(BCflag);
//		::GlobalUnlock((HGLOBAL) hDIB);
//		return; //如果不是256色图像，就使菜单变灰
//		}
    //下面根据调色板是否是从(0,0,0,0)到(255,255,255,0)递增的图像（即是否是本扫描探针显微镜软件生成的图像），如果是，那么就把BCflag标示设为真
		BCflag=TRUE;
	//检查图像的调色板是否符合要求，只要有一点不符合要求，就设BCAdjust为假，退出循环
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

    if (BCflag==FALSE) //如果BCAdjust标志仍为假，那么菜单为灰
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
    BOOL roughnessflag=FALSE; //roughness的标志

	//得到MDI程序活动子窗口指针
	CMDIChildWnd* pChild = MDIGetActive();
	
	if (pChild == NULL)
		{
		pCmdUI->Enable(roughnessflag);
		return; // no active MDI child frame 
		}

	//得到MDI程序活动文档指针
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //如果没有图像，就使菜单变灰
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
		return;  //如果不是Win3.0 DIB格式的图像就使菜单变灰
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//   if (wNumColors!=256)
//		{
//		pCmdUI->Enable(roughnessflag);
//		::GlobalUnlock((HGLOBAL) hDIB);
//		return; //如果不是256色图像，就使菜单变灰
//		}

	//如果biClrImportant位(存放为图像Z方向电压值的最大值信息)不是在0－4096=5000之间，就使菜单变灰
	if ((lpbmi->bmiHeader.biClrImportant)<=0||(lpbmi->bmiHeader.biClrImportant)>5000)
		{
		pCmdUI->Enable(roughnessflag);
		::GlobalUnlock((HGLOBAL) hDIB);
		return; 
		}

    //下面根据以及调色板是否是从(0,0,0,0)到(255,255,255,0)递增的图像（即是否是本扫描探针显微镜软件生成的图像），如果是，那么就把roughness标示设为真	
		roughnessflag=TRUE;
	//检查图像的调色板是否符合要求，只要有一点不符合要求，就设roughness为假，退出循环
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

    if (roughnessflag==FALSE) //如果roughness标志仍为假，那么菜单为灰
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
    BOOL labelflag=FALSE; //label的标志，加标尺的标志

	//得到MDI程序活动子窗口指针
	CMDIChildWnd* pChild = MDIGetActive();
	
	if (pChild == NULL)
		{
		pCmdUI->Enable(labelflag);
		return; // no active MDI child frame 
		}

	//得到MDI程序活动文档指针
	CMicroscopeDoc* pDoc=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc->GetHDIB();

	if (hDIB == NULL)  //如果没有图像，就使菜单变灰
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
		return;  //如果不是Win3.0 DIB格式的图像就使菜单变灰
		}

    /* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	/* get the number of colors in the DIB */
    wNumColors = ::DIBNumColors(lpbi);
//   if (wNumColors!=256)
//		{
//		pCmdUI->Enable(labelflag);
//		::GlobalUnlock((HGLOBAL) hDIB);
//		return; //如果不是256色图像，就使菜单变灰
//		}
    //下面根据以及调色板是否是从(0,0,0,0)到(255,255,255,0)递增的图像（即是否是本扫描探针显微镜软件生成的图像），如果是，那么就把label标示设为真	
		labelflag=TRUE;
	//检查图像的调色板是否符合要求，只要有一点不符合要求，就设blur为假，退出循环
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

    if (labelflag==FALSE) //如果label标志仍为假，那么菜单为灰
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
