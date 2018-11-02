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
	WORD DAdata;     //输出的16位整型值
	HANDLE hDevice;  //设备对象句柄
	int DeviceID;    //设备ID( Identifier )标识号
	ULONG PhysAddr;  //映射寄存器指向的物理地址，它指明该设备位于系统空间的物理位置
	ULONG LinearAddr;//指针参数，用于返回所取得的映射寄存器指向的线性地址,虚拟位置
	WORD  pADBuffer[512];//接受AD数据的用户缓冲区
//**************************************pci2003**********************************************//

	CSinglescanner(CWnd* pParent = NULL);   // standard constructor
//	CMeasure measuredlg;
	int singlelargecurve[400],singlesmallcurve[200];
	int width,height;
	float singlexaxis,singleyaxis,singlexaxiss,singleyaxiss;
//	CMeasure measuredlg;
	float singlezave;  //z平均值
	float singlezmax;  //z最大值
	float singlezmin;  //z最小值
	RGBQUAD singlebmpcolorpalette[256]; //调色板数据
	BYTE singlelargebmpbits[400][400]; //大图bmp索引值
	BYTE singlesmallbmpbits[100][100];  //小图bmp索引值
	float singlelargebmpdata[400][200]; //大图采集数据
	float singlesmallbmpdata[200][100]; //小图采集数据
	float tempmax,tempmin,tempave,tempv;
	char buffer[15]; //临时字符数组
	int singletime;  //滑块所表示的时间，即系统的扫描时间
	int singletimelast; //滑块所表示的时间，指当按下“开始扫描”按钮后当时记录下来的滑块时间
	int singlelightnessthreshold; //滑块所表示的亮度阈值，即以上一幅图像中采集数据最大值的百分之多少作为下一幅图像的最大值
	int singlelightnessthresholdlast; //滑块所表示的亮度阈值，指当按下“开始扫描”按钮后当时记录下来的亮度阈值
	BOOL singlescanendflag;  //本次扫描是否结束的标志
	double singlevoltagex,singlevoltagey; //实际写入的电压信号
	double singledelaytime; //每扫描一点所要延迟的时间
	double singlemovevoltagex;    //x方向每个像元所需的电压
	double singlemovevoltagey;	//y方向每个像元所需的电压
	double singlepianzhivoltagey;  //x方向需要事先偏移的电压
	double singlepianzhivoltagex;  //y方向需要事先偏移的电压
	int radiobmpsize;
	int direction;
	int singleyold,singlexold;
	float singleceramicscoefficient; //压电陶瓷非线性校正系数
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
	double singlesystemdelay; //系统执行指令所需的时间，此时间应该扣除
	float singlezavetemp; //计算z平均值的临时变量
	BOOL singlequickstopflag; //立刻从扫描循环中退出的标志
	BOOL singleredrawflag;  //对话框重画的标志
	BOOL singlesettimerflag; //设置WM_TIME的标志
	LONGLONG begin,end;
	double singlefreq;      //计算机高精度计时器的频率
	LARGE_INTEGER litmp; //高精度计时器的时间变量
	int j;
	int i;
	int high,low; //高8位和低4位
	int tempad;  //AD转换时读转换是否完成的临时变量
	BYTE singletempindex; //画点时的临时RGB颜色值
	float singletempbmpbits; //存放bmp图像数据的临时变量
	float singleoldzmax;  //上一幅扫描图像中的z最大值
	float singleoldzmin;  //上一幅扫描图像中的z最小值
	MSG message;
	char timechar[15];  //临时字符数组
//	int show;
	char thresholdchar[15]; //临时亮度阈值字符数组
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
