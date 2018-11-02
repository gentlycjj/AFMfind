#if !defined(AFX_ATOMFORCE_H__D4F12940_2C09_11D5_9E60_0080C8DB8CF1__INCLUDED_)
#define AFX_ATOMFORCE_H__D4F12940_2C09_11D5_9E60_0080C8DB8CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Atomforce.h : header file
//
#include "PCI2003.h"
/////////////////////////////////////////////////////////////////////////////
// CAtomforce dialog

class CAtomforce : public CDialog
{
// Construction
public:
	CAtomforce(CWnd* pParent = NULL);   // standard constructor
	
	float Volt;	
//	float Volt1;
//**************************************pci2003**********************************************//
    int nDAChannel;  
	WORD DAdata;     //输出的16位整型值
	HANDLE hDevice;  //设备对象句柄
	int DeviceID;    //设备ID( Identifier )标识号
	ULONG PhysAddr;  //映射寄存器指向的物理地址，它指明该设备位于系统空间的物理位置
	ULONG LinearAddr;//指针参数，用于返回所取得的映射寄存器指向的线性地址,虚拟位置
	WORD  pADBuffer[512];//接受AD数据的用户缓冲区
//**************************************pci2003**********************************************//

// Dialog Data
	//{{AFX_DATA(CAtomforce)
	enum { IDD = IDD_ATOMFORCE };
	CStatic	m_curveimage;
	CStatic	m_curvecolor;
	CStatic	m_coordinatecolor;
	CStatic	m_backgroundcolor;
	CSliderCtrl	m_slidertime;
	CSliderCtrl	m_sliderrange;
	CStatic	m_showrange;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAtomforce)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int range; //测量范围
	int imagerange; //在原子力测量图上显示的测量范围
	char rangechar[15]; //临时字符数组
	char buffer[15]; //临时字符数组
	int time; //测量时间
	int pointx; //坐标轴y的x坐标
	int pointy; //坐标轴y的最上顶点的y坐标
	float forcedata[200]; //AD卡直接测量的原始数据，即原子力大小
	int lastforcedata[200]; //归一化后的原子力大小的数据，即原始数据归一化后的数据
	COLORREF backgroundcolor;  //图像背景的颜色变量
	COLORREF coordinatecolor;  //坐标的颜色变量
	COLORREF curvecolor;  //曲线的颜色变量
	int i;
	BOOL quickstopflag; //立刻从扫描循环中退出的标志
	BOOL flag; //是否画曲线的标志
	double movevoltage; //每移动一步所需的电压
	double freq;      //计算机高精度计时器的频率
	LARGE_INTEGER litmp; //高精度计时器的时间变量
	double systemdelay; //系统执行指令所需的时间，此时间应该扣除
	double delaytime; //每移动一步所要延迟的时间
	
	// Generated message map functions
	//{{AFX_MSG(CAtomforce)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtongoback();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnButtoncapture();
	afx_msg void OnButtonstart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATOMFORCE_H__D4F12940_2C09_11D5_9E60_0080C8DB8CF1__INCLUDED_)
