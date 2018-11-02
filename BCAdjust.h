#if !defined(AFX_BCADJUST_H__DBC68B80_BFF2_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
#define AFX_BCADJUST_H__DBC68B80_BFF2_11D4_9E5C_0080C8DB8CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BCAdjust.h : header file
//
#include "Dibapi.h"
/////////////////////////////////////////////////////////////////////////////
// CBCAdjust dialog

class CBCAdjust : public CDialog
{
// Construction
public:
	HDIB hDIB;
	CBCAdjust(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBCAdjust)
	enum { IDD = IDD_BCADJUST };
	CStatic	m_staticcontrast;
	CStatic	m_staticbrightness;
	CSliderCtrl	m_slidercontrast;
	CSliderCtrl	m_sliderbrightness;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCAdjust)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BYTE **bmpoldbits;  //ԭʼͼ������ֵ��ά����
	BYTE **bmpnewbits;  //�����ͼ������ֵ��ά����
	LONG width;               //ͼ����
	LONG height;               //ͼ��߶�
	LONG factwidth;           //ͼ��DIB���ݿ��ʵ��ʹ�õ��ֽ��� DIBͼ��Ŀ��Ҫ��4byte�ı���
   	LPSTR lpbi;              // pointer to packed-DIB
	float brightness;       //���Ȱٷֱ�
	float contrast;    //�ԱȶȰٷֱ�
	char bcchar[5]; //��ʱ�ַ�����
	int i,j;

	// Generated message map functions
	//{{AFX_MSG(CBCAdjust)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCADJUST_H__DBC68B80_BFF2_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
