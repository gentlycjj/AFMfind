#if !defined(AFX_SHOW3D_H__48825E42_C53C_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
#define AFX_SHOW3D_H__48825E42_C53C_11D4_9E5C_0080C8DB8CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Show3d.h : header file
//
#include "gl\gl.h"
//#include "gl\glu.h"  //可以不需要，没有用到OpenGL实用库函数

/////////////////////////////////////////////////////////////////////////////
// CShow3d dialog

class CShow3d : public CDialog
{
// Construction
public:
	int takeinteger(double z);
	void RasterFont();
	void PrintString(const char* str);
	HACCEL Popup_Accel; //弹出菜单的加速键句柄
	float brightness; //亮度
	float contrast;  //对比度
	float solid;     //立体度（z方向数值）
	float position;  //图像整体位置（用来图像整体上下移动）
	void DrawScene();
	BOOL bSetupPixelFormat();
	void Init();
	HDIB holdDIB;   //三维化前的图像句柄
	BYTE **bmpoldbits;  //三维化前的图像索引值二维数组
	LONG oldwidth;      //三维化前的图像宽度
	LONG oldheight;     //三维化前的图像高度
	LONG oldfactwidth;  //三维化前的图像DIB数据宽度实际使用的字节数 DIB图像的宽度要是4byte的倍数
	RGBQUAD bmpoldcolorpalette[256]; //三维化前的图像的调色板数据
	HGLRC m_hRC;//RC 句柄
	CClientDC *m_pDC;//DC
	GLdouble m_xRotate;  //x方向旋转量
	GLdouble m_yRotate;   //y方向旋转量
	BOOL m_LeftButtonDown; //鼠标左键是否被按下的标志
	CPoint m_LeftDownPos;
	COLORREF m_BackgroundColor; //背景颜色
	COLORREF m_FrontColor; //图像颜色（前景颜色）	
	COLORREF m_LineColor; //图像颜色（前景颜色）
	BYTE Frontred;   //前景颜色中的红色
	BYTE Frontblue;  //前景颜色中的蓝色
	BYTE Frontgreen;  //前景颜色中的绿色
	CShow3d(CWnd* pParent = NULL);   // standard constructor
	int scanrangex;     //图像X方向扫描范围
    int scanrangey;
	char timechar[25];  //临时字符数组

// Dialog Data
	//{{AFX_DATA(CShow3d)
	enum { IDD = IDD_SHOW3D };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShow3d)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void drawstring(unsigned char* str);
	bool axisflag;
	int i;
	int j;
	BOOL legendflag;
	BOOL xrotateflag;
	BOOL yrotateflag;
    BOOL randomrotateflag;
	BOOL stoprotateflag;
	GLuint  m_nFontOffset;
	float zmax;        //图像Z向最大电压值
	float zmin;        //图像Z向最小电压值
	float zmax2;
	float zmin2;
	// Generated message map functions
	//{{AFX_MSG(CShow3d)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnPopupBackground();
	afx_msg void OnPopupFront();
	afx_msg void OnPopupLine();
	afx_msg void OnPopupAddcontrast();
	afx_msg void OnPopupResetcontrast();
	afx_msg void OnPopupSubtractcontrast();
	afx_msg void OnPopupAddbrightness();
	afx_msg void OnPopupResetbrightness();
	afx_msg void OnPopupSubtractbrightness();
	afx_msg void OnPopupAddsolid();
	afx_msg void OnPopupResetsolid();
	afx_msg void OnPopupSubtractsolid();
	afx_msg void OnPopupImageupper();
	afx_msg void OnPopupImagelower();
	afx_msg void OnPopupImagereset();
	afx_msg void OnPopupCaptureimage();
	afx_msg void OnPopupClose();
	afx_msg void OnPopupAddaxis();
	afx_msg void OnPopupSubtractaxis();
	afx_msg void OnPopupAddlegend();
	afx_msg void OnPopupDeletelegend();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPopupXrotate();
	afx_msg void OnPopupYrotate();
	afx_msg void OnPopupRandomrotate();
	afx_msg void OnPopupStoptotate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOW3D_H__48825E42_C53C_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
