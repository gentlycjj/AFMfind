// microscopeView.cpp : implementation of the CMicroscopeView class
//

#include "stdafx.h"
#include "microscope.h"

#include "microscopeDoc.h"
#include "microscopeView.h"
#include "dibapi.h"
#include "mainfrm.h"
#include "sizedlg.h"
#include "math.h"

#include "Measure.h"
#include "conio.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeView

IMPLEMENT_DYNCREATE(CMicroscopeView, CScrollView)

BEGIN_MESSAGE_MAP(CMicroscopeView, CScrollView)
	//{{AFX_MSG_MAP(CMicroscopeView)
	ON_MESSAGE(WM_DOREALIZE, OnDoRealize)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SIZE, OnSize)
	ON_WM_KEYDOWN()  
	ON_WM_KEYUP()

	//以上两个函数，是为Shift选择正方形区域而设 //


	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMicroscopeView construction/destruction

CMicroscopeView::CMicroscopeView()
{
	ShowRectTrackerflag=FALSE;
	Keyflag=FALSE;
}

CMicroscopeView::~CMicroscopeView()
{
}

BOOL CMicroscopeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeView drawing

void CMicroscopeView::OnDraw(CDC* pDC)
{
	CMicroscopeDoc* pDoc = GetDocument();
	HDIB hDIB = pDoc->GetHDIB();
	if (hDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
		int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
		::GlobalUnlock((HGLOBAL) hDIB);
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		CRect rcDest;
		if (pDC->IsPrinting())   // printer DC
		{
			// get size of printer page (in pixels)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			// get printer pixels per inch
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

			//
			// Best Fit case -- create a rectangle which preserves
			// the DIB's aspect ratio, and fills the page horizontally.
			//
			// The formula in the "->bottom" field below calculates the Y
			// position of the printed bitmap, based on the size of the
			// bitmap, the width of the page, and the relative size of
			// a printed pixel (cyInch / cxInch).
			//
			rcDest.top = rcDest.left =0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)
					/ ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
		}
		else   // not printer DC
		{
			rcDest = rcDIB;
		}
		::PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());
	}
	//显示所画的橡皮区矩形
	LONG width;               //图像宽度 2008.01.15
	LONG height;              //图像高度 2008.01.15



if ((pDoc->DrawRectTrackerflag&&ShowRectTrackerflag)==TRUE)
{
	LPBITMAPINFO lpbmi;
	LPSTR lpbi;
	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	lpbmi=(LPBITMAPINFO)lpbi;
	CString tmp;

    //得到图像的高度和宽度 
    width=lpbmi->bmiHeader.biWidth;  // 2008.01.15
	height=lpbmi->bmiHeader.biHeight; // 2008.01.15

	pDoc->RectTracker.Draw(pDC);

////////////////////////////////////////////////////////////////// 2008.01.15				
//	tmp.Format("X: %.4d",(pDoc->RectTracker.m_rect.right-pDoc->RectTracker.m_rect.left+1)*lpbmi->bmiHeader.biXPelsPerMeter/400);
	tmp.Format("X: %.4d",(pDoc->RectTracker.m_rect.right-pDoc->RectTracker.m_rect.left+1)*lpbmi->bmiHeader.biXPelsPerMeter/width);
	pDC->TextOut(340,370,tmp); 

//	tmp.Format("Y: %.4d",(pDoc->RectTracker.m_rect.bottom-pDoc->RectTracker.m_rect.top+1)*lpbmi->bmiHeader.biXPelsPerMeter/400);
	tmp.Format("Y: %.4d",(pDoc->RectTracker.m_rect.bottom-pDoc->RectTracker.m_rect.top+1)*lpbmi->bmiHeader.biXPelsPerMeter/height);
	pDC->TextOut(340,385,tmp);
////////////////////////////////////////////////////////////////// 2008.01.15
}


}

void CMicroscopeView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	ASSERT(GetDocument() != NULL);

	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
}

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeView printing

BOOL CMicroscopeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMicroscopeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMicroscopeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeView diagnostics

#ifdef _DEBUG
void CMicroscopeView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMicroscopeView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMicroscopeDoc* CMicroscopeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMicroscopeDoc)));
	return (CMicroscopeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMicroscopeView message handlers

void CMicroscopeView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		ASSERT(pActivateView == this);
		OnDoRealize((WPARAM)m_hWnd, 0);   // same as SendMessage(WM_DOREALIZE);
	}
}
LRESULT CMicroscopeView::OnDoRealize(WPARAM wParam, LPARAM)
{
	ASSERT(wParam != NULL);
	CMicroscopeDoc* pDoc = GetDocument();
	if (pDoc->GetHDIB() == NULL)
		return 0L;  // must be a new document

	CPalette* pPal = pDoc->GetDocPalette();
	if (pPal != NULL)
	{
		CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		ASSERT_KINDOF(CMainFrame, pAppFrame);

		CClientDC appDC(pAppFrame);
		// All views but one should be a background palette.
		// wParam contains a handle to the active view, so the SelectPalette
		// bForceBackground flag is FALSE only if wParam == m_hWnd (this view)
		CPalette* oldPalette = appDC.SelectPalette(pPal, ((HWND)wParam) != m_hWnd);
		if (oldPalette != NULL)
		{
			UINT nColorsChanged = appDC.RealizePalette();
			if (nColorsChanged > 0)
				pDoc->UpdateAllViews(NULL);
			appDC.SelectPalette(oldPalette, TRUE);
		}
		else
		{
			TRACE0("\tSelectPalette failed in CDibView::OnPaletteChanged\n");
		}
	}
	
	return 0L;
}

void CMicroscopeView::OnEditCopy() 
{
	CMicroscopeDoc* pDoc = GetDocument();
	// Clean clipboard of contents, and copy the DIB.

	if (OpenClipboard())
	{
		//分两种情况1.没有选择橡皮区矩形；2.有选择橡皮区矩形
		//1.没有选择橡皮区矩形
		if (ShowRectTrackerflag==FALSE)
		{
			BeginWaitCursor();
			EmptyClipboard();
			SetClipboardData (CF_DIB, CopyHandle((HANDLE) pDoc->GetHDIB()) );
			CloseClipboard();
			EndWaitCursor();
		}
		//2.有选择橡皮区矩形
		if (ShowRectTrackerflag==TRUE)
		{
			BeginWaitCursor();
          
		    //*************************************
			//建立选择区域的图像的句柄

			LPSTR lpbi;              // pointer to packed-DIB 整个图像
			LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0) 整个图像
			HDIB  hnewDIB;           //选择区域内的DIB图像句柄
			BITMAPINFOHEADER bmpnewheader; //选择区域内的DIB图像的信息头
			LPSTR pnewDIB;           //选择区域内的DIB图像的指针
			LONG oldwidth;               //整个图像宽度
			LONG oldheight;              //整个图像高度
			LONG oldfactwidth;           //整个图像宽度实际使用的字节数 DIB图像的宽度要是4byte的倍数
			LONG newwidth;               //选择区域内图像宽度
			LONG newheight;              //选择区域内图像高度
			LONG newfactwidth;           //选择区域内图像宽度实际使用的字节数 DIB图像的宽度要是4byte的倍数
			int i, j;

			lpbi = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHDIB());
 
			/* get pointer to BITMAPINFO (Win 3.0) */
			lpbmi = (LPBITMAPINFO)lpbi;

			//得到整个图像的高度和宽度
			oldwidth=lpbmi->bmiHeader.biWidth;
			oldheight=lpbmi->bmiHeader.biHeight;

			//计算整个DIB图像数据宽度实际使用的字节数
			oldfactwidth=WIDTHBYTES(oldwidth*8);

			//计算选择区域内图像宽度和高度
			newwidth=pDoc->RectTracker.m_rect.right - pDoc->RectTracker.m_rect.left + 1;
			newheight=pDoc->RectTracker.m_rect.bottom - pDoc->RectTracker.m_rect.top + 1;

			//计算选择区域内图像宽度实际使用的字节数
			newfactwidth=WIDTHBYTES(newwidth*8);

			//分配DIB图像的内存空间
			hnewDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, newfactwidth*newheight+1064);
			//得到其指针
			pnewDIB = (LPSTR) ::GlobalLock((HGLOBAL) hnewDIB);

			//动态申请用来存储整个图像数据和选择区域内图像数据的数组
			BYTE **bmpoldbits;      //整个图像数组
			bmpoldbits=new BYTE*[oldfactwidth];
			for (i=0;i<oldfactwidth;i++)
				bmpoldbits[i]=new BYTE[oldheight];

			BYTE **bmpnewbits;     //选择区域内图像数组
			bmpnewbits=new BYTE*[newfactwidth];
			for (i=0;i<newfactwidth;i++)
				bmpnewbits[i]=new BYTE[newheight];

		   	//读取原整个DIB图像的dib数据
			//拷贝整个图像的dib数据到整个图像数组
			for (j=0;j<oldheight;j++)
				for (i=0;i<oldfactwidth;i++)
					::memcpy(&bmpoldbits[i][j],lpbi+1064+i+j*oldfactwidth,1); 

			//拷贝选中的区域到选择区域内图像数组
			for (j=0;j<newheight;j++)
				for (i=0;i<newwidth;i++)
					bmpnewbits[i][j]=bmpoldbits[i+pDoc->RectTracker.m_rect.left][j+oldheight-1-pDoc->RectTracker.m_rect.bottom];

			//写选择区域内图像的信息头
			bmpnewheader.biSize=40;
			bmpnewheader.biWidth=newwidth;
			bmpnewheader.biHeight=newheight;
			bmpnewheader.biPlanes=1;
			bmpnewheader.biBitCount=8;
			bmpnewheader.biCompression=0;
			bmpnewheader.biSizeImage=(DWORD)((lpbmi->bmiHeader.biSizeImage)*newwidth/oldwidth);
			bmpnewheader.biXPelsPerMeter=(LONG)((lpbmi->bmiHeader.biSizeImage)*newheight/oldheight);
			bmpnewheader.biYPelsPerMeter=lpbmi->bmiHeader.biYPelsPerMeter;
			bmpnewheader.biClrUsed=0;
			bmpnewheader.biClrImportant=lpbmi->bmiHeader.biClrImportant;
			//写信息头到新图像中
			::memcpy(pnewDIB,&bmpnewheader,sizeof(BITMAPINFOHEADER));

			//复制整个图像调色板到新图像中
			::memcpy(pnewDIB+40,lpbi+40,1024);

			//把选择区域内图像数据写入新图像的DIB里
			for (j=0;j<newheight;j++)
				for (i=0;i<newfactwidth;i++)
					::memcpy(pnewDIB+1064+i+j*newfactwidth,&bmpnewbits[i][j],1);

			::GlobalUnlock((HGLOBAL)hnewDIB);
			::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
			//释放动态申请的内存
			for (i=0;i<oldfactwidth;i++)
			{
				delete bmpoldbits[i];
			}
			for (i=0;i<newfactwidth;i++)
			{
				delete bmpnewbits[i];
			}
			delete bmpoldbits;
			delete bmpnewbits;

			//建立选择区域的图像的句柄完毕
			//*************************************
   
			EmptyClipboard();
			SetClipboardData (CF_DIB, CopyHandle((HANDLE)hnewDIB));
			CloseClipboard();
			::GlobalFree((HGLOBAL) hnewDIB);
			EndWaitCursor();
		}
	}
	
}

void CMicroscopeView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(GetDocument()->GetHDIB() != NULL);
	
}

void CMicroscopeView::OnEditPaste() 
{
	HDIB hNewDIB = NULL;

	if (OpenClipboard())
	{
		BeginWaitCursor();

		hNewDIB = (HDIB) CopyHandle(::GetClipboardData(CF_DIB));

		CMicroscopeDoc* pDoc = GetDocument();

		CloseClipboard();

		if (hNewDIB != NULL)
		{
			pDoc->ReplaceHDIB(hNewDIB); // and free the old DIB
			pDoc->ReplaceFlag(FALSE);  //设置txt信息文件的标示为假
			pDoc->InitDIBData();    // set up new size & palette
			pDoc->SetModifiedFlag(TRUE);

			//加入的代码，用来判断此图像是否支持用鼠标画橡皮区矩形
			//*****************************************
			//判断此图像是否支持用鼠标画橡皮区矩形
			LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
			LPSTR lpbi;              // pointer to packed-DIB
			BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
			WORD wNumColors;         // number of colors in color table
			int i;

			lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hNewDIB);

			/* get pointer to BITMAPINFO (Win 3.0) */
			lpbmi = (LPBITMAPINFO)lpbi;

			pDoc->DrawRectTrackerflag=TRUE;  
			/* is this a Win 3.0 DIB? */
			bWinStyleDIB = IS_WIN30_DIB(lpbi);
			if (!bWinStyleDIB)
				{
					pDoc->DrawRectTrackerflag=FALSE; //如果不是Win3.0 DIB格式的图像，标志为假
				}
			else{
				/* get the number of colors in the DIB */
				wNumColors = ::DIBNumColors(lpbi);
				if (wNumColors!=256)
					{
//					pDoc->DrawRectTrackerflag=FALSE; //如果不是256色图像，标志为假
					}
				else{
					//检查图像的调色板是否符合要求，只要有一点不符合要求，就设DrawRectTrackerflag为假，退出循环
					for (i = 1; i < (int)wNumColors; i++)
						{
						if (lpbmi->bmiColors[i].rgbRed<lpbmi->bmiColors[i-1].rgbRed)
							{
								pDoc->DrawRectTrackerflag=FALSE;
								break;
							}
						if (lpbmi->bmiColors[i].rgbGreen<lpbmi->bmiColors[i-1].rgbGreen)
							{
								pDoc->DrawRectTrackerflag=FALSE;
								break;
							}
						if (lpbmi->bmiColors[i].rgbBlue<lpbmi->bmiColors[i-1].rgbBlue)
							{
								pDoc->DrawRectTrackerflag=FALSE;
								break;
							}
						}
					}
				}
			::GlobalUnlock((HGLOBAL) hNewDIB);
			//*****************************************
		
			ShowRectTrackerflag=FALSE;
			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
			OnDoRealize((WPARAM)m_hWnd,0);  // realize the new palette
			pDoc->UpdateAllViews(NULL);
		}
		EndWaitCursor();
	}
}

void CMicroscopeView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));
	
}

void CMicroscopeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());	
	OnDoRealize((WPARAM)m_hWnd,0);  // realize the new palette
	Invalidate();
}

void CMicroscopeView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMicroscopeDoc* pDoc=GetDocument();

	//如果不是本软件支持的图像格式就不能画橡皮区矩形
	if (pDoc->DrawRectTrackerflag==TRUE) 
	{    

		if (pDoc->RectTracker.TrackRubberBand(this, point,TRUE))
		{
		//ensure the rectangle is the right way up
		if (pDoc->RectTracker.m_rect.left > pDoc->RectTracker.m_rect.right)
			{
			int t = pDoc->RectTracker.m_rect.left;
			pDoc->RectTracker.m_rect.left = pDoc->RectTracker.m_rect.right;
			pDoc->RectTracker.m_rect.right = t;
			}

		//ensure the rectangle is the right way up
		if (pDoc->RectTracker.m_rect.top > pDoc->RectTracker.m_rect.bottom)
			{
			int t = pDoc->RectTracker.m_rect.top;
			pDoc->RectTracker.m_rect.top = pDoc->RectTracker.m_rect.bottom;
			pDoc->RectTracker.m_rect.bottom = t;
			}

		//clip the selection to the maximum size of the window
		CSize size(GetTotalSize());
		if (pDoc->RectTracker.m_rect.left < 0)
			pDoc->RectTracker.m_rect.left = 0;
		if (pDoc->RectTracker.m_rect.top < 0)
			pDoc->RectTracker.m_rect.top = 0;
		if (pDoc->RectTracker.m_rect.right >= size.cx)
			pDoc->RectTracker.m_rect.right = size.cx-1;
		if (pDoc->RectTracker.m_rect.bottom >= size.cy)
			pDoc->RectTracker.m_rect.bottom = size.cy-1;
		if(Keyflag==TRUE)
		  if ((pDoc->RectTracker.m_rect.right-pDoc->RectTracker.m_rect.left)!=(pDoc->RectTracker.m_rect.bottom-pDoc->RectTracker.m_rect.top))
             pDoc->RectTracker.m_rect.right=pDoc->RectTracker.m_rect.left+(pDoc->RectTracker.m_rect.bottom-pDoc->RectTracker.m_rect.top);


		//四个判断语句用来判断画的橡皮区矩形是否在图像上
		//如果一点都没有在图像上，则不显示画完后的矩形
		if (pDoc->RectTracker.m_rect.left >= size.cx)
			{
			ShowRectTrackerflag=FALSE;
			Invalidate(FALSE);
			return;
			}
		if (pDoc->RectTracker.m_rect.top >= size.cy)
			{
			ShowRectTrackerflag=FALSE;
			Invalidate(FALSE);
			return;
			}
		if (pDoc->RectTracker.m_rect.right < 0 )
			{
			ShowRectTrackerflag=FALSE;
			Invalidate(FALSE);
			return;
			}
		if (pDoc->RectTracker.m_rect.bottom < 0 )
			{
			ShowRectTrackerflag=FALSE;
			Invalidate(FALSE);
			return;
			}

		ShowRectTrackerflag=TRUE;
		Invalidate(FALSE);
		}
		else{
			ShowRectTrackerflag=FALSE;  //如果鼠标只是点击一下并没有移动则取消所画的橡皮区矩形
			Invalidate(FALSE);
			}
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void CMicroscopeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar==VK_SHIFT)
      Keyflag=TRUE;	

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMicroscopeView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar==VK_SHIFT)
      Keyflag=FALSE;	

	CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}



void CMicroscopeView::OnSize() 
{

	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
    //得到MDI程序活动子窗口指针
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
		{
		return; // no active MDI child frame 
		}

	//得到MDI程序活动文档指针
	CMicroscopeDoc* pDoc1=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc1->GetHDIB();

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
	
	//得到图像的高度和宽度
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;
	
	//计算DIB图像数据宽度实际使用的字节数
	factwidth=WIDTHBYTES(width*8);
	
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
	//由bmpbits数组计算得到其代表的电压值，并计算其中的平均值和最大值，最小值
	float zmaxtemp;  //计算Ry和Rz时储存Z方向电压最大值的临时变量
	float zmintemp;  //计算Ry和Rz时储存Z方向电压最小值的临时变量

///////////////////////////////////////////////////////////////////////// 2008.01.09


	// TODO: Add your command handler code here
	BeginWaitCursor();
	
	//捕获屏幕上所画的图像，分两步：
	//1.捕获屏幕上所画的图像为一个CBitmap对象，并得到此对象的BITMAP句柄
	//2.把HBITMAP转换成HDIB（DDB to DIB），从而进行显示和以后的保存。由于强制变成24bit的DIB图像，所以不需要加入调色板
	
	//以下是第一步，得到对话框上图像（即CBitmap对象）的HBITMAP句柄
	
	CDC memDC;  //内存设备上下文
	CBitmap bitmap; //捕获的BMP图
	
	CSize orgSize; //打开图象的大小；
	DWORD  dwWidth,dwHeight,dwOrgWidth,dwOrgHeight;
	CMicroscopeDoc* pDoc=GetDocument();
	orgSize=pDoc->GetDocSize();//获得原图象的大小（象素）
	dwOrgWidth=orgSize.cx;
	dwOrgHeight=orgSize.cy;


/////////////////////////////////////////////////////////////// 2008.01.09
	height=dwOrgHeight;
    width=dwOrgWidth;

	zmaxtemp=zmintemp=bmpbits[0][0];
	for (j=0;j<height;j++)
	{
		for (i=0;i<width;i++)
		{
		if (zmaxtemp<=bmpbits[i][j])
			{
			zmaxtemp=bmpbits[i][j];
			}
		if (zmintemp>=bmpbits[i][j])
			{
			zmintemp=bmpbits[i][j];
			}
		}
	}
    
  zmaxtemp=(float)((zmax-zmin)*(zmaxtemp*300./255.)); // 电压系数 20140505
  zmintemp=(float)((zmax-zmin)*(zmintemp*300./255.)); // 电压系数 20140505
  // AD值1V等于高压77V，恒力模式时，等于压电陶瓷伸缩77×6nm＝450nm
/////////////////////////////////////////////////////////////// 20140505

	dwWidth=orgSize.cx+0;
	dwHeight=orgSize.cy+0;//上下各加32个 pixels

	CRect	rect(0,0,dwWidth,dwHeight);  //原图的大小
//	CRect   newRect(0,0,rect.right+32,rect.bottom+32);//新图的大小
	CRect   newRect(0,0,rect.right+185,rect.bottom+64);//新图的大小 2008.01.09

	CDC* pDC=GetDC();

	//得到当前活动窗口（即本对话框窗口）的大小
	
	//this->GetActiveWindow()->GetClientRect(rect);
    
	//为对话框显示设备上下文创建兼容的内存设备上下文
    memDC.CreateCompatibleDC(pDC);
	//创建一个与对话框显示设备上下文兼容的位图 
    bitmap.CreateCompatibleBitmap(pDC, newRect.right,newRect.bottom);
    //把新位图选到内存设备上下文中，并返回被替代的原位图的指针
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	//把对话框显示设备描上下文拷贝到内存设备上下文中
	
	
	//
	//  revising....

	BOOL bZoom=FALSE;
	if (!(GetParent()->IsZoomed()))
	{
		GetParent()->ShowWindow(SW_SHOWMAXIMIZED);
		UpdateWindow();
		bZoom=TRUE;

	}

	
	memDC.FillSolidRect(0,0,rect.right+185,rect.bottom+64,RGB(220,220,220));
	//填充背景色为白色
	
    memDC.BitBlt(32,32,rect.right,rect.bottom , pDC, 0, 0, SRCCOPY); 



	if (bZoom)
	{
		GetParent()->ShowWindow(SW_RESTORE);
	    UpdateWindow();
	}


	//下面画尺寸线
	//

    // 关键数据说明：iBig--图像大小；dwXscan,dwYscan－－ 原始图像扫描尺寸；dwXreal,dwYreal－－预标注图像的实际扫描尺寸
	DWORD dwXscan=4000;//考虑到有可能重新剪切，所以要考虑，默认为 5000 纳米
	DWORD dwYscan=4000;//最原始图象x,y方向的扫描尺寸——总长
    DWORD dwXreal,dwYreal; //本图象的真实扫描尺寸
	int iBig;
	DWORD dwXTe,dwYTe;   // 文件打开过程中，使用的临时代表dwXscan,dwYscan 的变量，（因为文件有可能打开失败）
	
	//  首先尝试打开默认记录文件；
	//************打开扫描记录文件*************
	BOOL  bTxtOpen=TRUE; //
	CStdioFile filetxt;
	CString strBmpname,strTxtname;
	if(pDoc->m_nFromClip==12345)
	{
		
		CMicroscopeApp* app =(CMicroscopeApp*)AfxGetApp();
		strBmpname=	app->m_strPath;
	}
	else{
		strBmpname=pDoc->GetPathName();
	}
    strTxtname=strBmpname;
	// *******-------------------********************
	// 一下几行注释掉的文件可以 用于修改默认的文件名
	//  *******-------------------*******************
	//int bmpnameindex;  //bmp文件名在整个带路径的文件名字符串中的位置
	//bmpnameindex=strBmpname.GetLength()-4;
	//strTxtname=strBmpname.Left(bmpnameindex);
	//

	strTxtname=strTxtname+".TXT";
	
	if (!filetxt.Open(strTxtname,CFile::modeRead | CFile::shareExclusive | CFile::typeText))
	{
		bTxtOpen= FALSE;
	}
	else // 打开默认记录文件，获得原始数据 iBig--图像大小；dwXscan,dwYscan－－ 原始图像扫描尺寸；
		 //                                                 dwXreal,dwYreal－－预标注图像的实际扫描尺寸 
	{
		
		CString strBuf;
		TCHAR chBuf[10];
		int j;// iBig 扫描图像的大小
		
		
		for ( j=1;j<=5;j++)
			filetxt.ReadString(strBuf);
		strBuf.Replace("图像大小： ",NULL);
		for(j=0;j<=2;j++)
			chBuf[j]=strBuf[j];
		chBuf[j]='\0';
		iBig=atoi(chBuf);
		if( !((iBig==100)||(iBig==400)))
			bTxtOpen= FALSE;
		
		// now read the 扫描范围：
		chBuf[0]='\0';
		filetxt.ReadString(strBuf);
		filetxt.ReadString(strBuf);
		strBuf.Replace("扫描范围： X：",NULL);
		for(j=0;j<=5;j++)
		{
			if(strBuf[j]!=' ')
				chBuf[j]=strBuf[j];
			else break;					
		}
		chBuf[j]='\0';
		dwXTe=atoi(chBuf);
		strBuf.Replace(" nm ； Y：",NULL);
		strBuf.Replace(" nm",NULL);
		chBuf[0]='\0';
		strBuf=strBuf.Right(strBuf.GetLength()-j);
		for(j=0;j<strBuf.GetLength();j++)
			chBuf[j]=strBuf[j];
		chBuf[j]='\0';
		dwYTe=atoi(chBuf);
	}
	
	
	if (bTxtOpen)// open the txt file successfully!!
	{
        dwXscan=dwXTe;
		dwYscan=dwYTe;
	}
	else//打开默认记录文件失败!
	{
		// show the Mesgbox" search the record file by yourself or by dlg?"
		int iRe=AfxMessageBox("扫描记录文件没有找到，想亲自查找吗？",MB_YESNOCANCEL|MB_ICONQUESTION  ,(UINT)-1);
		switch (iRe)
		{
		case IDNO:
			{
				
				CSizeDlg dlg;
				if(IDOK==dlg.DoModal())
					
				{
					dwXscan=dlg.m_dwX;
					dwYscan=dlg.m_dwY;
					if(dlg.m_iSmall==1)
						iBig=400;
					if(dlg.m_iSmall==0)
						iBig=100;
				}
				else
					return;
				break;
			}
		case IDCANCEL:
			return;
			break;
		case IDYES:
			{
				CFileDialog dlg( true, "txt", "", OFN_EXPLORER | OFN_SHAREAWARE,"txt files |*.txt|" );
				if( dlg.DoModal() == IDOK )
				{
					CString strFileName;
					strFileName=dlg.GetPathName();
					/*if(!m_Capture.SaveToDib((LPTSTR)strFileName.GetBuffer(strFileName.GetLength()+1)))
					{	
					OnCcdStop();
					MessageBox("Can not capture bmp");
					}
					*/
					if (!filetxt.Open(strFileName,CFile::modeRead | CFile::shareExclusive | CFile::typeText))
					{
						bTxtOpen= FALSE;
						MessageBox("打开此文件失败!");
						return;
					}
					else
					{
						
						CString strBuf;
						TCHAR chBuf[10];
						int j;
						
						
						for ( j=1;j<=5;j++)
							filetxt.ReadString(strBuf);
						strBuf.Replace("图像大小： ",NULL);
						for(j=0;j<=2;j++)
							chBuf[j]=strBuf[j];
						chBuf[j]='\0';
						iBig=atoi(chBuf);
						if( !((iBig==100)||(iBig==400)))
							bTxtOpen= FALSE;
						
						// now read the 扫描范围：
						chBuf[0]='\0';
						filetxt.ReadString(strBuf);
						filetxt.ReadString(strBuf);
						strBuf.Replace("扫描范围： X：",NULL);
						for(j=0;j<=5;j++)
						{
							if(strBuf[j]!=' ')
								chBuf[j]=strBuf[j];
							else break;					
						}
						chBuf[j]='\0';
						dwXTe=atoi(chBuf);
						strBuf.Replace(" nm ； Y：",NULL);
						strBuf.Replace(" nm",NULL);
						chBuf[0]='\0';
						strBuf=strBuf.Right(strBuf.GetLength()-j);
						for(j=0;j<strBuf.GetLength();j++)
							chBuf[j]=strBuf[j];
						chBuf[j]='\0';
						dwYTe=atoi(chBuf);
						bTxtOpen= TRUE;
						dwXscan=dwXTe;
						dwYscan=dwYTe;
					}// end of file_open-else;
				} // end of ...	if( dlg.DoModal() == IDOK )
				else
					return;
				break;
			}// end of ...case IDYES:
		default:
			return;
		}// end of swtich;
	}//end of ...else//打开默认记录文件失败!

	//************-------*****************
	//  计算本图像实际的扫描范围；
	
	dwXreal=(DWORD)(dwOrgWidth*dwXscan/iBig);
	dwYreal=(DWORD)(dwOrgHeight*dwYscan/iBig);
	
	if(dwXreal<74)
	{
		if(IDNO==AfxMessageBox("图像X方向尺寸太小，可能无法标注，确实要继续吗？",MB_ICONQUESTION|MB_YESNO))
			return;
	}
	if(dwYreal<74)
	{
		if(IDNO==AfxMessageBox("图像Y方向尺寸太小，可能无法标注，确实要继续吗？",MB_ICONQUESTION|MB_YESNO))
				return;
	}
			
	CFont font;
	LOGFONT lf;

	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 14;// request a 14-pixel-height font
	lf.lfWeight=FW_MEDIUM;
	strcpy(lf.lfFaceName, "System");        // request a face name "Arial"
	VERIFY(font.CreateFontIndirect(&lf));  // create the font
	
	// Do something with the font just created...
	CFont* def_font = memDC.SelectObject(&font);
	
	CPen BlackPen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen* pOldPen = memDC.SelectObject(&BlackPen);
	

	// draw X direction;
	
	memDC.MoveTo(32,rect.Height()+32); //2008
	memDC.LineTo(rect.Width()+32,rect.Height()+32);
	
	short sTemp=0;
	
	for (sTemp=0;sTemp<5;sTemp++)
	{
		memDC.MoveTo((((dwWidth-0)*sTemp)/4+32),rect.Height()+32);
		memDC.LineTo((((dwWidth-0)*sTemp)/4+32),rect.Height()+32+7);
	}   //end of for...
	

	// draw Y direction;
	memDC.MoveTo(rect.Width()+32,32); //2008
	memDC.LineTo(rect.Width()+32,dwHeight+32);
	for (sTemp=0;sTemp<5;sTemp++)
	{
		memDC.MoveTo(rect.Width()+32,((dwHeight-0)*sTemp/4+32));
		memDC.LineTo((rect.Width()+32+7),((dwHeight-0)*sTemp/4+32));
	}   //end of for...
	
	
	DWORD dwEnd,dwMid;
	// now  textout the x -axis scale;
	
	if (dwXreal<100)
	{
		if ((dwXreal/10.0-dwXreal/10)>=0.5)
			dwEnd=(dwXreal/10+1)*10;
		else 
			dwEnd=(dwXreal/10)*10;
		dwMid=dwEnd/2;
	}
	else
	{
		if ((dwXreal/50.0-dwXreal/50)>=0.5)
			dwEnd=(dwXreal/50+1)*50;
		else 
			dwEnd=(dwXreal/50)*50;
		dwMid=dwEnd/2;
	}

	CString str;
	memDC.TextOut(29,(dwHeight+37),"0");
	str.Format("%d",dwMid);
	memDC.TextOut((dwWidth)/2+29-14,(dwHeight+37),str);
	str.Format("%d nm",dwEnd);
	memDC.TextOut(dwWidth+29-14,(dwHeight+37),str);
	str.Format("%d",dwMid/2);
	memDC.TextOut((dwWidth)/4+29-14,(dwHeight+37),str);
	str.Format("%d",dwEnd*3/4);
	memDC.TextOut((dwWidth)*3/4+29-14,(dwHeight+37),str);

	
	// Now textout the Y-axis scale;
	
	if (dwYreal<100)
	{
		
		if ((dwYreal/10.0-dwYreal/10)>=0.5)
			dwEnd=(dwYreal/10+1)*10;
		else 
			dwEnd=(dwYreal/10)*10;
		dwMid=dwEnd/2;
	}
	else
	{
		if ((dwYreal/50.0-dwYreal/50)>=0.5)
			dwEnd=(dwYreal/50+1)*50;
		else 
			dwEnd=(dwYreal/50)*50;
		dwMid=dwEnd/2;
	}
	
	memDC.TextOut((dwWidth+32+18),(dwHeight+32-12),"0");
	
	str.Format("%d",dwMid);
	memDC.TextOut(dwWidth+32+9,(dwHeight)/2+32-8,str);
	
	str.Format("%d",dwEnd);
	memDC.TextOut(dwWidth+32+9,32-8,str);
	
	str.Format("%d",dwMid/2);
	memDC.TextOut((dwWidth+32+9),((dwHeight)*3/4+32-8),str);

	str.Format("%d",dwEnd*3/4);
	memDC.TextOut((dwWidth+32+9),((dwHeight)/4+32-8),str);


/////////////////////////////////////////////////////////////////////// 2008.01.09
// 画Z向尺寸灰度条	

	short sTemp1=0;
	short sTemp2=0;
    float zmaxNew;

	for (sTemp1=0;sTemp1<64;sTemp1++)
	{
	sTemp2=(int)(255-sTemp1*2);	 //2008.01.09
//	CPen BlackPen(PS_SOLID, 1, RGB(sTemp2, sTemp2*0.8, sTemp2*0.2));   //2008.01.09
	CPen BlackPen(PS_SOLID, 1, RGB(255,sTemp2,2*(sTemp2-128)+1));   //2008.01.09
   	CPen* pOldPen = memDC.SelectObject(&BlackPen);
    memDC.MoveTo(rect.right+80,rect.bottom/2-32+sTemp1);    //灰度条显示在右侧中部
	memDC.LineTo(rect.right+80+30,rect.bottom/2-32+sTemp1); 
//  memDC.MoveTo(rect.right+80,rect.bottom+32-127+sTemp1);  //灰度条显示在右侧底部
//	memDC.LineTo(rect.right+80+30,rect.bottom+32-127+sTemp1);
	}  

	for (sTemp1=64;sTemp1<128;sTemp1++)
	{
	sTemp2=(int)(255-sTemp1*2);	 //2008.01.09
	CPen BlackPen(PS_SOLID, 1, RGB(2*sTemp2,sTemp2,0));   //2008.01.09
   	CPen* pOldPen = memDC.SelectObject(&BlackPen);
    memDC.MoveTo(rect.right+80,rect.bottom/2-32+sTemp1);    //灰度条显示在右侧中部
	memDC.LineTo(rect.right+80+30,rect.bottom/2-32+sTemp1); 
	}  


//  画Z向尺寸灰度条的框	
    memDC.MoveTo(rect.right+80,rect.bottom/2-32);      //框顶端，框底部即为灰度条最暗线条
	memDC.LineTo(rect.right+80+30,rect.bottom/2-32); 

    memDC.MoveTo(rect.right+80+30,rect.bottom/2-32);      //框右侧
	memDC.LineTo(rect.right+80+30,rect.bottom/2-32+128); 
    
	memDC.MoveTo(rect.right+80,rect.bottom/2-32);     //框左侧
	memDC.LineTo(rect.right+80,rect.bottom/2-32+128); 
//  画Z向尺寸灰度条的框完毕	

/////////////////////////////////////////////////////////////////////// 2008.01.09

	zmaxNew=zmaxtemp-zmintemp;
//	str.Format("%.1f nm",zmaxNew);
	str.Format("%.1f ",zmaxNew);
//	memDC.TextOut(rect.right+75,rect.bottom/2-50,str); //灰度条显示在右侧中部 c
	memDC.TextOut(rect.right+112,rect.bottom/2-35,str); //灰度条显示在右侧中部
//  memDC.TextOut(rect.right+112,rect.bottom+30-127,str); //灰度条显示在右侧底部

//	memDC.TextOut(rect.right+112,rect.bottom/2-22,"  nm"); //灰度条显示在右侧中部

	str.Format("%d",0);
	memDC.TextOut(rect.right+112,rect.bottom/2-45+128," 0 nm"); //灰度条显示在右侧中部
//	memDC.TextOut(rect.right+75,rect.bottom/2-45+128 + 15, str); //灰度条显示在右侧中部 c
//	memDC.TextOut(rect.right+116,rect.bottom+20,"0.0"); //灰度条显示在右侧底部

////////////////////////////////////////////////////////////////////// 2008.01.09


	memDC.SelectObject(pOldPen);
	memDC.SelectObject(def_font);
	
	//把被替代的位图重新选入内存设备上下文，即：恢复初始状态
	memDC.SelectObject(pOldBitmap);
	if (pDC!=NULL)
		ReleaseDC(pDC);
	//由CBitmap对象得到其HBITMAP句柄
	HBITMAP hBitmap=(HBITMAP)bitmap.GetSafeHandle();
	
	//以下是第二步，由HBITMAP句柄转换成HDIB句柄
	
	BITMAP bm;               // bitmap structure
	BITMAPINFOHEADER bi;     // bitmap header
	LPBITMAPINFOHEADER lpbi1; // pointer to BITMAPINFOHEADER
	DWORD dwLen;             // size of memory block
	HDIB hDib, h;            // handle to DIB, temp handle
	HDC hDC;                 // handle to DC
	WORD biBits;             // bits per pixel
	UINT wLineLen;           //CBITMAP对象所表示的图像（DDB）的实际宽度的字节数（宽度要是32bit的倍数）
	DWORD dwSize;             //CBITMAP对象所表示的图像（DDB）的大小
	DWORD wColSize;          //调色板所占的字节数
	
	// check if bitmap handle is valid
	if (!hBitmap)
	{
		return ;
	}
	
	// fill in BITMAP structure, return NULL if it didn't work
	if (!::GetObject(hBitmap, sizeof(bm), &bm))
	{
		return ;
	}
	
	// calculate bits per pixel
	biBits = (WORD) (bm.bmPlanes * bm.bmBitsPixel);
	
	wLineLen = ( bm.bmWidth * biBits + 31 ) / 32 * 4;
	wColSize = sizeof(RGBQUAD) * (( biBits <= 8 ) ? 
		1 << biBits : 0 );
	dwSize = sizeof( BITMAPINFOHEADER ) + wColSize +
		(DWORD)(UINT)wLineLen * (DWORD)(UINT)bm.bmHeight;
	
	// make sure bits per pixel is valid
	if (biBits <= 1)
		biBits = 1;
	else if (biBits <= 4)
		biBits = 4;
	else if (biBits <= 8)
		biBits = 8;
	else // if greater than 8-bit, force to 24-bit
 		biBits = 24;
	
	// initialize BITMAPINFOHEADER
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = biBits;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = dwSize - sizeof(BITMAPINFOHEADER) - wColSize;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = ( biBits <= 8 ) ? 1 << biBits : 0;	
	bi.biClrImportant = 0;
	
	// calculate size of memory block required to store BITMAPINFO
	dwLen = bi.biSize + ::PaletteSize((LPSTR) &bi);
	
	// get a DC
	hDC = ::GetDC(NULL);
	
	// alloc memory block to store our bitmap
	hDib = (HDIB)::GlobalAlloc(GHND, dwLen);
	
	// if we couldn't get memory block
	if (!hDib)
	{
		// clean up and return NULL
		::ReleaseDC(NULL, hDC);
		
		return ;
	}
	
	// lock memory and get pointer to it
	lpbi1 = (LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)hDib);
	if (!lpbi)
	{
		// clean up and return NULL
		::ReleaseDC(NULL, hDC);
		
		return ;
	}
	
	// use our bitmap info. to fill BITMAPINFOHEADER
	*lpbi1 = bi;
	
	// call GetDIBits with a NULL lpBits param, so it will 
	// calculate the biSizeImage field for us
	::GetDIBits(hDC, hBitmap, 0, (WORD)bi.biHeight, NULL, 
		(LPBITMAPINFO)lpbi1, DIB_RGB_COLORS);
	
	// get the info. returned by GetDIBits and unlock 
	// memory block
	bi = *lpbi1;
	bi.biClrUsed = ( biBits <= 8 ) ? 1 << biBits : 0;
	::GlobalUnlock(hDib);
	
	// if the driver did not fill in the biSizeImage field, 
	// make one up
	if (bi.biSizeImage == 0)
		bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight;
	
	
	// realloc the buffer big enough to hold all the bits
	dwLen = bi.biSize + ::PaletteSize((LPSTR) &bi) + 
		bi.biSizeImage;
	h = (HDIB)::GlobalReAlloc(hDib, dwLen, 0);
	if ( h )
	{
		hDib = h;
	}
	else
	{
		// clean up and return NULL
		::GlobalFree(hDib);
		hDib = NULL;
		
		::ReleaseDC(NULL, hDC);
		
		return ;
	}
	
	// lock memory block and get pointer to it
	lpbi1 = (LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)hDib);
	if (!lpbi)
	{
		// clean up and return NULL
		::GlobalFree(hDib);
		hDib = NULL;
		
		::ReleaseDC(NULL, hDC);
		
		return ;
	}
	
	// call GetDIBits with a NON-NULL lpBits param, and 
	// actualy get the bits this time
	if (::GetDIBits(hDC, hBitmap, 0, (WORD)bi.biHeight, 
		(LPSTR)lpbi1 + (WORD)lpbi1->biSize + 
		::PaletteSize((LPSTR) lpbi1), (LPBITMAPINFO)lpbi1, 
		DIB_RGB_COLORS) == 0)
	{
		// clean up and return NULL
		::GlobalUnlock(hDib);
		hDib = NULL;
		
		::ReleaseDC(NULL, hDC);
		
		return ;
	}
	
	bi = *lpbi1;
	
	// clean up
	::GlobalUnlock(hDib);
	::ReleaseDC(NULL, hDC);
	
	
	//打开一个新文档，显示得到的DIB图像
	CMicroscopeApp* app =(CMicroscopeApp*)AfxGetApp();
	
	//开启新的文档
	POSITION posTemplate = app->GetFirstDocTemplatePosition();
	CDocTemplate* pDocTemplate = app->GetNextDocTemplate(posTemplate);
	CMicroscopeDoc* pDocument = (CMicroscopeDoc*) pDocTemplate->OpenDocumentFile(NULL);
	
	//传递HDIB，并且更新视图
	pDocument->ReplaceHDIB(hDib);
	pDocument->InitDIBData();    // set up new size & palette
	pDocument->ReplaceFlag(FALSE); //不写txt信息文件
	pDocument->DrawRectTrackerflag=FALSE;	//画橡皮区矩形标志为假
	
	pDocument->SetModifiedFlag(TRUE);
	pDocument->UpdateAllViews(NULL);	
	
	EndWaitCursor();
	
	return;
		
}

LRESULT CMicroscopeView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{

	LONG width;               //图像宽度 2008.01.15
	LONG height;              //图像高度 2008.01.15
	
	if (message == WM_AAAA)
	{
		CDC* pDC=GetDC();
		CString tmp;
		LPBITMAPINFO lpbmi;
		LPSTR lpbi;
		CMicroscopeDoc* pDoc=GetDocument();
		HDIB hDIB = pDoc->GetHDIB();
		lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		lpbmi=(LPBITMAPINFO)lpbi;

	//得到图像的高度和宽度
        width=lpbmi->bmiHeader.biWidth; // 2008.01.15
	    height=lpbmi->bmiHeader.biHeight; // 2008.01.15

////////////////////////////////////////////////////////////////// 2008.01.15				
//		tmp.Format("X: %.4d",(wParam+1)*lpbmi->bmiHeader.biXPelsPerMeter/400);
		tmp.Format("X: %.4d",(wParam+1)*lpbmi->bmiHeader.biXPelsPerMeter/width);
		pDC->TextOut(340,370,tmp);
//		tmp.Format("Y: %.4d",(lParam+1)*lpbmi->bmiHeader.biXPelsPerMeter/400);
		tmp.Format("Y: %.4d",(lParam+1)*lpbmi->bmiHeader.biXPelsPerMeter/height);
		pDC->TextOut(340,385,tmp);
		UpdateData(FALSE);
////////////////////////////////////////////////////////////////// 2008.01.15				

	}
	return CScrollView::DefWindowProc(message, wParam, lParam);
}
