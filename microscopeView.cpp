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

	//����������������ΪShiftѡ��������������� //


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
	//��ʾ��������Ƥ������
	LONG width;               //ͼ���� 2008.01.15
	LONG height;              //ͼ��߶� 2008.01.15



if ((pDoc->DrawRectTrackerflag&&ShowRectTrackerflag)==TRUE)
{
	LPBITMAPINFO lpbmi;
	LPSTR lpbi;
	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	lpbmi=(LPBITMAPINFO)lpbi;
	CString tmp;

    //�õ�ͼ��ĸ߶ȺͿ�� 
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
		//���������1.û��ѡ����Ƥ�����Σ�2.��ѡ����Ƥ������
		//1.û��ѡ����Ƥ������
		if (ShowRectTrackerflag==FALSE)
		{
			BeginWaitCursor();
			EmptyClipboard();
			SetClipboardData (CF_DIB, CopyHandle((HANDLE) pDoc->GetHDIB()) );
			CloseClipboard();
			EndWaitCursor();
		}
		//2.��ѡ����Ƥ������
		if (ShowRectTrackerflag==TRUE)
		{
			BeginWaitCursor();
          
		    //*************************************
			//����ѡ�������ͼ��ľ��

			LPSTR lpbi;              // pointer to packed-DIB ����ͼ��
			LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0) ����ͼ��
			HDIB  hnewDIB;           //ѡ�������ڵ�DIBͼ����
			BITMAPINFOHEADER bmpnewheader; //ѡ�������ڵ�DIBͼ�����Ϣͷ
			LPSTR pnewDIB;           //ѡ�������ڵ�DIBͼ���ָ��
			LONG oldwidth;               //����ͼ����
			LONG oldheight;              //����ͼ��߶�
			LONG oldfactwidth;           //����ͼ����ʵ��ʹ�õ��ֽ��� DIBͼ��Ŀ��Ҫ��4byte�ı���
			LONG newwidth;               //ѡ��������ͼ����
			LONG newheight;              //ѡ��������ͼ��߶�
			LONG newfactwidth;           //ѡ��������ͼ����ʵ��ʹ�õ��ֽ��� DIBͼ��Ŀ��Ҫ��4byte�ı���
			int i, j;

			lpbi = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHDIB());
 
			/* get pointer to BITMAPINFO (Win 3.0) */
			lpbmi = (LPBITMAPINFO)lpbi;

			//�õ�����ͼ��ĸ߶ȺͿ��
			oldwidth=lpbmi->bmiHeader.biWidth;
			oldheight=lpbmi->bmiHeader.biHeight;

			//��������DIBͼ�����ݿ��ʵ��ʹ�õ��ֽ���
			oldfactwidth=WIDTHBYTES(oldwidth*8);

			//����ѡ��������ͼ���Ⱥ͸߶�
			newwidth=pDoc->RectTracker.m_rect.right - pDoc->RectTracker.m_rect.left + 1;
			newheight=pDoc->RectTracker.m_rect.bottom - pDoc->RectTracker.m_rect.top + 1;

			//����ѡ��������ͼ����ʵ��ʹ�õ��ֽ���
			newfactwidth=WIDTHBYTES(newwidth*8);

			//����DIBͼ����ڴ�ռ�
			hnewDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, newfactwidth*newheight+1064);
			//�õ���ָ��
			pnewDIB = (LPSTR) ::GlobalLock((HGLOBAL) hnewDIB);

			//��̬���������洢����ͼ�����ݺ�ѡ��������ͼ�����ݵ�����
			BYTE **bmpoldbits;      //����ͼ������
			bmpoldbits=new BYTE*[oldfactwidth];
			for (i=0;i<oldfactwidth;i++)
				bmpoldbits[i]=new BYTE[oldheight];

			BYTE **bmpnewbits;     //ѡ��������ͼ������
			bmpnewbits=new BYTE*[newfactwidth];
			for (i=0;i<newfactwidth;i++)
				bmpnewbits[i]=new BYTE[newheight];

		   	//��ȡԭ����DIBͼ���dib����
			//��������ͼ���dib���ݵ�����ͼ������
			for (j=0;j<oldheight;j++)
				for (i=0;i<oldfactwidth;i++)
					::memcpy(&bmpoldbits[i][j],lpbi+1064+i+j*oldfactwidth,1); 

			//����ѡ�е�����ѡ��������ͼ������
			for (j=0;j<newheight;j++)
				for (i=0;i<newwidth;i++)
					bmpnewbits[i][j]=bmpoldbits[i+pDoc->RectTracker.m_rect.left][j+oldheight-1-pDoc->RectTracker.m_rect.bottom];

			//дѡ��������ͼ�����Ϣͷ
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
			//д��Ϣͷ����ͼ����
			::memcpy(pnewDIB,&bmpnewheader,sizeof(BITMAPINFOHEADER));

			//��������ͼ���ɫ�嵽��ͼ����
			::memcpy(pnewDIB+40,lpbi+40,1024);

			//��ѡ��������ͼ������д����ͼ���DIB��
			for (j=0;j<newheight;j++)
				for (i=0;i<newfactwidth;i++)
					::memcpy(pnewDIB+1064+i+j*newfactwidth,&bmpnewbits[i][j],1);

			::GlobalUnlock((HGLOBAL)hnewDIB);
			::GlobalUnlock((HGLOBAL)pDoc->GetHDIB());
			//�ͷŶ�̬������ڴ�
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

			//����ѡ�������ͼ��ľ�����
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
			pDoc->ReplaceFlag(FALSE);  //����txt��Ϣ�ļ��ı�ʾΪ��
			pDoc->InitDIBData();    // set up new size & palette
			pDoc->SetModifiedFlag(TRUE);

			//����Ĵ��룬�����жϴ�ͼ���Ƿ�֧������껭��Ƥ������
			//*****************************************
			//�жϴ�ͼ���Ƿ�֧������껭��Ƥ������
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
					pDoc->DrawRectTrackerflag=FALSE; //�������Win3.0 DIB��ʽ��ͼ�񣬱�־Ϊ��
				}
			else{
				/* get the number of colors in the DIB */
				wNumColors = ::DIBNumColors(lpbi);
				if (wNumColors!=256)
					{
//					pDoc->DrawRectTrackerflag=FALSE; //�������256ɫͼ�񣬱�־Ϊ��
					}
				else{
					//���ͼ��ĵ�ɫ���Ƿ����Ҫ��ֻҪ��һ�㲻����Ҫ�󣬾���DrawRectTrackerflagΪ�٣��˳�ѭ��
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

	//������Ǳ����֧�ֵ�ͼ���ʽ�Ͳ��ܻ���Ƥ������
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


		//�ĸ��ж���������жϻ�����Ƥ�������Ƿ���ͼ����
		//���һ�㶼û����ͼ���ϣ�����ʾ�����ľ���
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
			ShowRectTrackerflag=FALSE;  //������ֻ�ǵ��һ�²�û���ƶ���ȡ����������Ƥ������
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
    //�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
		{
		return; // no active MDI child frame 
		}

	//�õ�MDI�����ĵ�ָ��
	CMicroscopeDoc* pDoc1=(CMicroscopeDoc*)pChild->GetActiveDocument();
	HDIB hDIB=pDoc1->GetHDIB();

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
	
	//�õ�ͼ��ĸ߶ȺͿ��
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;
	
	//����DIBͼ�����ݿ��ʵ��ʹ�õ��ֽ���
	factwidth=WIDTHBYTES(width*8);
	
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
	//��bmpbits�������õ������ĵ�ѹֵ�����������е�ƽ��ֵ�����ֵ����Сֵ
	float zmaxtemp;  //����Ry��Rzʱ����Z�����ѹ���ֵ����ʱ����
	float zmintemp;  //����Ry��Rzʱ����Z�����ѹ��Сֵ����ʱ����

///////////////////////////////////////////////////////////////////////// 2008.01.09


	// TODO: Add your command handler code here
	BeginWaitCursor();
	
	//������Ļ��������ͼ�񣬷�������
	//1.������Ļ��������ͼ��Ϊһ��CBitmap���󣬲��õ��˶����BITMAP���
	//2.��HBITMAPת����HDIB��DDB to DIB�����Ӷ�������ʾ���Ժ�ı��档����ǿ�Ʊ��24bit��DIBͼ�����Բ���Ҫ�����ɫ��
	
	//�����ǵ�һ�����õ��Ի�����ͼ�񣨼�CBitmap���󣩵�HBITMAP���
	
	CDC memDC;  //�ڴ��豸������
	CBitmap bitmap; //�����BMPͼ
	
	CSize orgSize; //��ͼ��Ĵ�С��
	DWORD  dwWidth,dwHeight,dwOrgWidth,dwOrgHeight;
	CMicroscopeDoc* pDoc=GetDocument();
	orgSize=pDoc->GetDocSize();//���ԭͼ��Ĵ�С�����أ�
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
    
  zmaxtemp=(float)((zmax-zmin)*(zmaxtemp*300./255.)); // ��ѹϵ�� 20140505
  zmintemp=(float)((zmax-zmin)*(zmintemp*300./255.)); // ��ѹϵ�� 20140505
  // ADֵ1V���ڸ�ѹ77V������ģʽʱ������ѹ���մ�����77��6nm��450nm
/////////////////////////////////////////////////////////////// 20140505

	dwWidth=orgSize.cx+0;
	dwHeight=orgSize.cy+0;//���¸���32�� pixels

	CRect	rect(0,0,dwWidth,dwHeight);  //ԭͼ�Ĵ�С
//	CRect   newRect(0,0,rect.right+32,rect.bottom+32);//��ͼ�Ĵ�С
	CRect   newRect(0,0,rect.right+185,rect.bottom+64);//��ͼ�Ĵ�С 2008.01.09

	CDC* pDC=GetDC();

	//�õ���ǰ����ڣ������Ի��򴰿ڣ��Ĵ�С
	
	//this->GetActiveWindow()->GetClientRect(rect);
    
	//Ϊ�Ի�����ʾ�豸�����Ĵ������ݵ��ڴ��豸������
    memDC.CreateCompatibleDC(pDC);
	//����һ����Ի�����ʾ�豸�����ļ��ݵ�λͼ 
    bitmap.CreateCompatibleBitmap(pDC, newRect.right,newRect.bottom);
    //����λͼѡ���ڴ��豸�������У������ر������ԭλͼ��ָ��
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	//�ѶԻ�����ʾ�豸�������Ŀ������ڴ��豸��������
	
	
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
	//��䱳��ɫΪ��ɫ
	
    memDC.BitBlt(32,32,rect.right,rect.bottom , pDC, 0, 0, SRCCOPY); 



	if (bZoom)
	{
		GetParent()->ShowWindow(SW_RESTORE);
	    UpdateWindow();
	}


	//���滭�ߴ���
	//

    // �ؼ�����˵����iBig--ͼ���С��dwXscan,dwYscan���� ԭʼͼ��ɨ��ߴ磻dwXreal,dwYreal����Ԥ��עͼ���ʵ��ɨ��ߴ�
	DWORD dwXscan=4000;//���ǵ��п������¼��У�����Ҫ���ǣ�Ĭ��Ϊ 5000 ����
	DWORD dwYscan=4000;//��ԭʼͼ��x,y�����ɨ��ߴ硪���ܳ�
    DWORD dwXreal,dwYreal; //��ͼ�����ʵɨ��ߴ�
	int iBig;
	DWORD dwXTe,dwYTe;   // �ļ��򿪹����У�ʹ�õ���ʱ����dwXscan,dwYscan �ı���������Ϊ�ļ��п��ܴ�ʧ�ܣ�
	
	//  ���ȳ��Դ�Ĭ�ϼ�¼�ļ���
	//************��ɨ���¼�ļ�*************
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
	// һ�¼���ע�͵����ļ����� �����޸�Ĭ�ϵ��ļ���
	//  *******-------------------*******************
	//int bmpnameindex;  //bmp�ļ�����������·�����ļ����ַ����е�λ��
	//bmpnameindex=strBmpname.GetLength()-4;
	//strTxtname=strBmpname.Left(bmpnameindex);
	//

	strTxtname=strTxtname+".TXT";
	
	if (!filetxt.Open(strTxtname,CFile::modeRead | CFile::shareExclusive | CFile::typeText))
	{
		bTxtOpen= FALSE;
	}
	else // ��Ĭ�ϼ�¼�ļ������ԭʼ���� iBig--ͼ���С��dwXscan,dwYscan���� ԭʼͼ��ɨ��ߴ磻
		 //                                                 dwXreal,dwYreal����Ԥ��עͼ���ʵ��ɨ��ߴ� 
	{
		
		CString strBuf;
		TCHAR chBuf[10];
		int j;// iBig ɨ��ͼ��Ĵ�С
		
		
		for ( j=1;j<=5;j++)
			filetxt.ReadString(strBuf);
		strBuf.Replace("ͼ���С�� ",NULL);
		for(j=0;j<=2;j++)
			chBuf[j]=strBuf[j];
		chBuf[j]='\0';
		iBig=atoi(chBuf);
		if( !((iBig==100)||(iBig==400)))
			bTxtOpen= FALSE;
		
		// now read the ɨ�跶Χ��
		chBuf[0]='\0';
		filetxt.ReadString(strBuf);
		filetxt.ReadString(strBuf);
		strBuf.Replace("ɨ�跶Χ�� X��",NULL);
		for(j=0;j<=5;j++)
		{
			if(strBuf[j]!=' ')
				chBuf[j]=strBuf[j];
			else break;					
		}
		chBuf[j]='\0';
		dwXTe=atoi(chBuf);
		strBuf.Replace(" nm �� Y��",NULL);
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
	else//��Ĭ�ϼ�¼�ļ�ʧ��!
	{
		// show the Mesgbox" search the record file by yourself or by dlg?"
		int iRe=AfxMessageBox("ɨ���¼�ļ�û���ҵ��������Բ�����",MB_YESNOCANCEL|MB_ICONQUESTION  ,(UINT)-1);
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
						MessageBox("�򿪴��ļ�ʧ��!");
						return;
					}
					else
					{
						
						CString strBuf;
						TCHAR chBuf[10];
						int j;
						
						
						for ( j=1;j<=5;j++)
							filetxt.ReadString(strBuf);
						strBuf.Replace("ͼ���С�� ",NULL);
						for(j=0;j<=2;j++)
							chBuf[j]=strBuf[j];
						chBuf[j]='\0';
						iBig=atoi(chBuf);
						if( !((iBig==100)||(iBig==400)))
							bTxtOpen= FALSE;
						
						// now read the ɨ�跶Χ��
						chBuf[0]='\0';
						filetxt.ReadString(strBuf);
						filetxt.ReadString(strBuf);
						strBuf.Replace("ɨ�跶Χ�� X��",NULL);
						for(j=0;j<=5;j++)
						{
							if(strBuf[j]!=' ')
								chBuf[j]=strBuf[j];
							else break;					
						}
						chBuf[j]='\0';
						dwXTe=atoi(chBuf);
						strBuf.Replace(" nm �� Y��",NULL);
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
	}//end of ...else//��Ĭ�ϼ�¼�ļ�ʧ��!

	//************-------*****************
	//  ���㱾ͼ��ʵ�ʵ�ɨ�跶Χ��
	
	dwXreal=(DWORD)(dwOrgWidth*dwXscan/iBig);
	dwYreal=(DWORD)(dwOrgHeight*dwYscan/iBig);
	
	if(dwXreal<74)
	{
		if(IDNO==AfxMessageBox("ͼ��X����ߴ�̫С�������޷���ע��ȷʵҪ������",MB_ICONQUESTION|MB_YESNO))
			return;
	}
	if(dwYreal<74)
	{
		if(IDNO==AfxMessageBox("ͼ��Y����ߴ�̫С�������޷���ע��ȷʵҪ������",MB_ICONQUESTION|MB_YESNO))
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
// ��Z��ߴ�Ҷ���	

	short sTemp1=0;
	short sTemp2=0;
    float zmaxNew;

	for (sTemp1=0;sTemp1<64;sTemp1++)
	{
	sTemp2=(int)(255-sTemp1*2);	 //2008.01.09
//	CPen BlackPen(PS_SOLID, 1, RGB(sTemp2, sTemp2*0.8, sTemp2*0.2));   //2008.01.09
	CPen BlackPen(PS_SOLID, 1, RGB(255,sTemp2,2*(sTemp2-128)+1));   //2008.01.09
   	CPen* pOldPen = memDC.SelectObject(&BlackPen);
    memDC.MoveTo(rect.right+80,rect.bottom/2-32+sTemp1);    //�Ҷ�����ʾ���Ҳ��в�
	memDC.LineTo(rect.right+80+30,rect.bottom/2-32+sTemp1); 
//  memDC.MoveTo(rect.right+80,rect.bottom+32-127+sTemp1);  //�Ҷ�����ʾ���Ҳ�ײ�
//	memDC.LineTo(rect.right+80+30,rect.bottom+32-127+sTemp1);
	}  

	for (sTemp1=64;sTemp1<128;sTemp1++)
	{
	sTemp2=(int)(255-sTemp1*2);	 //2008.01.09
	CPen BlackPen(PS_SOLID, 1, RGB(2*sTemp2,sTemp2,0));   //2008.01.09
   	CPen* pOldPen = memDC.SelectObject(&BlackPen);
    memDC.MoveTo(rect.right+80,rect.bottom/2-32+sTemp1);    //�Ҷ�����ʾ���Ҳ��в�
	memDC.LineTo(rect.right+80+30,rect.bottom/2-32+sTemp1); 
	}  


//  ��Z��ߴ�Ҷ����Ŀ�	
    memDC.MoveTo(rect.right+80,rect.bottom/2-32);      //�򶥶ˣ���ײ���Ϊ�Ҷ��������
	memDC.LineTo(rect.right+80+30,rect.bottom/2-32); 

    memDC.MoveTo(rect.right+80+30,rect.bottom/2-32);      //���Ҳ�
	memDC.LineTo(rect.right+80+30,rect.bottom/2-32+128); 
    
	memDC.MoveTo(rect.right+80,rect.bottom/2-32);     //�����
	memDC.LineTo(rect.right+80,rect.bottom/2-32+128); 
//  ��Z��ߴ�Ҷ����Ŀ����	

/////////////////////////////////////////////////////////////////////// 2008.01.09

	zmaxNew=zmaxtemp-zmintemp;
//	str.Format("%.1f nm",zmaxNew);
	str.Format("%.1f ",zmaxNew);
//	memDC.TextOut(rect.right+75,rect.bottom/2-50,str); //�Ҷ�����ʾ���Ҳ��в� c
	memDC.TextOut(rect.right+112,rect.bottom/2-35,str); //�Ҷ�����ʾ���Ҳ��в�
//  memDC.TextOut(rect.right+112,rect.bottom+30-127,str); //�Ҷ�����ʾ���Ҳ�ײ�

//	memDC.TextOut(rect.right+112,rect.bottom/2-22,"  nm"); //�Ҷ�����ʾ���Ҳ��в�

	str.Format("%d",0);
	memDC.TextOut(rect.right+112,rect.bottom/2-45+128," 0 nm"); //�Ҷ�����ʾ���Ҳ��в�
//	memDC.TextOut(rect.right+75,rect.bottom/2-45+128 + 15, str); //�Ҷ�����ʾ���Ҳ��в� c
//	memDC.TextOut(rect.right+116,rect.bottom+20,"0.0"); //�Ҷ�����ʾ���Ҳ�ײ�

////////////////////////////////////////////////////////////////////// 2008.01.09


	memDC.SelectObject(pOldPen);
	memDC.SelectObject(def_font);
	
	//�ѱ������λͼ����ѡ���ڴ��豸�����ģ������ָ���ʼ״̬
	memDC.SelectObject(pOldBitmap);
	if (pDC!=NULL)
		ReleaseDC(pDC);
	//��CBitmap����õ���HBITMAP���
	HBITMAP hBitmap=(HBITMAP)bitmap.GetSafeHandle();
	
	//�����ǵڶ�������HBITMAP���ת����HDIB���
	
	BITMAP bm;               // bitmap structure
	BITMAPINFOHEADER bi;     // bitmap header
	LPBITMAPINFOHEADER lpbi1; // pointer to BITMAPINFOHEADER
	DWORD dwLen;             // size of memory block
	HDIB hDib, h;            // handle to DIB, temp handle
	HDC hDC;                 // handle to DC
	WORD biBits;             // bits per pixel
	UINT wLineLen;           //CBITMAP��������ʾ��ͼ��DDB����ʵ�ʿ�ȵ��ֽ��������Ҫ��32bit�ı�����
	DWORD dwSize;             //CBITMAP��������ʾ��ͼ��DDB���Ĵ�С
	DWORD wColSize;          //��ɫ����ռ���ֽ���
	
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
	
	
	//��һ�����ĵ�����ʾ�õ���DIBͼ��
	CMicroscopeApp* app =(CMicroscopeApp*)AfxGetApp();
	
	//�����µ��ĵ�
	POSITION posTemplate = app->GetFirstDocTemplatePosition();
	CDocTemplate* pDocTemplate = app->GetNextDocTemplate(posTemplate);
	CMicroscopeDoc* pDocument = (CMicroscopeDoc*) pDocTemplate->OpenDocumentFile(NULL);
	
	//����HDIB�����Ҹ�����ͼ
	pDocument->ReplaceHDIB(hDib);
	pDocument->InitDIBData();    // set up new size & palette
	pDocument->ReplaceFlag(FALSE); //��дtxt��Ϣ�ļ�
	pDocument->DrawRectTrackerflag=FALSE;	//����Ƥ�����α�־Ϊ��
	
	pDocument->SetModifiedFlag(TRUE);
	pDocument->UpdateAllViews(NULL);	
	
	EndWaitCursor();
	
	return;
		
}

LRESULT CMicroscopeView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{

	LONG width;               //ͼ���� 2008.01.15
	LONG height;              //ͼ��߶� 2008.01.15
	
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

	//�õ�ͼ��ĸ߶ȺͿ��
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
