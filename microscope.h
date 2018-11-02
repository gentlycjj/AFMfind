// microscope.h : main header file for the MICROSCOPE application
//

#if !defined(AFX_MICROSCOPE_H__14722B04_A28A_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
#define AFX_MICROSCOPE_H__14722B04_A28A_11D4_9E5C_0080C8DB8CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Measure.h"	// Added by ClassView
#include "Atomforce.h"
#include "Roughness.h"
#include "Dibapi.h"
//#include "singlescanner.h"
#define WM_DOREALIZE   (WM_USER +100)
/////////////////////////////////////////////////////////////////////////////
// CMicroscopeApp:
// See microscope.cpp for the implementation of this class
//

class CMicroscopeApp : public CWinApp
{
public:
	CMeasure measuredlg;   //扫描测量对话框
	CAtomforce atomforcedlg; //原子力曲线测量对话框
	COLORREF m_OptionColor;  //渲染的颜色变量
	float voltagetonanometer; //Z方向电压对应纳米尺寸的比例
	float ceramics;			//压电陶瓷非线性校正系数
	CString m_strPath;
	void Bmpcapture();
	CMicroscopeApp();
//	double exdelaytime;
//	int extime;
//	double exvoltagex;
//	double exvoltagey;
//	double expianzhivoltagex;
//	double expianzhivoltagey;
//	double exmovevoltagex;
//	double exmovevoltagey;
//	int exradiobmpsize;
//	int exdirection;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMicroscopeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMicroscopeApp)
	afx_msg void OnAppAbout();
	afx_msg void OnMeasure();
	afx_msg void OnSetcolor();
	afx_msg void On3d();
	afx_msg void OnBlur();
	afx_msg void OnPseudocolor();
	afx_msg void OnBCAdjust();
	afx_msg void OnAtomforce();
	afx_msg void OnRoughness();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MICROSCOPE_H__14722B04_A28A_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
