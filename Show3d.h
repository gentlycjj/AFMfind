#if !defined(AFX_SHOW3D_H__48825E42_C53C_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
#define AFX_SHOW3D_H__48825E42_C53C_11D4_9E5C_0080C8DB8CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Show3d.h : header file
//
#include "gl\gl.h"
//#include "gl\glu.h"  //���Բ���Ҫ��û���õ�OpenGLʵ�ÿ⺯��

/////////////////////////////////////////////////////////////////////////////
// CShow3d dialog

class CShow3d : public CDialog
{
// Construction
public:
	int takeinteger(double z);
	void RasterFont();
	void PrintString(const char* str);
	HACCEL Popup_Accel; //�����˵��ļ��ټ����
	float brightness; //����
	float contrast;  //�Աȶ�
	float solid;     //����ȣ�z������ֵ��
	float position;  //ͼ������λ�ã�����ͼ�����������ƶ���
	void DrawScene();
	BOOL bSetupPixelFormat();
	void Init();
	HDIB holdDIB;   //��ά��ǰ��ͼ����
	BYTE **bmpoldbits;  //��ά��ǰ��ͼ������ֵ��ά����
	LONG oldwidth;      //��ά��ǰ��ͼ����
	LONG oldheight;     //��ά��ǰ��ͼ��߶�
	LONG oldfactwidth;  //��ά��ǰ��ͼ��DIB���ݿ��ʵ��ʹ�õ��ֽ��� DIBͼ��Ŀ��Ҫ��4byte�ı���
	RGBQUAD bmpoldcolorpalette[256]; //��ά��ǰ��ͼ��ĵ�ɫ������
	HGLRC m_hRC;//RC ���
	CClientDC *m_pDC;//DC
	GLdouble m_xRotate;  //x������ת��
	GLdouble m_yRotate;   //y������ת��
	BOOL m_LeftButtonDown; //�������Ƿ񱻰��µı�־
	CPoint m_LeftDownPos;
	COLORREF m_BackgroundColor; //������ɫ
	COLORREF m_FrontColor; //ͼ����ɫ��ǰ����ɫ��	
	COLORREF m_LineColor; //ͼ����ɫ��ǰ����ɫ��
	BYTE Frontred;   //ǰ����ɫ�еĺ�ɫ
	BYTE Frontblue;  //ǰ����ɫ�е���ɫ
	BYTE Frontgreen;  //ǰ����ɫ�е���ɫ
	CShow3d(CWnd* pParent = NULL);   // standard constructor
	int scanrangex;     //ͼ��X����ɨ�跶Χ
    int scanrangey;
	char timechar[25];  //��ʱ�ַ�����

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
	float zmax;        //ͼ��Z������ѹֵ
	float zmin;        //ͼ��Z����С��ѹֵ
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
