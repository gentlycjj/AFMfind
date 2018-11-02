// RectTrackerEx.cpp : implementation file
//

#include "stdafx.h"
#include "RectTrackerEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRectTrackerEx

CRectTrackerEx::CRectTrackerEx()
{
}

CRectTrackerEx::~CRectTrackerEx()
{
}
void CRectTrackerEx::DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo,
							 CDC* pDC, CWnd* pWnd)
{
	CRectTracker::DrawTrackerRect(lpRect,pWndClipTo,pDC,pWnd);
	pWnd->PostMessage(WM_AAAA,lpRect->right-lpRect->left,lpRect->bottom- lpRect->top);

}
void CRectTrackerEx::Draw(CDC* pDC) 
{
	CRectTracker::Draw(pDC);
}
BOOL CRectTrackerEx::SetCursor(CWnd* pWnd, UINT nHitTest)
{
	BOOL ret = CRectTracker::SetCursor(pWnd,nHitTest);
	return ret;
}
BOOL CRectTrackerEx::Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert,
						   CWnd* pWndClipTo)
{
	BOOL ret = CRectTracker::Track(pWnd,point,bAllowInvert,pWndClipTo);
	return ret;
}
BOOL CRectTrackerEx::TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bAllowInvert )
{

	BOOL ret = CRectTracker::TrackRubberBand(pWnd,point,bAllowInvert);

	return ret;
}
int CRectTrackerEx::HitTest(CPoint point)
{
	return CRectTracker::HitTest(point);
}

void CRectTrackerEx::SetRect(CRect rect)
{
	CRectTracker::m_rect.SetRect(rect.left,rect.top,rect.right,rect.bottom);
}

void CRectTrackerEx::SetStyle(UINT iStyle)
{
	CRectTracker::m_nStyle = iStyle;

}





















