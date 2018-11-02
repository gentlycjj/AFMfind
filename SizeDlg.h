#if !defined(AFX_SIZEDLG_H__7A4DCC21_7274_11D6_862F_0050BABC4913__INCLUDED_)
#define AFX_SIZEDLG_H__7A4DCC21_7274_11D6_862F_0050BABC4913__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SizeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSizeDlg dialog

class CSizeDlg : public CDialog
{
// Construction
public:
	CSizeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSizeDlg)
	enum { IDD = IDD_SIZE };
	CButton	m_smallBut;
	CSpinButtonCtrl	m_spiny;
	CSpinButtonCtrl	m_spinx;
	DWORD	m_dwX;
	DWORD	m_dwY;
	int		m_iSmall;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSizeDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIZEDLG_H__7A4DCC21_7274_11D6_862F_0050BABC4913__INCLUDED_)
