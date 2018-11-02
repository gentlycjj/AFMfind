#if !defined(AFX_SINGLESCANNER_H__F857CC21_69CB_11D7_9F30_DA6EC8A9FC09__INCLUDED_)
#define AFX_SINGLESCANNER_H__F857CC21_69CB_11D7_9F30_DA6EC8A9FC09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Singlescanner.h : header file
//
#include "Measure.h"

/////////////////////////////////////////////////////////////////////////////
// CSinglescanner dialog

class CSinglescanner : public CDialog
{
// Construction
public:
	
	float Volt;
//	float Volt1;
//**************************************pci2003**********************************************//
	//int nDAChannel;  
	WORD DAdata;     //�����16λ����ֵ
	HANDLE hDevice;  //�豸������
	int DeviceID;    //�豸ID( Identifier )��ʶ��
	ULONG PhysAddr;  //ӳ��Ĵ���ָ��������ַ����ָ�����豸λ��ϵͳ�ռ������λ��
	ULONG LinearAddr;//ָ����������ڷ�����ȡ�õ�ӳ��Ĵ���ָ������Ե�ַ,����λ��
	WORD  pADBuffer[512];//����AD���ݵ��û�������
//**************************************pci2003**********************************************//

	CSinglescanner(CWnd* pParent = NULL);   // standard constructor
//	CMeasure measuredlg;
	int singlelargecurve[400],singlesmallcurve[200];
	int width,height;
	float singlexaxis,singleyaxis,singlexaxiss,singleyaxiss;
//	CMeasure measuredlg;
	float singlezave;  //zƽ��ֵ
	float singlezmax;  //z���ֵ
	float singlezmin;  //z��Сֵ
	RGBQUAD singlebmpcolorpalette[256]; //��ɫ������
	BYTE singlelargebmpbits[400][400]; //��ͼbmp����ֵ
	BYTE singlesmallbmpbits[100][100];  //Сͼbmp����ֵ
	float singlelargebmpdata[400][200]; //��ͼ�ɼ�����
	float singlesmallbmpdata[200][100]; //Сͼ�ɼ�����
	float tempmax,tempmin,tempave,tempv;
	char buffer[15]; //��ʱ�ַ�����
	int singletime;  //��������ʾ��ʱ�䣬��ϵͳ��ɨ��ʱ��
	int singletimelast; //��������ʾ��ʱ�䣬ָ�����¡���ʼɨ�衱��ť��ʱ��¼�����Ļ���ʱ��
	int singlelightnessthreshold; //��������ʾ��������ֵ��������һ��ͼ���вɼ��������ֵ�İٷ�֮������Ϊ��һ��ͼ������ֵ
	int singlelightnessthresholdlast; //��������ʾ��������ֵ��ָ�����¡���ʼɨ�衱��ť��ʱ��¼������������ֵ
	BOOL singlescanendflag;  //����ɨ���Ƿ�����ı�־
	double singlevoltagex,singlevoltagey; //ʵ��д��ĵ�ѹ�ź�
	double singledelaytime; //ÿɨ��һ����Ҫ�ӳٵ�ʱ��
	double singlemovevoltagex;    //x����ÿ����Ԫ����ĵ�ѹ
	double singlemovevoltagey;	//y����ÿ����Ԫ����ĵ�ѹ
	double singlepianzhivoltagey;  //x������Ҫ����ƫ�Ƶĵ�ѹ
	double singlepianzhivoltagex;  //y������Ҫ����ƫ�Ƶĵ�ѹ
	int radiobmpsize;
	int direction;
	int singleyold,singlexold;
	float singleceramicscoefficient; //ѹ���մɷ�����У��ϵ��
	int singlepianzhix,singlepianzhiy;
	int singlelargerangex,singlelargerangey;
//	CMeasure measuredlg;
// Dialog Data
	//{{AFX_DATA(CSinglescanner)
	enum { IDD = IDD_SINGLESCANNER };
	CButton	m_stop;
	CButton	m_goback;
	CButton	m_getdata;
	CButton	m_capture;
	CStatic	m_bmpframe;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSinglescanner)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL singlepianzhiflag;

	BOOL toolarge;
	double singlesystemdelay; //ϵͳִ��ָ�������ʱ�䣬��ʱ��Ӧ�ÿ۳�
	float singlezavetemp; //����zƽ��ֵ����ʱ����
	BOOL singlequickstopflag; //���̴�ɨ��ѭ�����˳��ı�־
	BOOL singleredrawflag;  //�Ի����ػ��ı�־
	BOOL singlesettimerflag; //����WM_TIME�ı�־
	LONGLONG begin,end;
	double singlefreq;      //������߾��ȼ�ʱ����Ƶ��
	LARGE_INTEGER litmp; //�߾��ȼ�ʱ����ʱ�����
	int j;
	int i;
	int high,low; //��8λ�͵�4λ
	int tempad;  //ADת��ʱ��ת���Ƿ���ɵ���ʱ����
	BYTE singletempindex; //����ʱ����ʱRGB��ɫֵ
	float singletempbmpbits; //���bmpͼ�����ݵ���ʱ����
	float singleoldzmax;  //��һ��ɨ��ͼ���е�z���ֵ
	float singleoldzmin;  //��һ��ɨ��ͼ���е�z��Сֵ
	MSG message;
	char timechar[15];  //��ʱ�ַ�����
//	int show;
	char thresholdchar[15]; //��ʱ������ֵ�ַ�����
	int tempindexcurve;

	// Generated message map functions
	//{{AFX_MSG(CSinglescanner)
	afx_msg void OnGoback();
	afx_msg void OnCapture();
	afx_msg void OnGetdata();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStop();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SINGLESCANNER_H__F857CC21_69CB_11D7_9F30_DA6EC8A9FC09__INCLUDED_)
