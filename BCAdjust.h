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
	BYTE **bmpoldbits;  //原始图像索引值二维数组
	BYTE **bmpnewbits;  //处理后图像索引值二维数组
	LONG width;               //图像宽度
	LONG height;               //图像高度
	LONG factwidth;           //图像DIB数据宽度实际使用的字节数 DIB图像的宽度要是4byte的倍数
   	LPSTR lpbi;              // pointer to packed-DIB
	float brightness;       //亮度百分比
	float contrast;    //对比度百分比
	char bcchar[5]; //临时字符数组
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
