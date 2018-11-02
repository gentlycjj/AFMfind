#if !defined(AFX_RECTTRACKEREX_H__7CF10E08_C397_4E55_9579_0D511D8789D3__INCLUDED_)
#define AFX_RECTTRACKEREX_H__7CF10E08_C397_4E55_9579_0D511D8789D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RectTrackerEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRectTrackerEx window
#define WM_AAAA WM_USER+111
class CRectTrackerEx : public CRectTracker
{
// Construction
public:
	CRectTrackerEx();
// Operations
public:
	void Draw(CDC* pDC);
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest);
	BOOL Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert = FALSE,CWnd* pWndClipTo = NULL);
	BOOL TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bAllowInvert = TRUE);
	int HitTest(CPoint point);
// Implementation
public:
	void SetStyle(UINT iStyle);
	void SetRect(CRect rect);
	virtual ~CRectTrackerEx();
    virtual void DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo,CDC* pDC, CWnd* pWnd);
	// Generated message map functions
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECTTRACKEREX_H__7CF10E08_C397_4E55_9579_0D511D8789D3__INCLUDED_)
