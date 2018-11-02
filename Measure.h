#if !defined(AFX_MEASURE_H__C25F3C81_A366_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
#define AFX_MEASURE_H__C25F3C81_A366_11D4_9E5C_0080C8DB8CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Measure.h : header file
//**************************************pci2003**********************************************//
//#include "pci2003.h"    //���pci2003��ͷ�ļ�
#include "PCI8620.h"
//**************************************pci2003**********************************************//
/////////////////////////////////////////////////////////////////////////////
// CMeasure dialog
#include "resource.h"
#include "singlescanner.h"
#include "microscope.h"

class CMeasure : public CDialog
{
	// Construction
public:
	void SetStatus(int type, int status, int num,...);
	bool ReadDataFromAD(HANDLE hDevice, PCI8620_PARA_AD para, float &volt);
	float Volt;	
	ULONG PCI8620_ADBuffer;
	LONG nRealNum;
	//		float Volt1;
	//**************************************pci2003**********************************************//
	//int nDAChannel;  
	WORD DAdata;     //�����16λ����ֵ
	HANDLE hDevice;  //�豸������
	int DeviceID;    //�豸ID( Identifier )��ʶ��
	UCHAR PhysAddr;  //ӳ��Ĵ���ָ��������ַ����ָ�����豸λ��ϵͳ�ռ������λ��
	UCHAR LinearAddr;//ָ����������ڷ�����ȡ�õ�ӳ��Ĵ���ָ������Ե�ַ,����λ��
	WORD  pADBuffer[512];//����AD���ݵ��û�������
	//**************************************pci2003**********************************************//
    float zave;  //zƽ��ֵ
	float zmax;  //z���ֵ
	float zmin;  //z��Сֵ
	float Ra;    //Ra�ֲڶ�
	float Ry;    //Ry�ֲڶ�
	float Rz;    //Rz�ֲڶ�
	RGBQUAD bmpcolorpalette[256]; //��ɫ������
	BITMAPINFOHEADER largebmpinfoheader; //��ͼbmp�ļ���Ϣͷ
	BITMAPINFOHEADER smallbmpinfoheader; //Сͼbmp�ļ���Ϣͷ
	BYTE largebmpbits[400][400]; //��ͼbmp����ֵ
	BYTE smallbmpbits[100][100];  //Сͼbmp����ֵ
	float largebmpdata[400][400]; //��ͼ�ɼ�����
	float smallbmpdata[100][100]; //Сͼ�ɼ�����
	int time;  //��������ʾ��ʱ�䣬��ϵͳ��ɨ��ʱ��
	int timelast; //��������ʾ��ʱ�䣬ָ�����¡���ʼɨ�衱��ť��ʱ��¼�����Ļ���ʱ��
	int lightnessthreshold; //��������ʾ��������ֵ��������һ��ͼ���вɼ��������ֵ�İٷ�֮������Ϊ��һ��ͼ������ֵ
	int lightnessthresholdlast; //��������ʾ��������ֵ��ָ�����¡���ʼɨ�衱��ť��ʱ��¼������������ֵ
	CSinglescanner singlescannerdlg;
	//	CMicroscopeApp appdlg;
	float largecurve[400],smallcurve[200];
	float xaxis,yaxis;
	BOOL scanendflag;  //����ɨ���Ƿ�����ı�־
	double movevoltagex;    //x����ÿ����Ԫ����ĵ�ѹ
	double movevoltagey;	//y����ÿ����Ԫ����ĵ�ѹ
	double pianzhivoltagey;  //x������Ҫ����ƫ�Ƶĵ�ѹ
	double pianzhivoltagex;  //y������Ҫ����ƫ�Ƶĵ�ѹ
	double systemdelay; //ϵͳִ��ָ�������ʱ�䣬��ʱ��Ӧ�ÿ۳�
	double freq;      //������߾��ȼ�ʱ����Ƶ��
	double voltagex,voltagey; //ʵ��д��ĵ�ѹ�ź�
	double delaytime; //ÿɨ��һ����Ҫ�ӳٵ�ʱ��
	
	CMeasure(CWnd* pParent = NULL);   // standard constructor
	~CMeasure();
	// Dialog Data
	//{{AFX_DATA(CMeasure)
	enum { IDD = IDD_MEASURE };
	CEdit	m_csmallrangey;
	CEdit	m_csmallrangex;
	CButton	m_showbmp;
	CEdit	m_csingleyscantxt;
	CEdit	m_csinglexscantxt;
	CSpinButtonCtrl	m_spinsingleyscan;
	CSpinButtonCtrl	m_spinsinglexscan;
	CSpinButtonCtrl	m_spinsingleytxt;
	CSpinButtonCtrl	m_spinsinglextxt;
	CStatic	m_finishscantimes;
	CStatic	m_bmpscantime;
	CStatic	m_rowscantime;
	CSpinButtonCtrl	m_spinselecttimes;
	CEdit	m_cpianzhiy;
	CEdit	m_cpianzhix;
	CStatic	m_mouserangeyto;
	CStatic	m_mouserangeyfrom;
	CStatic	m_mouserangexto;
	CStatic	m_mouserangexfrom;
	CEdit	m_clargerangey;
	CEdit	m_clargerangex;
	CStatic	m_showRz;
	CStatic	m_showRy;
	CStatic	m_showRa;
	CStatic	m_showRzdanwei;
	CStatic	m_showRydanwei;
	CStatic	m_showRadanwei;
	CStatic	m_showthreshold;
	CSliderCtrl	m_sliderthreshold;
	CButton	m_buttongoback;
	CButton	m_ok;
	CButton	m_cancel;
	CStatic	m_pianzhirangey;
	CStatic	m_pianzhirangex;
	CSpinButtonCtrl	m_spinpianzhiy;
	CSpinButtonCtrl	m_spinpianzhix;
	CStatic	m_showzmindanwei;
	CStatic	m_showzmaxdanwei;
	CStatic	m_showydanwei;
	CStatic	m_showxdanwei;
	CButton	m_buttonstopscan;
	CButton	m_buttonstartscan;
	CButton	m_buttonbmpcapture;
	CSpinButtonCtrl	m_spinrangesmally;
	CSpinButtonCtrl	m_spinrangesmallx;
	CSpinButtonCtrl	m_spinrangelargey;
	CSpinButtonCtrl	m_spinrangelargex;
	CStatic	m_showzmin;
	CStatic	m_showzmax;
	CStatic	m_showy;
	CStatic	m_showx;
	CSliderCtrl	m_slidertime;
	CStatic	m_showtime;
	int		m_largerangex;
	int		m_largerangey;
	int		m_radiorange;
	int		m_radiobmpsize;
	int		m_smallrangex;
	int		m_smallrangey;
	BOOL	m_enablepianzhiflag;
	int		m_pianzhix;
	int		m_pianzhiy;
	BOOL	m_mouseselect;
	int		m_selecttimes;
	BOOL	m_selectscantime;
	int		m_direction;
	BOOL	m_checksinglescan;
	int		m_singlescan;
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasure)
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL
		
		// Implementation
	protected:
		BOOL pianzhiflag;
		BOOL checkflag;  //check�ؼ��Ľ�����־
		int width;    //ɨ��ͼ��Ŀ��
		int height;   //ɨ��ͼ��ĸ߶�
		float zavetemp; //����zƽ��ֵ����ʱ����
		float Raavetemp; //����Raʱ����Z�����ѹƽ��ֵ����ʱ����
		float Rmaxtemp;  //����Ry��Rzʱ����Z�����ѹ���ֵ����ʱ����
		float Rmintemp;  //����Ry��Rzʱ����Z�����ѹ��Сֵ����ʱ����
		BOOL quickstopflag; //���̴�ɨ��ѭ�����˳��ı�־
		BOOL redrawflag;  //�Ի����ػ��ı�־
		BOOL settimerflag; //����WM_TIME�ı�־
		LARGE_INTEGER litmp; //�߾��ȼ�ʱ����ʱ�����
		int j;
		int i;
		BOOL smalldataexchangeflag; //�ؼ��ͱ����������ݵı�ʾ
		BOOL largedataexchangeflag; //�ؼ��ͱ����������ݵı�ʾ
		int high,low; //��8λ�͵�4λ
		int tempad;  //ADת��ʱ��ת���Ƿ���ɵ���ʱ����
		BYTE tempindex; //����ʱ����ʱRGB��ɫֵ
		float tempbmpbits; //���bmpͼ�����ݵ���ʱ����
		float oldzmax;  //��һ��ɨ��ͼ���е�z���ֵ
		float oldzmin;  //��һ��ɨ��ͼ���е�z��Сֵ
		
		float ceramicscoefficient; //ѹ���մɷ�����У��ϵ�� 20140506	
		
		LONGLONG begin,end;
		int bmpscantime;//��ɨ��ʱ�䣬��ǰͼɨ��ʱ��
		int selecttimes,finishtimes;//�ֶ�����ɨ������������ɨ�����
		int scantimetemp,bmpscanbegin,bmpscanend;
		LONGLONG rowscantime,rowscanbegin,rowscanend;
		MSG message;
		BOOL MouseSelectFlag;
		BOOL m_bLMouseDown;
		int xold,xnew,xmove,yold,ynew,ymove,change;
		BOOL rightstart;
		BOOL SelectScanTimeFlag;
		BOOL SingleXFlag,SingleYFlag;
		char timechar[15];  //��ʱ�ַ�����
		char thresholdchar[15]; //��ʱ������ֵ�ַ�����
		int oldlargerangex,oldpianzhix,oldpianzhiy;
		int oldsmallrangex;
		
		// Generated message map functions
		//{{AFX_MSG(CMeasure)
		virtual BOOL OnInitDialog();
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void OnRadiosmallrange();
		afx_msg void OnRadiolargerange();
		afx_msg void OnButtonbmpcapture();
		afx_msg void OnButtonstartscan();
		afx_msg void OnButtonstopscan();
		afx_msg void OnPaint();
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg void OnCheckpianzhi();
		afx_msg void OnButtongoback();
		virtual void OnCancel();
		virtual void OnOK();
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnMouseselect();
		afx_msg void OnSelectscantime();
		afx_msg void OnSinglexscan();
		afx_msg void OnSingleyscan();
		afx_msg void OnChecksinglescan();
		afx_msg void OnXdirection();
		afx_msg void OnYdirection();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRowscantime();
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEASURE_H__C25F3C81_A366_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
