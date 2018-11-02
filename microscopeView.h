// microscopeView.h : interface of the CMicroscopeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICROSCOPEVIEW_H__14722B0E_A28A_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
#define AFX_MICROSCOPEVIEW_H__14722B0E_A28A_11D4_9E5C_0080C8DB8CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "microscopeDoc.h"

class CMicroscopeView : public CScrollView
{
protected: // create from serialization only
	CMicroscopeView();
	DECLARE_DYNCREATE(CMicroscopeView)

float Ra;

// Attributes
public:
	CMicroscopeDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMicroscopeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMicroscopeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	BOOL Keyflag;
	BOOL ShowRectTrackerflag;  //对于可以在其上面画橡皮区矩形的图像上画橡皮区矩形完毕后是否显示其矩形的标志
	//{{AFX_MSG(CMicroscopeView)
	afx_msg LRESULT OnDoRealize(WPARAM wParam, LPARAM lParam);  // user message
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnSize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in microscopeView.cpp
inline CMicroscopeDoc* CMicroscopeView::GetDocument()
   { return (CMicroscopeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MICROSCOPEVIEW_H__14722B0E_A28A_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
