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
	WORD DAdata;     //�����16λ����ֵ
	HANDLE hDevice;  //�豸������
	int DeviceID;    //�豸ID( Identifier )��ʶ��
	ULONG PhysAddr;  //ӳ��Ĵ���ָ��������ַ����ָ�����豸λ��ϵͳ�ռ������λ��
	ULONG LinearAddr;//ָ����������ڷ�����ȡ�õ�ӳ��Ĵ���ָ������Ե�ַ,����λ��
	WORD  pADBuffer[512];//����AD���ݵ��û�������
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
	int range; //������Χ
	int imagerange; //��ԭ��������ͼ����ʾ�Ĳ�����Χ
	char rangechar[15]; //��ʱ�ַ�����
	char buffer[15]; //��ʱ�ַ�����
	int time; //����ʱ��
	int pointx; //������y��x����
	int pointy; //������y�����϶����y����
	float forcedata[200]; //AD��ֱ�Ӳ�����ԭʼ���ݣ���ԭ������С
	int lastforcedata[200]; //��һ�����ԭ������С�����ݣ���ԭʼ���ݹ�һ���������
	COLORREF backgroundcolor;  //ͼ�񱳾�����ɫ����
	COLORREF coordinatecolor;  //�������ɫ����
	COLORREF curvecolor;  //���ߵ���ɫ����
	int i;
	BOOL quickstopflag; //���̴�ɨ��ѭ�����˳��ı�־
	BOOL flag; //�Ƿ����ߵı�־
	double movevoltage; //ÿ�ƶ�һ������ĵ�ѹ
	double freq;      //������߾��ȼ�ʱ����Ƶ��
	LARGE_INTEGER litmp; //�߾��ȼ�ʱ����ʱ�����
	double systemdelay; //ϵͳִ��ָ�������ʱ�䣬��ʱ��Ӧ�ÿ۳�
	double delaytime; //ÿ�ƶ�һ����Ҫ�ӳٵ�ʱ��
	
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
