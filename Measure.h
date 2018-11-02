#if !defined(AFX_MEASURE_H__C25F3C81_A366_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
#define AFX_MEASURE_H__C25F3C81_A366_11D4_9E5C_0080C8DB8CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Measure.h : header file
//**************************************pci2003**********************************************//
//#include "pci2003.h"    //添加pci2003的头文件
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
	WORD DAdata;     //输出的16位整型值
	HANDLE hDevice;  //设备对象句柄
	int DeviceID;    //设备ID( Identifier )标识号
	UCHAR PhysAddr;  //映射寄存器指向的物理地址，它指明该设备位于系统空间的物理位置
	UCHAR LinearAddr;//指针参数，用于返回所取得的映射寄存器指向的线性地址,虚拟位置
	WORD  pADBuffer[512];//接受AD数据的用户缓冲区
	//**************************************pci2003**********************************************//
    float zave;  //z平均值
	float zmax;  //z最大值
	float zmin;  //z最小值
	float Ra;    //Ra粗糙度
	float Ry;    //Ry粗糙度
	float Rz;    //Rz粗糙度
	RGBQUAD bmpcolorpalette[256]; //调色板数据
	BITMAPINFOHEADER largebmpinfoheader; //大图bmp文件信息头
	BITMAPINFOHEADER smallbmpinfoheader; //小图bmp文件信息头
	BYTE largebmpbits[400][400]; //大图bmp索引值
	BYTE smallbmpbits[100][100];  //小图bmp索引值
	float largebmpdata[400][400]; //大图采集数据
	float smallbmpdata[100][100]; //小图采集数据
	int time;  //滑块所表示的时间，即系统的扫描时间
	int timelast; //滑块所表示的时间，指当按下“开始扫描”按钮后当时记录下来的滑块时间
	int lightnessthreshold; //滑块所表示的亮度阈值，即以上一幅图像中采集数据最大值的百分之多少作为下一幅图像的最大值
	int lightnessthresholdlast; //滑块所表示的亮度阈值，指当按下“开始扫描”按钮后当时记录下来的亮度阈值
	CSinglescanner singlescannerdlg;
	//	CMicroscopeApp appdlg;
	float largecurve[400],smallcurve[200];
	float xaxis,yaxis;
	BOOL scanendflag;  //本次扫描是否结束的标志
	double movevoltagex;    //x方向每个像元所需的电压
	double movevoltagey;	//y方向每个像元所需的电压
	double pianzhivoltagey;  //x方向需要事先偏移的电压
	double pianzhivoltagex;  //y方向需要事先偏移的电压
	double systemdelay; //系统执行指令所需的时间，此时间应该扣除
	double freq;      //计算机高精度计时器的频率
	double voltagex,voltagey; //实际写入的电压信号
	double delaytime; //每扫描一点所要延迟的时间
	
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
		BOOL checkflag;  //check控件的交换标志
		int width;    //扫描图像的宽度
		int height;   //扫描图像的高度
		float zavetemp; //计算z平均值的临时变量
		float Raavetemp; //计算Ra时储存Z方向电压平均值的临时变量
		float Rmaxtemp;  //计算Ry和Rz时储存Z方向电压最大值的临时变量
		float Rmintemp;  //计算Ry和Rz时储存Z方向电压最小值的临时变量
		BOOL quickstopflag; //立刻从扫描循环中退出的标志
		BOOL redrawflag;  //对话框重画的标志
		BOOL settimerflag; //设置WM_TIME的标志
		LARGE_INTEGER litmp; //高精度计时器的时间变量
		int j;
		int i;
		BOOL smalldataexchangeflag; //控件和变量交换数据的标示
		BOOL largedataexchangeflag; //控件和变量交换数据的标示
		int high,low; //高8位和低4位
		int tempad;  //AD转换时读转换是否完成的临时变量
		BYTE tempindex; //画点时的临时RGB颜色值
		float tempbmpbits; //存放bmp图像数据的临时变量
		float oldzmax;  //上一幅扫描图像中的z最大值
		float oldzmin;  //上一幅扫描图像中的z最小值
		
		float ceramicscoefficient; //压电陶瓷非线性校正系数 20140506	
		
		LONGLONG begin,end;
		int bmpscantime;//行扫描时间，当前图扫描时间
		int selecttimes,finishtimes;//手动设置扫描次数，已完成扫描次数
		int scantimetemp,bmpscanbegin,bmpscanend;
		LONGLONG rowscantime,rowscanbegin,rowscanend;
		MSG message;
		BOOL MouseSelectFlag;
		BOOL m_bLMouseDown;
		int xold,xnew,xmove,yold,ynew,ymove,change;
		BOOL rightstart;
		BOOL SelectScanTimeFlag;
		BOOL SingleXFlag,SingleYFlag;
		char timechar[15];  //临时字符数组
		char thresholdchar[15]; //临时亮度阈值字符数组
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
