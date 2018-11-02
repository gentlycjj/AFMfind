// Measure.cpp : implementation file

// AFM源程序软件PCI2003章完全版4000_008_200旋转彩色r10g07b03亮度对比度改变,扫描裁剪有显示,立体彩显,平面显示灰度,Win2000_98含原子力曲线_改变坐标颜色

// 2008年4月30日，修改对比度调节，改变对比度:
// if ((128+(bmpoldbits[i][j]-194)*contrast)<0) // 20080430,原来是－128, 有5处

// 2008年1月09日，增加Topview显示的灰度条显示，在CSize函数中。因此，此版本功能包括：
// 完全版4000_12_200旋转彩色扫描裁剪有显示立体彩显Win2000_98含原子力曲线_改变坐标颜色_TopVeiw显示灰度值

// 对新打开的任何图像，不管象素是400*400点，还是m*m点(m<400)，作裁剪时，鼠标选框显示范围都正确；
// 但如果要二次三次...裁剪，显示范围会有不正确现象，解决办法是把二次裁剪的图像存盘，再打开裁剪即可。
// 或者，所有的裁剪，都从第一幅打开的图像开始。

// 2006年4月30日，此软件升级为Windows2000/98兼容型，改动之处是:
//        将在外面的循环里终止扫描if (quickstopflag==TRUE){break;}
//        前移到if(!PCI2003_InitDeviceProAD(hDevice, &Para))之前 
//        并且将if (singlequickstopflag==TRUE){break;}语句，
//        前移到if(!PCI2003_InitDeviceProAD(hDevice, &Para1)) 之前，共8处。

// 2007年5月21日，为了原子力曲线能够在Windows2000/98下兼容运行，改动之处是：   


// 2014年5月，集中升级了多种软件功能，包括：扫描显示采用新调色板、立体显示采用新调色板、捕获采用新调色板、二维标尺灰度条采用新调色板、电压系数500nm/V、
// 扫描像素改为400*400与200*200、计算上一幅图的最大最小值时先平滑这样即使有外界噪声也不影响下一幅扫描图像对比度、增加恒力模式下的图像消倾斜功能、
// 开始扫描时先扫描10行计算最大最小值作为第一幅完整扫描图像的上下限。  详见Word文本。

#include "stdafx.h"
#include "microscope.h"
#include "Measure.h"
#include "conio.h"
#include "dibapi.h"
#include "math.h"
#include "easyusb_card_dll.h" //2017USB

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

double m_kx, m_ky;
#define OUTPUTRANGE PCI8620_OUTPUT_N5000_P5000mV
#define XDIRECT 1
#define YDIRECT 2

//******************************pci8620*****************************************// 
PCI8620_PARA_AD  Para;  //2014/7 pci2003--->pci8620
//******************************pci8620*****************************************// 

static int usb_open=0; //2017USB

/////////////////////////////////////////////////////////////////////////////
// CMeasure dialog


CMeasure::CMeasure(CWnd* pParent /*=NULL*/)
: CDialog(CMeasure::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMeasure)
	m_enablepianzhiflag = FALSE;
	m_pianzhix = 0;
	m_pianzhiy = 0;
	m_mouseselect = FALSE;
	m_selecttimes = 0;
	m_selectscantime = FALSE;
	m_direction = -1;
	m_checksinglescan = FALSE;
	m_singlescan = -1;

	hDevice = NULL;  //20150212,为了停止扫描时，X和Y电压归零

	//}}AFX_DATA_INIT
}

CMeasure::~CMeasure()
{
	if(hDevice != NULL) PCI8620_ReleaseDevice(hDevice);   //20150212，为了停止扫描时X和Y电压归零
}
void CMeasure::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasure)
	DDX_Control(pDX, IDC_SMALLRANGEY, m_csmallrangey);
	DDX_Control(pDX, IDC_SMALLRANGEX, m_csmallrangex);
	DDX_Control(pDX, IDC_SINGLEYSCANTXT, m_csingleyscantxt);
	DDX_Control(pDX, IDC_SINGLEXSCANTXT, m_csinglexscantxt);
	DDX_Control(pDX, IDC_SPINSINGLEYSCANTXT, m_spinsingleyscan);
	DDX_Control(pDX, IDC_SPINSINGLEXSCANTXT, m_spinsinglexscan);
	DDX_Control(pDX, IDC_FINISHSCANTIMES, m_finishscantimes);
	DDX_Control(pDX, IDC_BMPSCANTIME, m_bmpscantime);
	DDX_Control(pDX, IDC_ROWSCANTIME, m_rowscantime);
	DDX_Control(pDX, IDC_SPINSELECTTIMES, m_spinselecttimes);
	DDX_Control(pDX, IDC_PIANZHIY, m_cpianzhiy);
	DDX_Control(pDX, IDC_PIANZHIX, m_cpianzhix);
	DDX_Control(pDX, IDC_MOUSERANGEYTO, m_mouserangeyto);
	DDX_Control(pDX, IDC_MOUSERANGEYFROM, m_mouserangeyfrom);
	DDX_Control(pDX, IDC_MOUSERANGEXTO, m_mouserangexto);
	DDX_Control(pDX, IDC_MOUSERANGEXFROM, m_mouserangexfrom);
	DDX_Control(pDX, IDC_LARGERANGEY, m_clargerangey);
	DDX_Control(pDX, IDC_LARGERANGEX, m_clargerangex);
	DDX_Control(pDX, IDC_SHOWRz, m_showRz);
	DDX_Control(pDX, IDC_SHOWRy, m_showRy);
	DDX_Control(pDX, IDC_SHOWRa, m_showRa);
	DDX_Control(pDX, IDC_SHOWRzDANWEI, m_showRzdanwei);
	DDX_Control(pDX, IDC_SHOWRyDANWEI, m_showRydanwei);
	DDX_Control(pDX, IDC_SHOWRaDANWEI, m_showRadanwei);
	DDX_Control(pDX, IDC_SHOWTHRESHOLD, m_showthreshold);
	DDX_Control(pDX, IDC_SLIDERTHRESHOLD, m_sliderthreshold);
	DDX_Control(pDX, IDC_BUTTONGOBACK, m_buttongoback);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_STATICPIANZHIXRANGEY, m_pianzhirangey);
	DDX_Control(pDX, IDC_STATICPIANZHIXRANGEX, m_pianzhirangex);
	DDX_Control(pDX, IDC_SPINPIANZHIY, m_spinpianzhiy);
	DDX_Control(pDX, IDC_SPINPIANZHIX, m_spinpianzhix);
	DDX_Control(pDX, IDC_SHOWZMINDANWEI, m_showzmindanwei);
	DDX_Control(pDX, IDC_SHOWZMAXDANWEI, m_showzmaxdanwei);
	DDX_Control(pDX, IDC_SHOWYDANWEI, m_showydanwei);
	DDX_Control(pDX, IDC_SHOWXDANWEI, m_showxdanwei);
	DDX_Control(pDX, IDC_BUTTONSTOPSCAN, m_buttonstopscan);
	DDX_Control(pDX, IDC_BUTTONSTARTSCAN, m_buttonstartscan);
	DDX_Control(pDX, IDC_BUTTONBMPCAPTURE, m_buttonbmpcapture);
	DDX_Control(pDX, IDC_SPINRANGESMALLY, m_spinrangesmally);
	DDX_Control(pDX, IDC_SPINRANGESMALLX, m_spinrangesmallx);
	DDX_Control(pDX, IDC_SPINRANGELARGEY, m_spinrangelargey);
	DDX_Control(pDX, IDC_SPINRANGELARGEX, m_spinrangelargex);
	DDX_Control(pDX, IDC_SHOWZMIN, m_showzmin);
	DDX_Control(pDX, IDC_SHOWZMAX, m_showzmax);
	DDX_Control(pDX, IDC_SHOWY, m_showy);
	DDX_Control(pDX, IDC_SHOWX, m_showx);
	DDX_Control(pDX, IDC_SLIDERTIME, m_slidertime);
	DDX_Control(pDX, IDC_SHOWTIME, m_showtime);
	DDX_Text(pDX, IDC_LARGERANGEX, m_largerangex);
	DDV_MinMaxInt(pDX, m_largerangex, 400, 4000);
	DDX_Text(pDX, IDC_LARGERANGEY, m_largerangey);
	DDV_MinMaxInt(pDX, m_largerangey, 400, 4000);
	DDX_Radio(pDX, IDC_RADIORANGE, m_radiorange);
	DDX_Radio(pDX, IDC_RADIOBMPSIZE, m_radiobmpsize);
	DDX_Text(pDX, IDC_SMALLRANGEX, m_smallrangex);
	DDV_MinMaxInt(pDX, m_smallrangex, 2, 20);
	DDX_Text(pDX, IDC_SMALLRANGEY, m_smallrangey);
	DDV_MinMaxInt(pDX, m_smallrangey, 2, 20);
	DDX_Check(pDX, IDC_CHECKPIANZHI, m_enablepianzhiflag);
	DDX_Text(pDX, IDC_PIANZHIX, m_pianzhix);
	DDX_Text(pDX, IDC_PIANZHIY, m_pianzhiy);
	DDX_Check(pDX, IDC_MOUSESELECT, m_mouseselect);
	DDX_Text(pDX, IDC_SELECTTIMES, m_selecttimes);
	DDX_Check(pDX, IDC_SELECTSCANTIME, m_selectscantime);
	DDX_Radio(pDX, IDC_XDIRECTION, m_direction);
	DDX_Check(pDX, IDC_CHECKSINGLESCAN, m_checksinglescan);
	DDX_Radio(pDX, IDC_SINGLEXSCAN, m_singlescan);
	//}}AFX_DATA_MAP
	
}


BEGIN_MESSAGE_MAP(CMeasure, CDialog)
//{{AFX_MSG_MAP(CMeasure)
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_RADIORANGE, OnRadiosmallrange)
ON_BN_CLICKED(IDC_RADIORANGE2, OnRadiolargerange)
ON_BN_CLICKED(IDC_BUTTONBMPCAPTURE, OnButtonbmpcapture)
ON_BN_CLICKED(IDC_BUTTONSTARTSCAN, OnButtonstartscan)
ON_BN_CLICKED(IDC_BUTTONSTOPSCAN, OnButtonstopscan)
ON_WM_PAINT()
ON_WM_TIMER()
ON_BN_CLICKED(IDC_CHECKPIANZHI, OnCheckpianzhi)
ON_BN_CLICKED(IDC_BUTTONGOBACK, OnButtongoback)
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_BN_CLICKED(IDC_MOUSESELECT, OnMouseselect)
ON_BN_CLICKED(IDC_SELECTSCANTIME, OnSelectscantime)
ON_BN_CLICKED(IDC_SINGLEXSCAN, OnSinglexscan)
ON_BN_CLICKED(IDC_SINGLEYSCAN, OnSingleyscan)
ON_BN_CLICKED(IDC_CHECKSINGLESCAN, OnChecksinglescan)
ON_BN_CLICKED(IDC_XDIRECTION, OnXdirection)
ON_BN_CLICKED(IDC_YDIRECTION, OnYdirection)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasure message handlers

BOOL CMeasure::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//把对话框置于屏幕的中间
	CenterWindow(CWnd::GetDesktopWindow());
	
	scanendflag=TRUE;  //设置扫描结束标志为真
	redrawflag=FALSE;  //设置重画标志为假
	
	//设置各个控件的初始值
	m_largerangex = 4000;
	m_largerangey = 4000;
	m_radiorange = 1;
	m_radiobmpsize = 0;
	m_smallrangex = 5;
	m_smallrangey = 5;
	m_mouseselect=0;
	checkflag=TRUE;
	smalldataexchangeflag=TRUE;
	largedataexchangeflag=TRUE;
	rightstart=FALSE;
	m_pianzhix=0;
	m_pianzhiy=0;
	m_enablepianzhiflag=FALSE;
	m_checksinglescan=-1;
	m_singlescan=-1;
	UpdateData(FALSE);
	SelectScanTimeFlag=FALSE;
	SingleXFlag=FALSE;
	SingleYFlag=FALSE;
    
	//设置显示时间的初始值
	timelast=time=0; // 2005.10.30 //2017USB将30修改为2
	wsprintf(timechar," %d     ",time);
	m_showtime.SetWindowText(timechar);
    
	//设置spin控件的范围
	//	m_spinrangesmallx.SetRange(2,20);
	//	m_spinrangesmally.SetRange(2,20);
	m_spinrangelargex.SetRange(400,4000);
	m_spinrangelargey.SetRange(400,4000);
	m_spinselecttimes.SetRange(1,1000);
	
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPINKX))->SetRange(-100, 100); //20140506
	SetDlgItemInt(IDC_KX, 0);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPINKY))->SetRange(-100, 100); //20140506
	SetDlgItemInt(IDC_KY, 0);	

	((CSpinButtonCtrl*)GetDlgItem(IDC_SPINKZ))->SetRange(-10, 6); //2017USB
	//SetDlgItemInt(IDC_KZ, 4);
	CString strNumber="";
	strNumber.Format("%1.1f",0.6); //对比度系数默认值
	SetDlgItemText(IDC_KZ,strNumber);

	//设置时间滑块的范围、移动步长和初始值
	m_slidertime.SetRange(0,900,TRUE); // 2005.10.30 //
	m_slidertime.SetTicFreq(1);
	m_slidertime.SetPos(time);
	
	//设置亮度阈值的初始值
	lightnessthresholdlast=lightnessthreshold=80;
	sprintf(thresholdchar,"%1.2f     ",(float)(lightnessthreshold/100.0));
	m_showthreshold.SetWindowText(thresholdchar);
	
	//设置亮度阈值滑块的范围、移动步长和初始值
	m_sliderthreshold.SetRange(60,100,TRUE);
	m_sliderthreshold.SetTicFreq(1);
	m_sliderthreshold.SetPos(lightnessthreshold);
	
	//设置编辑框能使 0708
	SetStatus(1, FALSE, 10, IDC_SMALLRANGEX, IDC_SMALLRANGEY, IDC_SELECTTIMES, IDC_SINGLEXSCANTXT,
		IDC_SINGLEYSCANTXT, IDC_SINGLEXSCAN, IDC_SINGLEYSCAN, IDC_YDIRECTION, IDC_PIANZHIX, IDC_PIANZHIY);
	SetStatus(1, TRUE, 3, IDC_LARGERANGEY, IDC_LARGERANGEX, IDC_XDIRECTION);
	
	m_direction=0;
	UpdateData(FALSE);
	
	//设置四个按钮的能使
	m_buttonstartscan.EnableWindow(TRUE);
	m_buttongoback.EnableWindow(TRUE);
	m_buttonbmpcapture.EnableWindow(FALSE);
	m_buttonstopscan.EnableWindow(FALSE);
	
	
	//写两个BMP图的信息头和调色板
	//写smallbmp的信息头
	smallbmpinfoheader.biSize=40;
	smallbmpinfoheader.biWidth=100;
	smallbmpinfoheader.biHeight=100;
	smallbmpinfoheader.biPlanes=1;
	smallbmpinfoheader.biBitCount=8;
	smallbmpinfoheader.biCompression=0;
	smallbmpinfoheader.biSizeImage=10000;  //图像采集得到后写图像X方向代表的纳米数      //将采集的图像的有关信息存入BMP信息头中，因为BMP信息头的有些参数可以自由改动而不会使BMP图失效
	smallbmpinfoheader.biXPelsPerMeter=3780; //图像采集得到后写图像Y方向代表的纳米数
	smallbmpinfoheader.biYPelsPerMeter=3780; //图像采集得到后写图像采集时的最小电压值
	smallbmpinfoheader.biClrUsed=0;           
	smallbmpinfoheader.biClrImportant=0;   //图像采集得到后写图像采集时的最大电压值
	//写largebmp的信息头
	largebmpinfoheader.biSize=40;
	largebmpinfoheader.biWidth=400;
	largebmpinfoheader.biHeight=400;
	largebmpinfoheader.biPlanes=1;
	largebmpinfoheader.biBitCount=8;
	largebmpinfoheader.biCompression=0;
	largebmpinfoheader.biSizeImage=160000; //图像采集得到后写图像X方向代表的纳米数
	largebmpinfoheader.biXPelsPerMeter=3780; //图像采集得到后写图像Y方向代表的纳米数
	largebmpinfoheader.biYPelsPerMeter=3780;  //图像采集得到后写图像采集时的最小电压值
	largebmpinfoheader.biClrUsed=0;            
	largebmpinfoheader.biClrImportant=0;      //图像采集得到后写图像采集时的最大电压值
	
	
	//写调色板
	/*
	for (i=0;i<=255;i++)
	{
	bmpcolorpalette[i].rgbRed=i;
	bmpcolorpalette[i].rgbGreen=i*0.7; // 原来值R＝G＝B＝i //
	bmpcolorpalette[i].rgbBlue=i*0.3; // 2005年9月23日 //
	bmpcolorpalette[i].rgbReserved=0;
	}
	
	*/
	
	/////////////////////////////// 20140504
	for (i=0;i<128;i++)
	{
		bmpcolorpalette[i].rgbRed=2*i;  
		bmpcolorpalette[i].rgbGreen=i;
		bmpcolorpalette[i].rgbBlue=0;
		bmpcolorpalette[i].rgbReserved=0;
	}
	
	for (i=128;i<=255;i++)
	{
		bmpcolorpalette[i].rgbRed=255;  
		bmpcolorpalette[i].rgbGreen=i;
		bmpcolorpalette[i].rgbBlue=2*(i-128)+1;
		bmpcolorpalette[i].rgbReserved=0;
	}
	//////////////////////////////// 20140504
	
	//设置小图的数据
	for (i=0;i<100;i++)
	{
		for (j=0;j<100;j++)
		{
			smallbmpbits[i][j]=192;
			smallbmpdata[i][j]=(float)(3.7647);
		}
	}
    //设置大图的数据
	for (i=0;i<400;i++)
	{
		for (j=0;j<400;j++)
		{
			largebmpbits[i][j]=192;
			largebmpdata[i][j]=(float)(3.7647);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMeasure::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	
	//时间滚动条
	CSliderCtrl* slidertime=(CSliderCtrl*)GetDlgItem(IDC_SLIDERTIME);
	time=slidertime->GetPos();
    //更新时间滚动条显示值
	sprintf(timechar," %d     ",time);
	m_showtime.SetWindowText(timechar);
	
	//亮度阈值滚动条
	CSliderCtrl* sliderthreshold=(CSliderCtrl*)GetDlgItem(IDC_SLIDERTHRESHOLD);
	lightnessthreshold=sliderthreshold->GetPos();
    //更新亮度阈值滚动条显示值
	sprintf(thresholdchar," %1.2f     ",(float)(lightnessthreshold/100.0));
	m_showthreshold.SetWindowText(thresholdchar);
	
}

void CMeasure::OnRadiosmallrange() 
{
	// 0708
	SetStatus(1, FALSE, 2, IDC_LARGERANGEX, IDC_LARGERANGEY);
	SetStatus(1, TRUE, 2, IDC_SMALLRANGEX, IDC_SMALLRANGEY);
	
	smalldataexchangeflag=TRUE;
	largedataexchangeflag=FALSE;	
}

void CMeasure::OnRadiolargerange() 
{
	// 0708
	SetStatus(1, TRUE, 2, IDC_LARGERANGEX, IDC_LARGERANGEY);
	SetStatus(1, FALSE, 2, IDC_SMALLRANGEX, IDC_SMALLRANGEY);
	
	smalldataexchangeflag=FALSE;
	largedataexchangeflag=TRUE;	
}

void CMeasure::OnButtonbmpcapture() 
{
	CMicroscopeApp* app =(CMicroscopeApp*)AfxGetApp();
	app->Bmpcapture();
}

const LONG PerLsbVolt = (LONG)((10000.0/8192)*1000.0);

bool CMeasure::ReadDataFromAD(HANDLE hDevice, PCI8620_PARA_AD para, float &volt)
{
/*	//初始化并且清空数据
	if(!PCI8620_InitDeviceProAD(hDevice, &para)) 
	{
		MessageBox(_T("不明确的初始化错误..."),
			_T(""), MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
	//启动AD			
	PCI8620_StartDeviceProAD(hDevice);
	//非空采集数据，并且只采样一个数据（为了减小噪声，可以采集多个数据，然后取中值）
	PCI8620_ReadDeviceProAD_Npt(hDevice, &PCI8620_ADBuffer, 1, &nRealNum);
	DWORD addata = PCI8620_ADBuffer;
	// 	if ((PCI8620_ADBuffer&0x1FFF) > 4095)
	// 	{
	// 		PCI8620_ADBuffer = 4095;
	// 	}
	// 	else if ((PCI8620_ADBuffer&0x1FFF) < 2048)
	// 	{
	// 		PCI8620_ADBuffer = 2048;
	// 	}
	
	//计算实际采样电压值	
	volt = (float)(PerLsbVolt * (PCI8620_ADBuffer & 0x1FFF) - 5000000.0) / 1000000.0;
*/
  return true;

}



void CMeasure::OnButtonstartscan() 
{
//	Para.ADMode = PCI8620_ADMODE_SEQUENCE; //连续采样
//	Para.FirstChannel = 0;                     //置首通道为0
//	Para.LastChannel = 0;                      //置末通道为0
	Para.Frequency = 160000;                    //置AD采样频率为160KHz
//	Para.InputRange = PCI8620_INPUT_N5000_P5000mV; //
//	Para.Gains = PCI8620_GAINS_1MULT;
//	Para.TriggerMode = PCI8620_TRIGMODE_SOFT;
    //******************************pci8620*****************************************/

	//******************************2017USB*****************************************//
	if(usb_open==0)//如果设备未打开，则打开设备
	{
		if(!OpenUsbV20())
			usb_open=1;
		else
		{
			MessageBox(_T("打开设备失败..."),
			_T(""), MB_ICONEXCLAMATION | MB_OK);
			return;
		}
	}
	//******************************2017USB*****************************************//

	if (scanendflag==TRUE)
	{
		
		//更新对话框中各控件的数值
		if (UpdateData(TRUE)==0)
			return;
		
		//更新扫描时间和亮度阈值这两个滑块当时的数据，因为它们记录的数据不能用UpdateData函数来更新
		timelast=time;
		lightnessthresholdlast=lightnessthreshold;
		
		//设置当前图像的初始z最大和z最小值,以及上一幅图像的初始z最大值和z最小值
		zmax=(float)5.0;
		zmin=(float)0.0;
		oldzmax=(float)2.9;  //// 原来值为2.0 
		oldzmin=(float)1.5;  //// 原来值为1.0
		pianzhivoltagex=0.0;
		pianzhivoltagey=0.0;
		
		//判断扫描范围对应的扫描图像大小
		if (m_radiorange==0)
		{
			if(m_radiobmpsize==1)
			{
				if(m_smallrangex<4||m_smallrangey<4)
				{
					MessageBox(_T("2 ～ 20纳米档：扫描范围小于4纳米时请选择扫描200×200象素的图像"),
						_T("D/A分辨率限制无法扫描400×400象素的图像"), MB_ICONEXCLAMATION | MB_OK);
					return;
				}
			}
		}
		if (m_radiorange==1)
		{
			if(m_radiobmpsize==1)
			{
				if(m_largerangex<400||m_largerangey<400)
				{
					MessageBox(_T("400 ～ 4000纳米档：扫描范围小于400纳米时请选择扫描200×200象素的图像"),
						_T("D/A分辨率限制无法扫描400×400象素的图像"), MB_ICONEXCLAMATION | MB_OK);
					return;
				}
				
			}
		}
		
		//获取计算机高精度计时器的频率
		if(QueryPerformanceFrequency(&litmp)==0)
		{
			
			MessageBox(_T("此计算机硬件不支持高精度计数器"),
				_T("硬件不支持"), MB_ICONEXCLAMATION | MB_OK);
			
			return;
		}
		
		freq=(double)(litmp.QuadPart);
		
		//计算while循环所需的时间
		LONGLONG begintime,endtime;
		QueryPerformanceCounter(&litmp);
		begintime=litmp.QuadPart;
		while(1)
		{
			QueryPerformanceCounter(&litmp);
			endtime=litmp.QuadPart;
			if ((double)(endtime-begintime)/freq>0.0)
				break;
		}
		QueryPerformanceCounter(&litmp);
		endtime=litmp.QuadPart;
		systemdelay=(double)(endtime-begintime)/freq;
		
		//判断样品偏置输入值是否正确，若正确就计算偏移电压和像元电压
		if (m_radiorange==0)
		{
			if (((m_smallrangex+m_pianzhix)>20||m_pianzhix<0||(m_smallrangey+m_pianzhiy)>20||m_pianzhiy<0)&&SingleXFlag==FALSE&&SingleYFlag==FALSE)
			{
				MessageBox(_T("请重新输入样品偏移值"),
					_T("样品偏移输入值错误"), MB_ICONEXCLAMATION | MB_OK);
				
				//如果扫描范围有错误，选中check控件，使偏移edit控件使能
				m_enablepianzhiflag=TRUE;
				checkflag=FALSE;
				UpdateData(FALSE);
				checkflag=TRUE;
				CEdit* editpianzhix=(CEdit*)GetDlgItem(IDC_PIANZHIX);
				editpianzhix->EnableWindow(TRUE);
				CEdit* editpianzhiy=(CEdit*)GetDlgItem(IDC_PIANZHIY);
				editpianzhiy->EnableWindow(TRUE);
				//设置偏置静态文本控件的显示范围和可以输入的最大最小值和spin控件的值
				sprintf(timechar,"0 ～ %d     ",(20-m_smallrangex));
				m_pianzhirangex.SetWindowText(timechar);
				
				sprintf(timechar,"0 ～ %d     ",(20-m_smallrangey));
				m_pianzhirangey.SetWindowText(timechar);
				
				m_spinpianzhix.SetRange(0,20-m_smallrangex);
				m_spinpianzhiy.SetRange(0,20-m_smallrangey);
				
				return;
			}
			//计算偏移电压和移动每个像元需要的电压
			else
			{
				pianzhivoltagex=4.9975586*m_pianzhix/20.0;
				pianzhivoltagey=4.9975586*m_pianzhiy/20.0;
				
				if (m_radiobmpsize==0)
				{
					movevoltagex=(4.9975586*m_smallrangex/20.0)/99.0;
					movevoltagey=(4.9975586*m_smallrangey/20.0)/99.0;
				}
				if (m_radiobmpsize==1)
				{
					movevoltagex=(4.9975586*m_smallrangex/20.0)/399.0;
					movevoltagey=(4.9975586*m_smallrangey/20.0)/399.0;
				}
			}
		}
		
		if (m_radiorange==1)
		{
			if (((m_largerangex+m_pianzhix)>4000||m_pianzhix<0||(m_largerangey+m_pianzhiy>4000||m_pianzhiy<0))&&SingleXFlag==FALSE&&SingleYFlag==FALSE)
			{
				MessageBox(_T("请重新输入样品偏移值"),
					_T("样品偏移输入值错误"), MB_ICONEXCLAMATION | MB_OK);
				
				//如果扫描范围有错误，选中check控件，使偏移edit控件使能
				m_enablepianzhiflag=TRUE;
				checkflag=FALSE;
				UpdateData(FALSE);
				checkflag=TRUE;
				CEdit* editpianzhix=(CEdit*)GetDlgItem(IDC_PIANZHIX);
				editpianzhix->EnableWindow(TRUE);
				CEdit* editpianzhiy=(CEdit*)GetDlgItem(IDC_PIANZHIY);
				editpianzhiy->EnableWindow(TRUE);
				//设置偏置静态文本控件的显示范围和可以输入的最大最小值和spin控件的值
				sprintf(timechar,"0 ～ %d     ",(4000-m_largerangex));
				m_pianzhirangex.SetWindowText(timechar);
				
				sprintf(timechar,"0 ～ %d     ",(4000-m_largerangey));
				m_pianzhirangey.SetWindowText(timechar);
				
				m_spinpianzhix.SetRange(0,4000-m_largerangex);
				m_spinpianzhiy.SetRange(0,4000-m_largerangey);
				
				return;
			}
			//计算偏移电压和移动每个像元需要的电压
			else 
			{
				
				if (m_radiobmpsize==0)
				{
					movevoltagex=(4.9975586*m_largerangex/4000.0)/99.0;
					for(i=0;i<m_pianzhix*100/4000;i++)
					{
						pianzhivoltagex=pianzhivoltagex+(4.9975586/99.0);
						pianzhivoltagex=pianzhivoltagex-pianzhivoltagex*(4.9975586/99.0)*ceramicscoefficient;
					}
					
					movevoltagey=(4.9975586*m_largerangey/4000.0)/99.0;
					for(i=0;i<m_pianzhiy*100/4000;i++)
					{
						pianzhivoltagey=pianzhivoltagey+(4.9975586/99.0);
						pianzhivoltagey=pianzhivoltagey-pianzhivoltagey*(4.9975586/99.0)*ceramicscoefficient;
					}
				}
				if (m_radiobmpsize==1)
				{
					movevoltagex=(4.9975586*m_largerangex/4000.0)/399.0;
					for(i=0;i<m_pianzhix*400/4000;i++)
					{
						pianzhivoltagex=pianzhivoltagex+(4.9975586/399.0);
						pianzhivoltagex=pianzhivoltagex-pianzhivoltagex*(4.9975586/399.0)*ceramicscoefficient;
					}
					
					movevoltagey=(4.9975586*m_largerangey/4000.0)/399.0;
					for(i=0;i<m_pianzhiy*400/4000;i++)
					{
						pianzhivoltagey=pianzhivoltagey+(4.9975586/399.0);
						pianzhivoltagey=pianzhivoltagey-pianzhivoltagey*(4.9975586/399.0)*ceramicscoefficient;
					}
				}
			}
			
		}
		
		//传递压电陶瓷非线性校正系数
		ceramicscoefficient=((CMicroscopeApp*) AfxGetApp())->ceramics;
		
		if(SingleXFlag==FALSE&&SingleYFlag==FALSE)
		{
			//设置时间变量，在WM_TIMER消息中进行采样和画图
			if (!SetTimer(1,5,NULL))
			{
				MessageBox(_T("此窗口没有足够的定时器"),
					_T("请重新启动计算机"), MB_ICONEXCLAMATION | MB_OK);
				
				return ;
			}
		}
		
		//置探针到偏移位置处
		//	int datahigh,datalow;
		//移动X方向，使之到达偏移位置处，定义D/A1为x方向
		if (pianzhivoltagex!=0.0)
		{
			//******************************2017USB*****************************************//
			DAdata=(WORD)(((pianzhivoltagex*1000.0)*4096)/4250.0); //由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
			DASingleOutV20(XDIRECT,DAdata);
			//******************************2017USB*****************************************//
		}
		
		//移动Y方向，使之到达偏移位置处，定义D/A2为y方向
		if (pianzhivoltagey!=0.0)
		{	
			//******************************2017USB*****************************************//
			DAdata=(WORD)(((pianzhivoltagey*1000.0)*4096)/4250.0);//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
			DASingleOutV20(YDIRECT,DAdata);
			//******************************2017USB*****************************************//
		}
		
		//设置初始参数值
		redrawflag=FALSE;
		if(SingleXFlag==FALSE&&SingleYFlag==FALSE)
			Invalidate();
		settimerflag=TRUE;
		quickstopflag=FALSE;
		pianzhiflag=FALSE;
		rowscantime=0;
		bmpscantime=0;
		finishtimes=0;
		sprintf(timechar,"%d",finishtimes);
		m_finishscantimes.SetWindowText(timechar);
		scantimetemp=0;
		m_bLMouseDown=FALSE;
		
		if(SingleXFlag==FALSE&&SingleYFlag==FALSE)
		{
			oldlargerangex=m_largerangex;
			oldsmallrangex=m_largerangex;
			oldpianzhix=m_pianzhix;
			oldpianzhiy=m_pianzhiy;
		}
		
		//设置显示x，y范围的static控件的显示内容及其单位
		if (m_radiorange==0)
		{
			sprintf(timechar," %d     ",m_smallrangex);
			m_showx.SetWindowText(timechar);
			sprintf(timechar,"nm");
			m_showxdanwei.SetWindowText(timechar);
			m_showydanwei.SetWindowText(timechar);
			sprintf(timechar," %d     ",m_smallrangey);
			m_showy.SetWindowText(timechar);
		}
		if (m_radiorange==1)
		{
			sprintf(timechar," %d     ",m_largerangex);
			m_showx.SetWindowText(timechar);
			sprintf(timechar,"nm");
			m_showxdanwei.SetWindowText(timechar);
			m_showydanwei.SetWindowText(timechar);
			sprintf(timechar," %d     ",m_largerangey);
			m_showy.SetWindowText(timechar);
		}
		//设置显示z最大和最小及其单位的static控件的显示内容为空
		sprintf(timechar," ");
		m_showzmax.SetWindowText(timechar);
		m_showzmaxdanwei.SetWindowText(timechar);
		m_showzmindanwei.SetWindowText(timechar);
		m_showzmin.SetWindowText(timechar);
		
		//设置粗糙度Ra,Ry和Rz值及其单位的static控件的显示内容为空
		sprintf(timechar," ");
		m_showRa.SetWindowText(timechar);
		m_showRy.SetWindowText(timechar);
		m_showRz.SetWindowText(timechar);
		m_showRadanwei.SetWindowText(timechar);
		m_showRydanwei.SetWindowText(timechar);
		m_showRzdanwei.SetWindowText(timechar);
		
		
		//设置四个按钮的能使
		m_buttongoback.EnableWindow(FALSE);
		m_buttonbmpcapture.EnableWindow(FALSE);
		m_buttonstopscan.EnableWindow(TRUE);
		m_buttonstartscan.EnableWindow(FALSE);
		
		// 07/08 使用SetStatus函数来简化控件操作
		SetStatus(1, FALSE, 9, IDC_LARGERANGEX, IDC_LARGERANGEY, IDC_SMALLRANGEX, 
			IDC_SMALLRANGEY, IDC_PIANZHIX, IDC_PIANZHIY, IDC_SELECTTIMES,
			IDC_SINGLEXSCANTXT, IDC_SINGLEYSCANTXT);
		SetStatus(2, FALSE, 12, IDC_RADIOBMPSIZE, IDC_RADIOBMPSIZE2, IDC_RADIORANGE, 
			IDC_RADIORANGE2, IDC_CHECKPIANZHI, IDC_MOUSESELECT, IDC_SELECTSCANTIME,
			IDC_CHECKSINGLESCAN, IDC_XDIRECTION, IDC_YDIRECTION, IDC_SINGLEXSCAN,IDC_SINGLEYSCAN);
		
		//禁止对话框上的所有控件
		m_slidertime.EnableWindow(FALSE);
		m_sliderthreshold.EnableWindow(FALSE);
		
		if(SingleXFlag==TRUE||SingleYFlag==TRUE)
		{
			//开始传送数据 
			singlescannerdlg.singledelaytime=delaytime;
			singlescannerdlg.singletime=time;
			//		singlescannerdlg.singlevoltagex=voltagex;
			//		singlescannerdlg.singlevoltagey=voltagey;
			singlescannerdlg.singlepianzhivoltagex=pianzhivoltagex;
			singlescannerdlg.singlepianzhivoltagey=pianzhivoltagey;
			singlescannerdlg.singlepianzhix=m_pianzhix;
			singlescannerdlg.singlepianzhiy=m_pianzhiy;
			singlescannerdlg.singlemovevoltagex=movevoltagex;
			singlescannerdlg.singlemovevoltagey=movevoltagey;
			singlescannerdlg.radiobmpsize=m_radiobmpsize;
			singlescannerdlg.direction=m_direction;
			singlescannerdlg.singlelightnessthreshold=lightnessthreshold;
			singlescannerdlg.singleyold=yold;
			singlescannerdlg.singlexold=xold;
			singlescannerdlg.singleceramicscoefficient=ceramicscoefficient;
			singlescannerdlg.DoModal();
		}
  }
  else
	  MessageBox(_T("上次扫描还未完成"),
	  _T("请稍等"), MB_ICONEXCLAMATION | MB_OK);
  
}

void CMeasure::OnButtonstopscan() 
{
	
	settimerflag=FALSE;
	redrawflag=TRUE;
	quickstopflag=TRUE;
	pianzhiflag=TRUE;
	
	//设置对话框上按钮的能使
	m_buttongoback.EnableWindow(TRUE);	
	m_buttonstartscan.EnableWindow(TRUE);
	m_buttonstopscan.EnableWindow(FALSE);
	
	SetStatus(2, TRUE, 10, IDC_RADIOBMPSIZE, IDC_RADIOBMPSIZE2,
		IDC_RADIORANGE, IDC_RADIORANGE2, IDC_MOUSESELECT, IDC_SELECTSCANTIME, 
		IDC_CHECKSINGLESCAN, IDC_XDIRECTION, IDC_YDIRECTION, IDC_CHECKPIANZHI);
	
	//恢复对话框上控件的能使
    m_slidertime.EnableWindow(TRUE);
	m_sliderthreshold.EnableWindow(TRUE);
	CEdit* selecttimes=(CEdit*)GetDlgItem(IDC_SELECTTIMES);
	selecttimes->EnableWindow(TRUE);	
	
	if (m_radiorange==0)
	{
		CEdit* editsmallrangey=(CEdit*)GetDlgItem(IDC_SMALLRANGEY);
		editsmallrangey->EnableWindow(TRUE);
		CEdit* editsmallrangex=(CEdit*)GetDlgItem(IDC_SMALLRANGEX);
		editsmallrangex->EnableWindow(TRUE);
	}
	if (m_radiorange==1)
	{
		CEdit* editlargerangex=(CEdit*)GetDlgItem(IDC_LARGERANGEX);
		editlargerangex->EnableWindow(TRUE);
		CEdit* editlargerangey=(CEdit*)GetDlgItem(IDC_LARGERANGEY);
		editlargerangey->EnableWindow(TRUE);
	}
	
	m_enablepianzhiflag=FALSE;
	checkflag=FALSE;
	UpdateData(FALSE);
	checkflag=TRUE;
	//**************************************pci2003**********************************************//
    /*2017USB	//2014/7 pci2003--->pci8620
	PCI8620_ReleaseDevice(hDevice);//释放设备对象所占用的系统资源及设备对象自身 20150212，为了停止扫描时X和Y电压归零	
 	//**************************************pci2003**********************************************/

	if(SingleXFlag==TRUE||SingleYFlag==TRUE)
		Invalidate();
	m_checksinglescan=-1;
	m_singlescan=-1;
	SingleXFlag=FALSE;
	SingleYFlag=FALSE;
	m_pianzhix=oldpianzhix;
	m_pianzhiy=oldpianzhiy;
	UpdateData(FALSE);
}

void CMeasure::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	///每次按下"停止扫描"后显示最后一次扫描结果
	if (redrawflag==TRUE)
	{
		///小图的显示
		if (m_radiobmpsize==0)
		{
			HDIB hsmallDIB;
			LPSTR psmallDIB;
			CPalette* smallpalDIB;
			hsmallDIB = NULL;
			smallpalDIB = NULL;
			
			if (smallpalDIB != NULL)
			{
				delete smallpalDIB;
				smallpalDIB = NULL;
			}
			
			hsmallDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, 12500);
			if (hsmallDIB == 0)
			{
				return ;
			}
			psmallDIB = (LPSTR) ::GlobalLock((HGLOBAL) hsmallDIB);
			
			::memcpy(psmallDIB,&(smallbmpinfoheader),40);
			::memcpy(psmallDIB+40,bmpcolorpalette,1024);
			::memcpy(psmallDIB+1064,smallbmpbits,10000);
			::GlobalUnlock((HGLOBAL)hsmallDIB);
			// Create copy of palette
			smallpalDIB = new CPalette;
			if (smallpalDIB == NULL)
			{
				// we must be really low on memory
				::GlobalFree((HGLOBAL) hsmallDIB);
				hsmallDIB = NULL;
				return;
			}
			if (::CreateDIBPalette(hsmallDIB, smallpalDIB) == NULL)
			{
				// DIB may not have a palette
				delete smallpalDIB;
				smallpalDIB = NULL;
				return;
			}
			LPSTR    lpsmallDIBHdr;            // Pointer to BITMAPINFOHEADER
			LPSTR    lpsmallDIBBits;           // Pointer to DIB bits
			HPALETTE hsmallPal=NULL;           // Our DIB's palette
			HPALETTE hsmallOldPal=NULL;        // Previous palette
			lpsmallDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hsmallDIB);
			lpsmallDIBBits = ::FindDIBBits(lpsmallDIBHdr);
			if (smallpalDIB != NULL)
			{
				hsmallPal = (HPALETTE) smallpalDIB->m_hObject;
				
				// Select as background since we have
				// already realized in forground if needed
				hsmallOldPal = ::SelectPalette(dc, hsmallPal, TRUE);
			}
			::SetStretchBltMode(dc, COLORONCOLOR);
			::StretchDIBits(dc,                          // hDC
				483,                 // DestX
				180,                  // DestY
				100,            // nDestWidth
				100,           // nDestHeight
				0,                // SrcX
				0,                 // SrcY
				100,           // wSrcWidth
				100,          // wSrcHeight
				lpsmallDIBBits,                      // lpBits
				(LPBITMAPINFO)lpsmallDIBHdr,         // lpBitsInfo
				DIB_RGB_COLORS,                 // wUsage
				SRCCOPY);                       // dwROP
			::GlobalUnlock((HGLOBAL) hsmallDIB);
			if (hsmallOldPal != NULL)
			{
				::SelectPalette(dc, hsmallOldPal, TRUE);
			}
			
			if (hsmallDIB != NULL)
			{
				::GlobalFree((HGLOBAL) hsmallDIB);
			}
			if (smallpalDIB != NULL)
			{
				delete smallpalDIB;
			}
		}
		
		//大图的显示	
		if (m_radiobmpsize==1)
		{
			HDIB hlargeDIB;
			LPSTR plargeDIB;
			CPalette* largepalDIB;
			hlargeDIB = NULL;
			largepalDIB = NULL;
			
			if (largepalDIB != NULL)
			{
				delete largepalDIB;
				largepalDIB = NULL;
			}
			
			hlargeDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, 200000);
			if (hlargeDIB == 0)
			{
				return ;
			}
			plargeDIB = (LPSTR) ::GlobalLock((HGLOBAL) hlargeDIB);
			
			::memcpy(plargeDIB,&(largebmpinfoheader),40);
			::memcpy(plargeDIB+40,bmpcolorpalette,1024);
			::memcpy(plargeDIB+1064,largebmpbits,160000);
			::GlobalUnlock((HGLOBAL)hlargeDIB);
			// Create copy of palette
			largepalDIB = new CPalette;
			if (largepalDIB == NULL)
			{
				// we must be really low on memory
				::GlobalFree((HGLOBAL) hlargeDIB);
				hlargeDIB = NULL;
				return;
			}
			if (::CreateDIBPalette(hlargeDIB, largepalDIB) == NULL)
			{
				// DIB may not have a palette
				delete largepalDIB;
				largepalDIB = NULL;
				return;
			}
			LPSTR    lplargeDIBHdr;            // Pointer to BITMAPINFOHEADER
			LPSTR    lplargeDIBBits;           // Pointer to DIB bits
			HPALETTE hlargePal=NULL;           // Our DIB's palette
			HPALETTE hlargeOldPal=NULL;        // Previous palette
			lplargeDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hlargeDIB);
			lplargeDIBBits = ::FindDIBBits(lplargeDIBHdr);
			if (largepalDIB != NULL)
			{
				hlargePal = (HPALETTE) largepalDIB->m_hObject;
				
				// Select as background since we have
				// already realized in forground if needed
				hlargeOldPal = ::SelectPalette(dc, hlargePal, TRUE);
			}
			::SetStretchBltMode(dc, COLORONCOLOR);
			::StretchDIBits(dc,                          // hDC
				332,                 // DestX
				20,                  // DestY
				400,            // nDestWidth
				400,           // nDestHeight
				0,                // SrcX
				0,                 // SrcY
				400,           // wSrcWidth
				400,          // wSrcHeight
				lplargeDIBBits,                      // lpBits
				(LPBITMAPINFO)lplargeDIBHdr,         // lpBitsInfo
				DIB_RGB_COLORS,                 // wUsage
				SRCCOPY);                       // dwROP
			::GlobalUnlock((HGLOBAL) hlargeDIB);
			if (hlargeOldPal != NULL)
			{
				::SelectPalette(dc, hlargeOldPal, TRUE);
			}
			
			if (hlargeDIB != NULL)
			{
				::GlobalFree((HGLOBAL) hlargeDIB);
			}
			if (largepalDIB != NULL)
			{
				delete largepalDIB;
			}
		}
}

}

void CMeasure::OnTimer(UINT nIDEvent) 
{
	//	CSinglescanner singlescannerdlg;	
	scanendflag=FALSE;
	KillTimer(1);
	CClientDC dc(this);
	dc.SetMapMode(MM_TEXT);
	
	CTime now=CTime::GetCurrentTime();
	int hourbegin=now.GetHour();
	int minutebegin=now.GetMinute();
	int secondbegin=now.GetSecond();

	float adResult;//2017USB
	//**************************************pci2003**********************************************//
    /*2017USB   LONG PerLsbVolt;    //定义一个足够宽度的整型变量存放单位电压值(mV)  
	PerLsbVolt = (LONG)((10000.0/4096)*1000.0); //取得单位电压值，并以mV为单位将其转换成整型值
    //**************************************pci2003**********************************************/

	//******************************2017USB*****************************************//
	LONG PerLsbVolt;    //定义一个足够宽度的变量存放单位电压值(mW)  
	PerLsbVolt = (LONG)((4350.0/4096)*1000.0); //取得单位电压值，并以mV为单位将其转换成整型值//4350.0为测量后取的值
	//******************************2017USB*****************************************//
	
	
	//*************************************** X方向小图扫描或X方向大图扫描 *****************************************//
	if(m_direction==0)  
	{
	
		//*********************************** X方向小图扫描 ***************************************************//
		if (m_radiobmpsize==0) // X方向小图实时扫描
		{
		//***** 先X方向预扫描10行，不显示图像，仅计算其最大值和最小值，作为第一幅完整扫描图像的基准 20140521
		int m_height = 5;

		if(finishtimes == 0)  
		{
			delaytime=time/10000.0 - 10.0*systemdelay;
//			delaytime=time/10000.0;

			voltagey=pianzhivoltagey;
			for(height=0;height < m_height;height++)
			{
				//写入y方向的电压信号
				voltagey=voltagey+movevoltagey;
				
				voltagey=voltagey-voltagey*movevoltagey*ceramicscoefficient; //校正压电陶瓷的非线性
				//******************************2017USB******************************************//
//	            DAdata=(WORD)(((voltagey*1000.0)*4096)/4250.0);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
		        DAdata=(WORD)(voltagey*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
			    DASingleOutV20(YDIRECT,DAdata);
			    //******************************2017USB******************************************//
						
				voltagex=pianzhivoltagex;
					
				for(width=0;width<100;width++)
				{
					//写入x方向的电压信号
					voltagex=voltagex+movevoltagex; 
						
					voltagex=voltagex-voltagex*movevoltagex*ceramicscoefficient; //校正压电陶瓷的非线性
		            DAdata=(WORD)(voltagex*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
				    DASingleOutV20(XDIRECT,DAdata);
				    //******************************2017USB*******************************************//

			    	QueryPerformanceCounter(&litmp);
				    begin=litmp.QuadPart;
				        while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
								break;
						}

                    ///////////在外面的循环里终止扫描，同时将X和Y压电陶瓷归零 20150209///////////////////////////
					if (quickstopflag==TRUE)
					{

					//******************************2017USB*****************************************//
					voltagex=0;
		            DAdata=(WORD)(voltagex*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
					DASingleOutV20(XDIRECT,DAdata);

					voltagey=0;
		            DAdata=(WORD)(voltagey*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
				 	DASingleOutV20(YDIRECT,DAdata);//
					//******************************2017USB*****************************************//
					break;
					}
                    ///////////在外面的循环里终止扫描，同时将X和Y压电陶瓷归零 20150209///////////////////////////


					    //****************************** AD采样 ****************************************//
					    //******************************2017USB*****************************************//	
					    ADSingleV20(0,&adResult);//从USB采集卡读取0通道数据
					    Volt=(float)(PerLsbVolt*adResult/1000);//计算实际电压值,单位V
					    //******************************2017USB*****************************************//


			        	//****************** X方向和Y方向图像消倾斜处理 ***************************//
						CString m_cstr;
						GetDlgItemText(IDC_KX, m_cstr);
						m_kx = atof(m_cstr)/100.0; //-0.10是预设的经验值
						Volt=(float)(Volt+m_kx*(50-width)/50); // X方向图像倾斜度调节 20140506
						
						GetDlgItemText(IDC_KY, m_cstr);
						m_ky = atof(m_cstr)/100.0; //+0.19是预设的经验值
						Volt=(float)(Volt+m_ky*(50-height)/50); // Y方向图像倾斜度调节 20140506
						
						if(Volt<0)Volt=0;
						////////////////////////////////////////////////////////////////////////////	
					
						smallbmpdata[width][height]=Volt; 
						
						//捕捉外来消息
						if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
						{
							::TranslateMessage(&message);
							::DispatchMessage(&message);
						}
				

					      //////// 预扫描时，也显示扫描行，以免误解为仪器没有启动扫描 /////////
				          if (smallbmpdata[width][height]>2.5)
					      tempbmpbits=2.5;
				          if (smallbmpdata[width][height]<1.0)
					      tempbmpbits=1.0;
			              if (smallbmpdata[width][height]<=2.5&&smallbmpdata[width][height]>=1.0)
					      tempbmpbits=smallbmpdata[width][height];
			              tempindex=(BYTE)(255*(tempbmpbits-1.0)/(2.5-1.0));
				          ////////////////////////////////////////////////////////////////////////////// 20140506新调色板
				          if(tempindex>=128) 
						  {	dc.SetPixelV(width+483,height+180,RGB(255,tempindex,2*(tempindex-128)+1)); // 20140518，原来423,150
						  }
				          if(tempindex<128) 
						  {
				        	dc.SetPixelV(width+483,height+180,RGB(2*tempindex,tempindex,0)); 
						  }
				          ///////////////////////////////////////////////////////////////////////////// 20140506新调色板
	
					}	
				    //恢复x方向的伸长到最初的x方向偏移位置
		     	    voltagex=pianzhivoltagex;
		            DAdata=(WORD)(voltagex*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
				    DASingleOutV20(XDIRECT,DAdata);
				    //******************************2017USB*****************************************//	

			    	QueryPerformanceCounter(&litmp);
				    begin=litmp.QuadPart;
				        while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
								break;
						}
				}	

		        //恢复y方向的伸长到最初的y方向偏移位置
		        voltagey=pianzhivoltagey;
		        DAdata=(WORD)(voltagey*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
		        DASingleOutV20(YDIRECT,DAdata);
		        //******************************2017USB*****************************************//	

				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
			        while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
							break;
						}
        
			
			//********** Process，计算前10行扫描的最大值和最小值 ***********//
			float m_ZMax0 = 0.0, m_ZMin0 = 5.0;
			float m_temp;
			
			for(height = 0; height < m_height; height++)
			{
				for(width = 20; width < 100; width++) //消除每行前20个点的影响
				{
					m_temp = smallbmpdata[width][height];
					if(m_ZMax0 < m_temp)
						m_ZMax0 = m_temp;
					if(m_ZMin0 > m_temp)
						m_ZMin0 = m_temp;
				}
			}

			//*****************2017USB****************************//
			CString m_cstr;
			GetDlgItemText(IDC_KZ, m_cstr);
			float m_kz = atof(m_cstr)/2.0; //m_kz为扫描界面显示的对比系数的一半
			//*****************2017USB****************************//			
        	oldzmin =(float)(m_ZMax0-m_ZMin0)*m_kz+m_ZMin0; //20140521 前10行的最大值和最小值数据，作为第一次完整扫描图像的显示依据、存储依据  
			oldzmax = m_ZMax0;
			
			//设置显示小图的z最大和最小值及其单位
			sprintf(timechar," %1.2f",m_ZMax0);
			m_showzmax.SetWindowText(timechar);
			sprintf(timechar,"V");
			m_showzmaxdanwei.SetWindowText(timechar);
			m_showzmindanwei.SetWindowText(timechar);
			sprintf(timechar," %1.2f",m_ZMin0);
			m_showzmin.SetWindowText(timechar);
			//UpdateData(FALSE);
		}
		//************** X方向小图预扫描10行结束，并计算出最大最小值，作为X方向小图扫描基准，直接跳到X方向的小图实时扫描开始 ********20140521   
		

		
		//************** X方向完整小图实时扫描，如果是第1次扫描，则跳过此段，直接去小图实时扫描；如果是第2次，则进入此段，计算第1次图像的最大值、最小值等，以此类推 *********** 
		if (height==100&&width==100)
		{
			//********* 对刚刚完成完整扫描的图像（定义为第1次图像）的数据进行量化，计算z电压平均值、最大值和最小值 *********//
			zavetemp=0;
			for (height=0;height<100;height++)
			{
				for (width=0;width<100;width++)
				{
					if (smallbmpdata[width][99-height]>oldzmax)
						tempbmpbits=oldzmax;
					if (smallbmpdata[width][99-height]<oldzmin)
						tempbmpbits=oldzmin;
					if (smallbmpdata[width][99-height]<=oldzmax&&smallbmpdata[width][99-height]>=oldzmin)
						tempbmpbits=smallbmpdata[width][99-height];
					
					//****** 对于第1次完整扫描的图像，依据前10行的oldzmax和oldzmin值，将第1次图像的电压值归一化成255等分的灰度值，以便捕获图像或停止扫描时保存第1次图像数据 ******/
					//****** 对于第2次完整扫描的图像，依据第1次图像的oldzmax和oldzmin值，将第2次图像的电压值归一化成255等分的灰度值，以便捕获图像或停止扫描时保存第2次图像数据 ******/
					//****** 以此类推 ******/
					
					//****** 由于电脑的左下角是坐标原点，因此需要对AFM图像进行坐标变换
					smallbmpbits[height][width]=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin)); 
					
					//****** 计算第1次图像的电压平均值，以便在第1次图像扫描一结束时就显示出来
					zavetemp=zavetemp+smallbmpdata[width][height]; 
				}
			}
			zave=(float)(zavetemp/10000.0); //第1次图像的电压平均值，在第1次图像扫描一结束就显示出来
			
			
			//***** 计算第1次图像的最大值和最小值前，先平滑（但不改变图像本身），作为计算第1次图像数据的依据，并传给第2次图像作为其显示依据，20140521 */
			float m_ZMax1 = 0.0, m_ZMin1 = 5.0;
			float m_temp;
			
			for(height = 1; height < 99; height++)
			{
				for(width = 20; width <99; width++) //消除每行前20个点的影响
				{
					m_temp = smallbmpdata[width][height-1] + smallbmpdata[width][height+1] + 
						smallbmpdata[width-1][height] + smallbmpdata[width+1][height] + 
						smallbmpdata[width][height];
					m_temp /= 5.0;
					if(m_ZMax1 < m_temp)
						m_ZMax1 = m_temp;
					if(m_ZMin1 > m_temp)
						m_ZMin1 = m_temp;
				}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////			
			
			//********************* 这些数据，只是传给图像存储之用20140521 ********************//
			zmin = m_ZMin1;   
			zmax = m_ZMax1;	
			//*********************************************************************************//
			
			
			//设置显示第1次完整扫描的小图的z最大值和最小值及其单位
			sprintf(timechar," %1.2f",m_ZMax1); //第1次图像的电压最大值，在第1次图像扫描一结束后、第2次图像扫描过程中一直显示
			m_showzmax.SetWindowText(timechar);
			sprintf(timechar,"V");
			m_showzmaxdanwei.SetWindowText(timechar);
			m_showzmindanwei.SetWindowText(timechar);
			sprintf(timechar," %1.2f",m_ZMin1); //第1次图像的电压最小值，在第1次图像扫描一结束后、第2次图像扫描过程中一直显示
			m_showzmin.SetWindowText(timechar);
			
			
			Ry=(m_ZMax1-m_ZMin1)*(((CMicroscopeApp*)AfxGetApp())->voltagetonanometer);
			Rz=(float)(fabs(m_ZMax1-zave)+fabs(zave-m_ZMin1))*(((CMicroscopeApp*)AfxGetApp())->voltagetonanometer);
			
			Ra=0;
			for (height=0;height<100;height++)
			{
				for (width=0;width<100;width++)
				{
					Ra=Ra+(float)fabs(smallbmpdata[width][height]-zave);
				}
			}
			Ra=(float)(Ra/10000.0)*(((CMicroscopeApp*)AfxGetApp())->voltagetonanometer);
			
			//设置显示小图的Ra,Ry,Rz值及其单位
			sprintf(timechar,"%.1f",Ra);
			m_showRa.SetWindowText(timechar);
			sprintf(timechar,"nm");
			m_showRadanwei.SetWindowText(timechar);
			m_showRydanwei.SetWindowText(timechar);
			m_showRzdanwei.SetWindowText(timechar);
			sprintf(timechar,"%.1f",Ry);
			m_showRy.SetWindowText(timechar);
			sprintf(timechar,"%.1f",Rz);
			m_showRz.SetWindowText(timechar);
			
			//设置捕获图像按钮为能使
			m_buttonbmpcapture.EnableWindow(TRUE);
			
			
			//********* 最后，将oldzmax和oldzmin传给第2次图像，作为第2次图像显示的依据，以此类推 *********//
			//*****************2017USB****************************//
			CString m_cstr;
			GetDlgItemText(IDC_KZ, m_cstr);
			float m_kz = atof(m_cstr)/2.0; 
			//*****************2017USB****************************//	
			oldzmin=(float)(m_ZMax1-m_ZMin1)*m_kz+m_ZMin1; //20140521  
			oldzmax=m_ZMax1;		
		}
		//*********************************************** X方向小图数据计算处理结束 **************************************//
		


		//********************************************** X方向的小图实时扫描正式开始 *************************************//
		voltagey=pianzhivoltagey;

		for(height=0;height<100;height++)
		{
			//写入y方向的电压信号
			voltagey=voltagey+movevoltagey;
						
			voltagey=voltagey-voltagey*movevoltagey*ceramicscoefficient; //校正压电陶瓷的非线性
//	       	DAdata=(WORD)(((voltagey*1000.0)*4096)/4250.0);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
		    DAdata=(WORD)(voltagey*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
			DASingleOutV20(YDIRECT,DAdata);
			//******************************2017USB*****************************************//		
						
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;						
				while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
								break;
						}

		
			voltagex=pianzhivoltagex;

			for(width=0;width<100;width++)
			{
				//写入x方向的电压信号
				voltagex=voltagex+movevoltagex;
				
				voltagex=voltagex-voltagex*movevoltagex*ceramicscoefficient; //校正压电陶瓷的非线性
		        DAdata=(WORD)(voltagex*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
				DASingleOutV20(XDIRECT,DAdata);
				//******************************2017USB******************************************//					
																
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
				        while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
								break;
						}


                ////////////////////////在外面的循环里终止扫描，同时将X和Y压电陶瓷归零 20150209////////////////
				if (quickstopflag==TRUE)
				{
					//******************************2017USB**************************************//
					voltagex=0;
		            DAdata=(WORD)(voltagex*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
					DASingleOutV20(XDIRECT,DAdata);

					voltagey=0;
		            DAdata=(WORD)(voltagey*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
					DASingleOutV20(YDIRECT,DAdata);//
					//******************************2017USB*****************************************//
					break;
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				//***************************** AD采样 *****************************************//
				ADSingleV20(0,&adResult);//从USB采集卡读取0通道数据
				Volt=(float)(PerLsbVolt*adResult/1000);//计算实际电压值,单位V
				//******************************2017USB*****************************************//


				//****************** X方向和Y方向图像消倾斜处理 ***************************//
				CString m_cstr;
				GetDlgItemText(IDC_KX, m_cstr);
				m_kx = atof(m_cstr)/100.0; 
				Volt=(float)(Volt+m_kx*(50-width)/50); // X方向图像倾斜度调节 20140506
				
				GetDlgItemText(IDC_KY, m_cstr);
				m_ky = atof(m_cstr)/100.0;
				Volt=(float)(Volt+m_ky*(50-height)/50); // Y方向图像倾斜度调节 20140506
				////////////////////////////////////////////////////////////////////////////		
				

				smallbmpdata[width][height]=Volt; //将消倾斜后的电压值，传输给小图的图像数组
				if (smallbmpdata[width][height]<=0.0)
				{
					smallbmpdata[width][height]=0.0;
				}
				
				//用上一幅图像中传递过来的z最大和z最小值（对第一次图像扫描而言，来自前10行扫描）为上下界划分为255等分进行量化，用于扫描过程中实时显示
				if (smallbmpdata[width][height]>oldzmax)
					tempbmpbits=oldzmax;
				if (smallbmpdata[width][height]<oldzmin)
					tempbmpbits=oldzmin;
				if (smallbmpdata[width][height]<=oldzmax&&smallbmpdata[width][height]>=oldzmin)
					tempbmpbits=smallbmpdata[width][height];
				
				tempindex=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin));
				

				////////////////////////////////////////////////////////////////////////////// 20140506新调色板
				if(tempindex>=128) 
				{	dc.SetPixelV(width+483,height+180,RGB(255,tempindex,2*(tempindex-128)+1)); // 20140518，原来423,150
				}
				
				if(tempindex<128) 
				{
					dc.SetPixelV(width+483,height+180,RGB(2*tempindex,tempindex,0)); 
				}
				////////////////////////////////////////////////////////////////////////////// 20140506新调色板
				
				//捕捉外来消息
				if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
				{
					::TranslateMessage(&message);
					::DispatchMessage(&message);
				}
				
			}
					
			//恢复x方向的伸长到最初的x方向偏移位置
		    voltagex=pianzhivoltagex;
			if(voltagex<=0)voltagex=0;
			DAdata=(WORD)(voltagex*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
			DASingleOutV20(XDIRECT,DAdata);
			//******************************2017USB*****************************************//
					
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
					    while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
							break;
						}

		
			}
			
		   //恢复y方向的伸长到最初的y方向偏移位置
		   voltagey=pianzhivoltagey;
           if(voltagey<=0)voltagey=0;
		   DAdata=(WORD)(voltagey*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
		   DASingleOutV20(YDIRECT,DAdata);
		   //******************************2017USB*****************************************//	

		            QueryPerformanceCounter(&litmp);
			        begin=litmp.QuadPart;	
	       		        while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
							break;
						}
			   

	}
	//************************ X方向小图扫描结束，如果继续扫描，跳回到开始if (height==200&&width==200)处，继续扫描 ************************//
		

	
	//****************************************** X方向大图扫描，X方向大图扫描，X方向大图扫描 *********************************************//
	if (m_radiobmpsize==1) // X方向大图实时扫描
	{
        //***** 先X方向预扫描10行，不显示图像，仅计算其最大值和最小值，作为第一幅完整扫描图像的基准 20140521
		int m_height = 5;

		if(finishtimes == 0) // 
		{
		    delaytime=time/40000.0 - 10.0*systemdelay;
//          delaytime=time/40000.0;
			voltagey=pianzhivoltagey;

			for(height=0;height < m_height;height++)
			{
				//写入y方向的电压信号
				voltagey=voltagey+movevoltagey;
				voltagey=voltagey-voltagey*movevoltagey*ceramicscoefficient; //校正压电陶瓷的非线性
//	         	DAdata=(WORD)(((voltagey*1000.0)*4096)/4250.0);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
		        DAdata=(WORD)(voltagey*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
				DASingleOutV20(YDIRECT,DAdata);
				//******************************2017USB******************************************//
							
				voltagex=pianzhivoltagex;
								
				for(width=0;width<400;width++)
				{
					//写入x方向的电压信号
					voltagex=voltagex+movevoltagex; //20140520，扫描第0,10,20,30.....行，作为下一幅图像的基准
					voltagex=voltagex-voltagex*movevoltagex*ceramicscoefficient; //校正压电陶瓷的非线性
		            DAdata=(WORD)(voltagex*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
					DASingleOutV20(XDIRECT,DAdata);
					//******************************2017USB******************************************//

				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
				        while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
								break;
						}


					////////////////////////在外面的循环里终止扫描，同时将X和Y压电陶瓷归零 20150209///////////
					if (quickstopflag==TRUE)
					{
					//******************************2017USB*****************************************//
					voltagex=0;
		            DAdata=(WORD)(voltagex*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
					DASingleOutV20(XDIRECT,DAdata);

					voltagey=0;
		            DAdata=(WORD)(voltagey*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
					DASingleOutV20(YDIRECT,DAdata);//
					//******************************2017USB*****************************************//
                    break;
					}
                    /////////////////////////////////////////////////////////////////////////////////////////
                    
					//AD采样
					//******************************2017USB*****************************************//
					ADSingleV20(0,&adResult);//从USB采集卡读取0通道数据
					Volt=(float)(PerLsbVolt*adResult/1000);//计算实际电压值,单位V
					//******************************2017USB*****************************************//
					

				    //*********************** X方向和Y方向图像消倾斜处理 ***************************//
					CString m_cstr;
					GetDlgItemText(IDC_KX, m_cstr);
					m_kx = atof(m_cstr)/100.0; 
					Volt=(float)(Volt+m_kx*(200-width)/200); // X方向图像倾斜度调节 20140506
					
					GetDlgItemText(IDC_KY, m_cstr);
					m_ky = atof(m_cstr)/100.0;
					Volt=(float)(Volt+m_ky*(200-height)/200); // Y方向图像倾斜度调节 20140506
					
					if(Volt<0)Volt=0;
					//////////////////////////////////////////////////////////////////////////////////	

					largebmpdata[width][height]=Volt; 

					      //////// 预扫描时，也显示扫描行，以免误解为仪器没有启动扫描 /////////
				          if (largebmpdata[width][height]>2.5)
					      tempbmpbits=2.5;
				          if (largebmpdata[width][height]<1.0)
					      tempbmpbits=1.0;
			              if (largebmpdata[width][height]<=2.5&&largebmpdata[width][height]>=1.0)
					      tempbmpbits=largebmpdata[width][height];
			              tempindex=(BYTE)(255*(tempbmpbits-1.0)/(2.5-1.0));
				          ////////////////////////////////////////////////////////////////////////////// 20140506新调色板
				          if(tempindex>=128) 
						  {	dc.SetPixelV(width+332,height+20,RGB(255,tempindex,2*(tempindex-128)+1)); // 20140518，原来423,150
						  }
				          if(tempindex<128) 
						  {
				        	dc.SetPixelV(width+332,height+20,RGB(2*tempindex,tempindex,0)); 
						  }
				          ///////////////////////////////////////////////////////////////////////////// 20140506新调色板

	
					//捕捉外来消息
					if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
					{
						::TranslateMessage(&message);
						::DispatchMessage(&message);
					}
					
				}

			    //恢复x方向的伸长到最初的x方向偏移位置
			    voltagex=pianzhivoltagex;
		        DAdata=(WORD)(voltagex*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
				DASingleOutV20(XDIRECT,DAdata);
				//******************************2017USB*****************************************//	
										
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
				        while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
								break;
						}

			}	

    		//恢复y方向的伸长到最初的y方向偏移位置
			voltagey=pianzhivoltagey;
		    DAdata=(WORD)(voltagey*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
			DASingleOutV20(YDIRECT,DAdata);

				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
				        while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
								break;
						}
				
							
			//********** Process，计算前10行扫描的最大值和最小值 ***********//
			float m_ZMax0 = 0.0, m_ZMin0 = 5.0;
			float m_temp;
			
			for(height = 0; height < m_height; height++)
			{	for(width = 40; width < 400; width++) //消除每行前40个点的影响
			{
				m_temp = largebmpdata[width][height];
				if(m_ZMax0 < m_temp)
					m_ZMax0 = m_temp;
				if(m_ZMin0 > m_temp)
					m_ZMin0 = m_temp;
			}
			}

			//*****************2017USB****************************//
			CString m_cstr;
			GetDlgItemText(IDC_KZ, m_cstr);
			float m_kz = atof(m_cstr)/2.0; 
			//*****************2017USB****************************//			
            oldzmin =(float)(m_ZMax0-m_ZMin0)*m_kz+m_ZMin0; //20140521 前10行的最大值和最小值数据，作为第一次完整扫描图像的显示依据、存储依据  
			oldzmax = m_ZMax0;
			
			//设置显示小图的z最大和最小值及其单位
			sprintf(timechar," %1.2f",m_ZMax0);
			m_showzmax.SetWindowText(timechar);
			sprintf(timechar,"V");
			m_showzmaxdanwei.SetWindowText(timechar);
			m_showzmindanwei.SetWindowText(timechar);
			sprintf(timechar," %1.2f",m_ZMin0);
			m_showzmin.SetWindowText(timechar);
		}
		//************** X方向大图预扫描10行结束，并计算出最大最小值，作为X方向大图扫描基准，直接跳到X方向的大图实时扫描开始 ********20140521   
		
		

		//************** X方向完整大图数据计算，如果是第1次扫描，则跳过此段，直接去大图实时扫描；如果是第2次，则进入此段，计算第1次图像的最大值、最小值等，以此类推 *********** 
		if (height==400&&width==400)
		{
			//********* 对刚刚完成完整扫描的图像（定义为第1次图像）的数据进行量化，计算z电压平均值、最大值和最小值 *********//
			zavetemp=0;
			for (height=0;height<400;height++)
			{
				for (width=0;width<400;width++)
				{
					if (largebmpdata[width][399-height]>oldzmax)
						tempbmpbits=oldzmax;
					if (largebmpdata[width][399-height]<oldzmin)
						tempbmpbits=oldzmin;
					if (largebmpdata[width][399-height]<=oldzmax&&largebmpdata[width][399-height]>=oldzmin)
						tempbmpbits=largebmpdata[width][399-height];
					
					//****** 对于第1次完整扫描的图像，依据前10行的oldzmax和oldzmin值，将第1次图像的电压值归一化成255等分的灰度值，以便捕获图像或停止扫描时保存第1次图像数据 ******/
					//****** 对于第2次完整扫描的图像，依据第1次图像的oldzmax和oldzmin值，将第2次图像的电压值归一化成255等分的灰度值，以便捕获图像或停止扫描时保存第2次图像数据 ******/
					//****** 以此类推 ******/
					
					//****** 由于电脑屏幕左下角是坐标原点，因此需要对AFM图像进行坐标变换
					largebmpbits[height][width]=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin)); 
					
					//****** 计算第1次图像的电压平均值，以便在第1次图像扫描一结束时就显示出来
					zavetemp=zavetemp+largebmpdata[width][height]; 
				}
			}
			zave=(float)(zavetemp/160000.0); //第1次图像的电压平均值，在第1次图像扫描一结束就显示出来
			
			
			//***** 计算第1次图像的最大值和最小值前，先平滑（但不改变图像本身），作为计算第1次图像数据的依据，并传给第2次图像作为其显示依据，20140521 */
			float m_ZMax1 = 0.0, m_ZMin1 = 5.0;
			float m_temp;
			
			for(height = 1; height < 399; height++)
			{
				for(width = 40; width <399; width++) //消除每行前40个点的影响
				{
					m_temp = largebmpdata[width][height-1] + largebmpdata[width][height+1] + 
						largebmpdata[width-1][height] + largebmpdata[width+1][height] + 
						largebmpdata[width][height];
					m_temp /= 5.0;
					if(m_ZMax1 < m_temp)
						m_ZMax1 = m_temp;
					if(m_ZMin1 > m_temp)
						m_ZMin1 = m_temp;
				}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////			
			
			//********************* 这些数据，只是传给图像存储之用20140521 ********************//
			zmin = m_ZMin1;   
			zmax = m_ZMax1;	
			//*********************************************************************************//
			
			
			//设置显示第1次完整扫描的大图的z最大值和最小值及其单位
			sprintf(timechar," %1.2f",m_ZMax1); //第1次图像的电压最大值，在第1次图像扫描一结束后、第2次图像扫描过程中一直显示
			m_showzmax.SetWindowText(timechar);
			sprintf(timechar,"V");
			m_showzmaxdanwei.SetWindowText(timechar);
			m_showzmindanwei.SetWindowText(timechar);
			sprintf(timechar," %1.2f",m_ZMin1); //第1次图像的电压最小值，在第1次图像扫描一结束后、第2次图像扫描过程中一直显示
			m_showzmin.SetWindowText(timechar);
			
			
			Ry=(m_ZMax1-m_ZMin1)*(((CMicroscopeApp*)AfxGetApp())->voltagetonanometer);
			Rz=(float)(fabs(m_ZMax1-zave)+fabs(zave-m_ZMin1))*(((CMicroscopeApp*)AfxGetApp())->voltagetonanometer);
			
			Ra=0;
			for (height=0;height<400;height++)
			{
				for (width=0;width<400;width++)
				{
					Ra=Ra+(float)fabs(largebmpdata[width][height]-zave);
				}
			}
			Ra=(float)(Ra/160000.0)*(((CMicroscopeApp*)AfxGetApp())->voltagetonanometer);
			
			//设置显示小图的Ra,Ry,Rz值及其单位
			sprintf(timechar,"%.1f",Ra);
			m_showRa.SetWindowText(timechar);
			sprintf(timechar,"nm");
			m_showRadanwei.SetWindowText(timechar);
			m_showRydanwei.SetWindowText(timechar);
			m_showRzdanwei.SetWindowText(timechar);
			sprintf(timechar,"%.1f",Ry);
			m_showRy.SetWindowText(timechar);
			sprintf(timechar,"%.1f",Rz);
			m_showRz.SetWindowText(timechar);
			
			//设置捕获图像按钮为能使
			m_buttonbmpcapture.EnableWindow(TRUE);
			
			
			//********* 最后，将oldzmax和oldzmin传给第2次图像，作为第2次图像显示的依据，以此类推 *********//
			//*****************2017USB****************************//
			CString m_cstr;
			GetDlgItemText(IDC_KZ, m_cstr);
			float m_kz = atof(m_cstr)/2.0; 
			//*****************2017USB****************************//	
			oldzmin=(float)(m_ZMax1-m_ZMin1)*m_kz+m_ZMin1; //20140521  
			oldzmax=m_ZMax1;		
		}
		//*********************************************** X方向大图数据计算处理结束 **************************************//
		
		
		//********************************************** X方向的大图实时扫描正式开始 *************************************//
		voltagey=pianzhivoltagey;
		for(height=0;height<400;height++)
		{
			//写入y方向的电压信号
			voltagey=voltagey+movevoltagey;
		
			voltagey=voltagey-voltagey*movevoltagey*ceramicscoefficient; //校正压电陶瓷的非线性
			//******************************2017USB******************************************//
//	        DAdata=(WORD)(((voltagey*1000.0)*4096)/4250.0);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
		    DAdata=(WORD)(voltagey*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
			DASingleOutV20(YDIRECT,DAdata);
			//******************************2017USB******************************************//		
			
			voltagex=pianzhivoltagex;
			
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
				        while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
								break;
						}
			
			for(width=0;width<400;width++)
			{
				//写入x方向的电压信号
				voltagex=voltagex+movevoltagex;
				voltagex=voltagex-voltagex*movevoltagex*ceramicscoefficient; //校正压电陶瓷的非线性
		        DAdata=(WORD)(voltagex*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
				DASingleOutV20(XDIRECT,DAdata);
				//******************************2017USB******************************************//					
				
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
				        while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
								break;
						}

			
				////////////////////////在外面的循环里终止扫描，同时将X和Y压电陶瓷归零 20150209///////////////
				if (quickstopflag==TRUE)  
				{
					//******************************2017USB*****************************************//
					voltagex=0;
	            	DAdata=(WORD)(voltagex*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
					DASingleOutV20(XDIRECT,DAdata);

					voltagey=0;
	             	DAdata=(WORD)(voltagey*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
					DASingleOutV20(YDIRECT,DAdata);//
					//******************************2017USB*****************************************//
                    break;
				}		
				/////////////////////////////////////////////////////////////////////////////////////////////
				

				//AD采样
				//******************************2017USB************************************//
				ADSingleV20(0,&adResult);//从USB采集卡读取0通道数据
				Volt=(float)(PerLsbVolt*adResult/1000);//计算实际电压值,单位V
				//******************************2017USB************************************//
				

				//****************** X方向和Y方向图像消倾斜处理 ***************************//
				CString m_cstr;
				GetDlgItemText(IDC_KX, m_cstr);
				m_kx = atof(m_cstr)/100.0; 
				Volt=(float)(Volt+m_kx*(200-width)/200); // X方向图像倾斜度调节 20140506
				
				GetDlgItemText(IDC_KY, m_cstr);
				m_ky = atof(m_cstr)/100.0;
				Volt=(float)(Volt+m_ky*(200-height)/200); // Y方向图像倾斜度调节 20140506
				////////////////////////////////////////////////////////////////////////////		
				

				largebmpdata[width][height]=Volt; //将消倾斜后的电压值，传输给小图的图像数组
				if (largebmpdata[width][height]<=0.0)
				{
					largebmpdata[width][height]=0.0;
				}
				
				//用上一幅图像中传递过来的z最大和z最小值（对第一次图像扫描而言，来自前10行扫描）为上下界划分为255等分进行量化，用于扫描过程中实时显示
				if (largebmpdata[width][height]>oldzmax)
					tempbmpbits=oldzmax;
				if (largebmpdata[width][height]<oldzmin)
					tempbmpbits=oldzmin;
				if (largebmpdata[width][height]<=oldzmax&&largebmpdata[width][height]>=oldzmin)
					tempbmpbits=largebmpdata[width][height];
				
				tempindex=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin));
				
				////////////////////////////////////////////////////////////////////////////// 20140506新调色板
				if(tempindex>=128) 
				{	dc.SetPixelV(width+332,height+20,RGB(255,tempindex,2*(tempindex-128)+1)); // 20140518，原来423,150
				}
				
				if(tempindex<128) 
				{
					dc.SetPixelV(width+332,height+20,RGB(2*tempindex,tempindex,0)); 
				}
				////////////////////////////////////////////////////////////////////////////// 20140506新调色板
				
				//捕捉外来消息
				if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
				{
					::TranslateMessage(&message);
					::DispatchMessage(&message);
				}
				
			}
			//恢复x方向的伸长到最初的x方向偏移位置
			voltagex=pianzhivoltagex;
		    DAdata=(WORD)(voltagex*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
			DASingleOutV20(XDIRECT,DAdata);
			//******************************2017USB*****************************************//	

				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
				        while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
								break;
						}
		}

		   //恢复y方向的伸长到最初的y方向偏移位置
		   voltagey=pianzhivoltagey;
		   DAdata=(WORD)(voltagey*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
		   DASingleOutV20(YDIRECT,DAdata);
		   //******************************2017USB*****************************************//	

		   QueryPerformanceCounter(&litmp);
	       begin=litmp.QuadPart;

				        while(1)
						{
							QueryPerformanceCounter(&litmp);
							end=litmp.QuadPart;
							if ((double)(end-begin)/freq>delaytime)
								break;
						}
		
	}
//************************ X方向大图扫描结束，如果继续扫描，跳回到开始if (height==400&&width==400)处，继续扫描 ************************//

}
//************************ X方向扫描全部结束 **********************************************************************//



//*************************************** Y方向小图扫描或Y方向大图扫描，现在关闭此功能 20140521 ************************************//
if(m_direction==1) // Y方向的扫描  Y方向的扫描
{
	// Y方向扫描的小图实时扫描
	if (m_radiobmpsize==0)
	{
		//检查上次扫描是否正常完成，如果正常完成就计算在值并使捕获图像按钮为能使（其实这个操作仅检查是否第一次扫描正常完成）
		if (height==100&&width==100)
		{
			//量化数据并计算z平均、最大和最小
			zmax=zmin=smallbmpdata[0][0];
			zavetemp=0;
			for (width=0;width<100;width++)
			{
				for (height=0;height<100;height++)
				{
					//以上一幅图像中的z最大和z最小值为上下界划分为255等分，进行量化
					if (smallbmpdata[height][99-width]>oldzmax)
						tempbmpbits=oldzmax;
					if (smallbmpdata[height][99-width]<oldzmin)
						tempbmpbits=oldzmin;
					if (smallbmpdata[height][99-width]<=oldzmax&&smallbmpdata[height][99-width]>=oldzmin)
						tempbmpbits=smallbmpdata[height][99-width];
					
					smallbmpbits[width][height]=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin));
					zavetemp=zavetemp+smallbmpdata[width][height];
					if (zmax<=smallbmpdata[width][height])
					{
						zmax=smallbmpdata[width][height];
					}
					if (zmin>=smallbmpdata[width][height])
					{
						zmin=smallbmpdata[width][height];
					}
				}
			}
			zave=(float)(zavetemp/10000.0);
			
			oldzmin=zmin;
			//以当前图像中z最大值的百分比作为上一幅图像的z最大值，百分比即为输入的亮度阈值
			//			oldzmax=(float)((zmax-zmin)*lightnessthreshold/100.0+zmin);
			oldzmax=zmax;
			
			//设置显示小图的z最大和最小值及其单位
			sprintf(timechar," %1.2f",zmax);
			m_showzmax.SetWindowText(timechar);
			sprintf(timechar,"V");
			m_showzmaxdanwei.SetWindowText(timechar);
			m_showzmindanwei.SetWindowText(timechar);
			sprintf(timechar," %1.2f",zmin);
			m_showzmin.SetWindowText(timechar);
			
			//计算粗糙度Ra，Ry和Rz
			//Ra计算
			//由smallbmpbits数组计算得到其代表的电压值，并计算其中的平均值和最大值，最小值
			Raavetemp=0;
			Rmaxtemp=Rmintemp=smallbmpbits[0][0];
			for (height=0;height<100;height++)
			{
				for (width=0;width<100;width++)
				{
					
					Raavetemp=Raavetemp+(zmax-zmin)*(float)smallbmpbits[width][height]/255+zmin;
					if (Rmaxtemp<=smallbmpbits[width][height])
					{
						Rmaxtemp=smallbmpbits[width][height];
					}
					if (Rmintemp>=smallbmpbits[width][height])
					{
						Rmintemp=smallbmpbits[width][height];
					}
				}
			}
			Raavetemp=(float)(Raavetemp/10000.0);
			
			Ry=((zmax-zmin)*(Rmaxtemp-Rmintemp)/255)*(((CMicroscopeApp*)AfxGetApp())->voltagetonanometer);
			Rz=(float)(fabs((zmax-zmin)*Rmaxtemp/255+zmin-Raavetemp)+fabs((zmax-zmin)*Rmintemp/255+zmin-Raavetemp))*(((CMicroscopeApp*)AfxGetApp())->voltagetonanometer);
			
			Ra=0;
			for (height=0;height<100;height++)
			{
				for (width=0;width<100;width++)
				{
					Ra=Ra+(float)fabs((zmax-zmin)*(float)smallbmpbits[width][height]/255+zmin-Raavetemp);
				}
			}
			Ra=(float)(Ra/10000.0)*(((CMicroscopeApp*)AfxGetApp())->voltagetonanometer);
			
			//设置显示小图的Ra,Ry,Rz值及其单位
			sprintf(timechar,"%.1f",Ra);
			m_showRa.SetWindowText(timechar);
			sprintf(timechar,"nm");
			m_showRadanwei.SetWindowText(timechar);
			m_showRydanwei.SetWindowText(timechar);
			m_showRzdanwei.SetWindowText(timechar);
			sprintf(timechar,"%.1f",Ry);
			m_showRy.SetWindowText(timechar);
			sprintf(timechar,"%.1f",Rz);
			m_showRz.SetWindowText(timechar);
			
			
			
			//设置捕获图像按钮为能使
			m_buttonbmpcapture.EnableWindow(TRUE);
		}
		//计算每一象素点的延时时间	
        delaytime=time/10000.0 - 10.0*systemdelay;	
//		delaytime=time/10000.0;
		
		//*************************************************************************************
		// Y方向的小图实时扫描开始，发送DA信号，采集AD
		voltagex=pianzhivoltagex;
		for(width=0;width<100;width++)
		{
			//写入x方向的电压信号
			voltagex=voltagex+movevoltagex;
			
			//校正压电陶瓷的非线性
			voltagex=voltagex-voltagex*movevoltagex*ceramicscoefficient;
			//			voltagex=voltagex-voltagex*movevoltagex*0.10;
			
			//***********************************pci2003********************************************//
			DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);     //将电压值转换成LSB码制
			//2014/7 pci2003--->pci8620
			PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
			//************************************pci2003************************************************//
			
			voltagey=pianzhivoltagey;
			
			QueryPerformanceCounter(&litmp);
			rowscanbegin=litmp.QuadPart;
			
			for(height=0;height<200;height++)
			{
				
				//写入y方向的电压信号
				voltagey=voltagey+movevoltagey;
				
				//校正压电陶瓷的非线性
				voltagey=voltagey-voltagey*movevoltagey*ceramicscoefficient;
				//				voltagey=voltagey-voltagey*movevoltagey*0.10;
				
				//***********************************pci2003**************************************************//
				DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
				PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
				//************************************pci2003****************************************************//
				
				//根据用户输入的时间，程序延迟一段时间
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
				while(1)
				{
					QueryPerformanceCounter(&litmp);
					end=litmp.QuadPart;
					if ((double)(end-begin)/freq>delaytime)
						break;
				}
				
				//AD采样	
				//******************************pci2003*********************************************//
				//初始化设备对象中的AD部件,为设备操作就绪有关工作,如预置AD采集通道,采样频率等
				
				//在外面的循环里终止扫描
				if (quickstopflag==TRUE)
				{
					//**************************************pci2003**********************************************//	
					//2014/7 pci2003--->pci8620
                    voltagex=0;
					DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
					PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
					//**************************************pci2003**********************************************//

					//**************************************pci2003**********************************************//	
					//2014/7 pci2003--->pci8620
                    voltagey=0;
					DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
					PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
					//**************************************pci2003**********************************************//	
                break;
				}	
				
				ReadDataFromAD(hDevice, Para, Volt);
				//******************************pci2003*********************************************//
				
				smallbmpdata[width][height]=Volt; 
				if (smallbmpdata[width][height]<=0.0)
				{
					smallbmpdata[width][height]=0.0;
				}
				
				//以上一幅图像中的z最大和z最小值为上下界划分为255等分，进行量化
				if (smallbmpdata[width][height]>oldzmax)
					tempbmpbits=oldzmax;
				if (smallbmpdata[width][height]<oldzmin)
					tempbmpbits=oldzmin;
				if (smallbmpdata[width][height]<=oldzmax&&smallbmpdata[width][height]>=oldzmin)
					tempbmpbits=smallbmpdata[width][height];
				
				
				tempindex=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin));
				//在画图区画点
				
				////////////////////////////////////////////////////////////////////////////// 2014
				
				if(tempindex>=128) 
				{	dc.SetPixelV(width+483,height+180,RGB(255,tempindex,2*(tempindex-128)+1)); 
				}
				
				if(tempindex<128) 
				{
					dc.SetPixelV(width+483,height+180,RGB(2*tempindex,tempindex,0)); 
				}
				////////////////////////////////////////////////////////////////////////////// 2014
				
				//捕捉外来消息
				if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
				{
					::TranslateMessage(&message);
					::DispatchMessage(&message);
				}
			}
			//恢复y方向的伸长到最初的y方向偏移位置
			for (i=0;i<height;i++)
			{
				//因为压电陶瓷线性校正后，按其原来步长逐渐降低电压可能会小于原来设定的偏置电压值，所以需要判断语句进行判断
				//线性校正：输入电压不为线性而使得压电陶瓷的伸长量为线性
				if (voltagey> pianzhivoltagey)
				{
					voltagey=voltagey-movevoltagey;
				}
				if (voltagey <= pianzhivoltagey)
				{
					voltagey=pianzhivoltagey;
				}
				
				//*****************************************pci2003******************************************//
				DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
				PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
				//*****************************************pci2003******************************************//
				
				//根据用户输入的时间，y方向空回时延迟一段时间
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
				while(1)
				{
					QueryPerformanceCounter(&litmp);
					end=litmp.QuadPart;
					if ((double)(end-begin)/freq>delaytime)
						break;
				}
			}
			//保险起见，再一次置y方向为最初偏移位置
			voltagey=pianzhivoltagey;
			//**************************************pci2003**********************************************//
			DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
			PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
			//***************************************pci2003*********************************************//
			
			QueryPerformanceCounter(&litmp);
			rowscanend=litmp.QuadPart;		
		}
		//恢复x方向的伸长到最初的x方向偏移位置
		
		for (i=0;i<width;i++)
		{
			//因为压电陶瓷线性校正后，按其原来步长逐渐降低电压可能会小于原来设定的偏置电压值，所以需要判断语句进行判断
			//线性校正：输入电压不为线性而使得压电陶瓷的伸长量为线性
			if (voltagex > pianzhivoltagex)
			{
				voltagex=voltagey-movevoltagex;
			}
			if (voltagex <= pianzhivoltagex)
			{
				voltagex=pianzhivoltagex;
			}
			//**************************************pci2003**********************************************//        
			DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
			PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
			//**************************************pci2003**********************************************//
			//根据用户输入的时间，x方向空回时延迟一段时间
			QueryPerformanceCounter(&litmp);
			begin=litmp.QuadPart;
			while(1)
			{
				QueryPerformanceCounter(&litmp);
				end=litmp.QuadPart;
				if ((double)(end-begin)/freq>delaytime)
					break;
			}
		}
		
		//保险起见，再一次置x方向为最初偏移位置
		voltagex=pianzhivoltagex;
		//**************************************pci2003**********************************************//
		DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
		//**************************************pci2003**********************************************//
	}
	
	// *******************************************************************************************
	// Y方向的大图实时扫描
	if (m_radiobmpsize==1)
	{
		//检查上次扫描是否正常完成，如果正常完成就计算在值并使捕获图像按钮为能使（其实这个操作仅检查是否第一次扫描正常完成）
		if (height==400&&width==400)
		{
			//量化数据并计算z平均、最大和最小
			zmax=zmin=largebmpdata[0][0];
			zavetemp=0;
			for (height=0;height<400;height++)
			{
				for (width=0;width<400;width++)
				{
					//以上一幅图像中的z最大和z最小值为上下界划分为255等分，进行量化
					if (largebmpdata[width][399-height]>oldzmax)
						tempbmpbits=oldzmax;
					if (largebmpdata[width][399-height]<oldzmin)
						tempbmpbits=oldzmin;
					if (largebmpdata[width][399-height]<=oldzmax&&largebmpdata[width][399-height]>=oldzmin)
						tempbmpbits=largebmpdata[width][399-height];
					
					largebmpbits[height][width]=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin));
					zavetemp=zavetemp+largebmpdata[height][width];
					if (zmax<=largebmpdata[height][width])
					{
						zmax=largebmpdata[height][width];
					}
					if (zmin>=largebmpdata[height][width])
					{
						zmin=largebmpdata[height][width];
					}
				}
			}
			zave=(float)(zavetemp/160000.0);
			
			oldzmin=zmin;
			//以当前图像中z最大值的百分比作为上一幅图像的z最大值，百分比即为输入的亮度阈值
			//			oldzmax=(float)((zmax-zmin)*lightnessthreshold/100.0+zmin);
			oldzmax=zmax;
			
			//设置显示大图的z最大和最小值及其单位
			sprintf(timechar," %1.2f",zmax);
			m_showzmax.SetWindowText(timechar);
			sprintf(timechar,"V");
			m_showzmaxdanwei.SetWindowText(timechar);
			m_showzmindanwei.SetWindowText(timechar);
			sprintf(timechar," %1.2f",zmin);
			m_showzmin.SetWindowText(timechar);
			
			//计算粗糙度Ra，Ry和Rz
			//Ra计算
			//由smallbmpbits数组计算得到其代表的电压值，并计算其中的平均值和最大值，最小值
			Raavetemp=0;
			Rmaxtemp=Rmintemp=largebmpbits[0][0];
			for (height=0;height<400;height++)
			{
				for (width=0;width<400;width++)
				{
					Raavetemp=Raavetemp+(zmax-zmin)*(float)largebmpbits[width][height]/255+zmin;
					if (Rmaxtemp<=largebmpbits[width][height])
					{
						Rmaxtemp=largebmpbits[width][height];
					}
					if (Rmintemp>=largebmpbits[width][height])
					{
						Rmintemp=largebmpbits[width][height];
					}
				}
			}
			Raavetemp=(float)(Raavetemp/160000.0);
			
			CMicroscopeApp* app=(CMicroscopeApp*)AfxGetApp();
			
			Ry=((zmax-zmin)*(Rmaxtemp-Rmintemp)/255)*(((CMicroscopeApp*)AfxGetApp())->voltagetonanometer);
			Rz=(float)(fabs((zmax-zmin)*Rmaxtemp/255+zmin-Raavetemp)+fabs((zmax-zmin)*Rmintemp/255+zmin-Raavetemp))*(((CMicroscopeApp*)AfxGetApp())->voltagetonanometer);
			
			Ra=0;
			for (height=0;height<400;height++)
			{
				for (width=0;width<400;width++)
				{
					Ra=Ra+(float)fabs((zmax-zmin)*(float)largebmpbits[width][height]/255+zmin-Raavetemp);
				}
			}
			Ra=(float)(Ra/160000.0)*(((CMicroscopeApp*)AfxGetApp())->voltagetonanometer);
			
			//设置显示大图的Ra,Ry,Rz值及其单位
			sprintf(timechar,"%.1f",Ra);
			m_showRa.SetWindowText(timechar);
			sprintf(timechar,"nm");
			m_showRadanwei.SetWindowText(timechar);
			m_showRydanwei.SetWindowText(timechar);
			m_showRzdanwei.SetWindowText(timechar);
			sprintf(timechar,"%.1f",Ry);
			m_showRy.SetWindowText(timechar);
			sprintf(timechar,"%.1f",Rz);
			m_showRz.SetWindowText(timechar);
			
			
			//设置捕获图像按钮为能使
			m_buttonbmpcapture.EnableWindow(TRUE);
		}
		
		//计算每一象素点的延时时间	
		delaytime=time/40000.0 - 10.0*systemdelay;
//      delaytime=time/40000.0;
		
		
		//****************************************************************************************
		
		// Y方向的大图实时扫描开始
		voltagex=pianzhivoltagex;
		for(width=0;width<400;width++)
		{
			//写入x方向的电压信号
			voltagex=voltagex+movevoltagex;
			
			//校正压电陶瓷的非线性
			voltagex=voltagex-voltagex*movevoltagex*ceramicscoefficient;
			//			voltagex=voltagex-voltagex*movevoltagex*0.10;
			
			//**************************************pci2003**********************************************//		
			DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
			PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
			//**************************************pci2003**********************************************//
			
			voltagey=pianzhivoltagey;
			
			QueryPerformanceCounter(&litmp);
			rowscanbegin=litmp.QuadPart;
			
			for(height=0;height<400;height++)
			{
				//写入y方向的电压信号
				voltagey=voltagey+movevoltagey;
				
				//校正压电陶瓷的非线性
				voltagey=voltagey-voltagey*movevoltagey*ceramicscoefficient;
				//				voltagey=voltagey-voltagey*movevoltagey*0.10;
				
				//**************************************pci2003**********************************************//		
				DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
				PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
				//**************************************pci2003**********************************************//
				
				//根据用户输入的时间，程序延迟一段时间
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
				while(1)
				{
					QueryPerformanceCounter(&litmp);
					end=litmp.QuadPart;
					if ((double)(end-begin)/freq>delaytime)
						break;
				}
				
				//AD采样
				//**************************************pci2003**********************************************//
				//初始化设备对象中的AD部件,为设备操作就绪有关工作,如预置AD采集通道,采样频率等
				
				//在外面的循环里终止扫描
				if (quickstopflag==TRUE)
				{
					//**************************************pci2003**********************************************//	
					//2014/7 pci2003--->pci8620
                    voltagex=0;
					DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
					PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
					//**************************************pci2003**********************************************//

					//**************************************pci2003**********************************************//	
					//2014/7 pci2003--->pci8620
                    voltagey=0;
					DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
					PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
					//**************************************pci2003**********************************************//
                    break;
				}				

				ReadDataFromAD(hDevice, Para, Volt);
				//**************************************pci2003**********************************************//
				
				largebmpdata[width][height]=Volt;
				if (largebmpdata[width][height]<=0.0)
				{
					largebmpdata[width][height]=0.0;
				}
				
				//以上一幅图像中的z最大和z最小值为上下界划分为255等分，进行量化
				if (largebmpdata[width][height]>oldzmax)
					tempbmpbits=oldzmax;
				if (largebmpdata[width][height]<oldzmin)
					tempbmpbits=oldzmin;
				if (largebmpdata[width][height]<=oldzmax&&largebmpdata[width][height]>=oldzmin)
					tempbmpbits=largebmpdata[width][height];
				
				tempindex=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin));
				//在画图区画点
				//				dc.SetPixelV(width+332,height+20,RGB(tempindex,tempindex*0.7,tempindex*0.3));  // 2005年9月23日 //
				
				////////////////////////////////////////////////////////////////////////////// 2014
				
				if(tempindex>=128) 
				{	dc.SetPixelV(width+332,height+20,RGB(255,tempindex,2*(tempindex-128)+1)); 
				}
				
				if(tempindex<128) 
				{
					dc.SetPixelV(width+332,height+20,RGB(2*tempindex,tempindex,0)); 
				}
				////////////////////////////////////////////////////////////////////////////// 2014
				
				
				//捕捉外来消息
				if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
				{
					::TranslateMessage(&message);
					::DispatchMessage(&message);
				}
			}
			//恢复y方向的伸长到最初的y方向偏移位置
			for (i=0;i<height;i++)
			{
				
				//因为压电陶瓷线性校正后，按其原来步长逐渐降低电压可能会小于原来设定的偏置电压值，所以需要判断语句进行判断
				//线性校正：输入电压不为线性而使得压电陶瓷的伸长量为线性
				if (voltagey > pianzhivoltagey)
				{
					voltagey=voltagey-movevoltagey;
				}
				if (voltagey <= pianzhivoltagey)
				{
					voltagey=pianzhivoltagey;
				}
				
				//**************************************pci2003**********************************************//
				DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
				PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
				//**************************************pci2003**********************************************//	    
				
				//根据用户输入的时间，y方向空回时延迟一段时间
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
				while(1)
				{
					QueryPerformanceCounter(&litmp);
					end=litmp.QuadPart;
					if ((double)(end-begin)/freq>delaytime)
						break;
				}
			}
			//保险起见，再一次置y方向为最初偏移位置
			voltagey=pianzhivoltagey;
			//**************************************pci2003**********************************************//
			DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
			PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);   
			//**************************************pci2003**********************************************//
			
			
			QueryPerformanceCounter(&litmp);
			rowscanend=litmp.QuadPart;
			
		}
		//恢复x方向的伸长到最初的x方向偏移位置
		for (i=0;i<width;i++)
		{
			//因为压电陶瓷线性校正后，按其原来步长逐渐降低电压可能会小于原来设定的偏置电压值，所以需要判断语句进行判断
			//线性校正：输入电压不为线性而使得压电陶瓷的伸长量为线性
			if (voltagex > pianzhivoltagex)
			{
				voltagex=voltagex-movevoltagex;
			}
			if (voltagex <= pianzhivoltagex)
			{
				voltagex=pianzhivoltagex;
			}
			
			voltagex=voltagex-movevoltagex;
			//**************************************pci2003**********************************************//		
			DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
			PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
			
			//**************************************pci2003**********************************************//	   
			
			//根据用户输入的时间，x方向空回时延迟一段时间
			QueryPerformanceCounter(&litmp);
			begin=litmp.QuadPart;
			while(1)
			{
				QueryPerformanceCounter(&litmp);
				end=litmp.QuadPart;
				if ((double)(end-begin)/freq>delaytime)
					break;
			}
		}
		//保险起见，再一次置x方向为最初偏移位置
		voltagex=pianzhivoltagex;
		//**************************************pci2003**********************************************//		
		DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
		//**************************************pci2003**********************************************//
	}
	//**************************************
}

////////////////////////////////////////////
now=CTime::GetCurrentTime();
int hourend=now.GetHour();
int minuteend=now.GetMinute();
int secondend=now.GetSecond();

if((m_radiobmpsize==0&&height==100&&width==100)||(m_radiobmpsize==1&&height==400&&width==400))
{
	rowscantime=(rowscanend-rowscanbegin)/1000;
	sprintf(timechar,"%d",rowscantime);
	m_rowscantime.SetWindowText(timechar);
	
	bmpscantime=(hourend-hourbegin)*3600+(minuteend-minutebegin)*60+secondend-secondbegin;
	
	sprintf(timechar,"%d",bmpscantime);
	m_bmpscantime.SetWindowText(timechar);
	
	finishtimes++;
	
	sprintf(timechar,"%d",finishtimes);
	m_finishscantimes.SetWindowText(timechar);
}
//取消偏移时加的电压，使压电陶瓷恢复到0长度
if (pianzhiflag==TRUE)
{
	//恢复x方向的压电陶瓷
	if (pianzhivoltagex!=0.0)
	{
        DAdata=0;
//		PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
//	       	DAdata=(WORD)(((voltagex*1000.0)*4096)/4250.0);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
//		    DAdata=(WORD)(voltagex*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
			DASingleOutV20(XDIRECT,DAdata);
	}
	//恢复y方向的压电陶瓷
	if (pianzhivoltagey!=0.0)
	{
		DAdata=0;
//		PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
//	       	DAdata=(WORD)(((voltagey*1000.0)*4096)/4250.0);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
//		    DAdata=(WORD)(voltagey*963.8);//将电压值转换成LSB码制//由5000.0改为4250.0，使输出最大值接近USB采集板最大输出3.3V
			DASingleOutV20(YDIRECT,DAdata);
	}
}
//重画画图显示区
if (redrawflag==TRUE)
{
	Invalidate();
}

//设置时间，进行下一次循环
if (settimerflag==TRUE)
{
	SetTimer(1,5,NULL);
}
//扫描结束标志
scanendflag=TRUE;
//	singlescannerdlg.Startcapture();
if(m_selectscantime==TRUE)
{
	if(finishtimes==m_selecttimes)
	{
		CMeasure::OnButtonstopscan();
	}
}

}



void CMeasure::OnCheckpianzhi() 
{
	//更新对话框中各控件的数值
	if (UpdateData(TRUE)==0)
	{
		//如果扫描范围输入有错误，保持check状态的原始状态
		//checkflag是DoDataExchange函数的标志，目的是只让对话框里的check控件进行数据交换	
		checkflag=FALSE;
		UpdateData(FALSE);
		checkflag=TRUE;
		return;
	}
	//根据偏置选择框的选中与否来能使偏置输入框的状态
	if (m_enablepianzhiflag==TRUE)
	{
		CEdit* editpianzhix=(CEdit*)GetDlgItem(IDC_PIANZHIX);
		editpianzhix->EnableWindow(TRUE);
		CEdit* editpianzhiy=(CEdit*)GetDlgItem(IDC_PIANZHIY);
		editpianzhiy->EnableWindow(TRUE);
		//设置偏置静态文本控件的显示范围和可以输入的最大最小值和spin控件的值
		if (m_radiorange==0)
		{
			sprintf(timechar,"0 ～ %d     ",(20-m_smallrangex));
			m_pianzhirangex.SetWindowText(timechar);
			
			sprintf(timechar,"0 ～ %d     ",(20-m_smallrangey));
			m_pianzhirangey.SetWindowText(timechar);
			
			m_spinpianzhix.SetRange(0,20-m_smallrangex);
			m_spinpianzhiy.SetRange(0,20-m_smallrangey);
			
		}
		if (m_radiorange==1)
		{
			sprintf(timechar,"0 ～ %d     ",(4000-m_largerangex));
			m_pianzhirangex.SetWindowText(timechar);
			
			sprintf(timechar,"0 ～ %d     ",(4000-m_largerangey));
			m_pianzhirangey.SetWindowText(timechar);
			
			m_spinpianzhix.SetRange(0,4000-m_largerangex);
			m_spinpianzhiy.SetRange(0,4000-m_largerangey);
			
		}
		
	}
	if (m_enablepianzhiflag==FALSE)
	{
		CEdit* editpianzhix=(CEdit*)GetDlgItem(IDC_PIANZHIX);
		editpianzhix->EnableWindow(FALSE);
		CEdit* editpianzhiy=(CEdit*)GetDlgItem(IDC_PIANZHIY);
		editpianzhiy->EnableWindow(FALSE);
		//设置偏置静态文本控件的显示范围和可以输入的最大最小值和spin控件的值
		if (m_radiorange==0)
		{
			sprintf(timechar,"0 ～ %d     ",(20-m_smallrangex));
			m_pianzhirangex.SetWindowText(timechar);
			
			sprintf(timechar,"0 ～ %d     ",(20-m_smallrangey));
			m_pianzhirangey.SetWindowText(timechar);
			
			m_spinpianzhix.SetRange(0,20-m_smallrangex);
			m_spinpianzhiy.SetRange(0,20-m_smallrangey);
			
		}
		if (m_radiorange==1)
		{
			sprintf(timechar,"0 ～ %d     ",(4000-m_largerangex));
			m_pianzhirangex.SetWindowText(timechar);
			
			sprintf(timechar,"0 ～ %d     ",(4000-m_largerangey));
			m_pianzhirangey.SetWindowText(timechar);
			
			m_spinpianzhix.SetRange(0,4000-m_largerangex);
			m_spinpianzhiy.SetRange(0,4000-m_largerangey);
			
		}
		
	}
	
}

void CMeasure::OnButtongoback() 
{
	if(scanendflag==TRUE)
	{
		CDialog::OnCancel();
	}
	else
		MessageBox(_T("上次扫描还未完成"),
		_T("请稍等"), MB_ICONEXCLAMATION | MB_OK);	
}

void CMeasure::OnCancel() 
{
	//重载OnCancel函数，并使其无效，目的是让对话框不响应esc键	
	//	CDialog::OnCancel();
}

void CMeasure::OnOK() 
{
	//重载OnOk函数，并使其无效，目的是让对话框不响应enter键		
	//	CDialog::OnOK();
}

void CMeasure::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(redrawflag==TRUE&&scanendflag==TRUE)
	{
		CClientDC dc(this);
		CEdit* editsinglexscantxt=(CEdit*)GetDlgItem(IDC_SINGLEXSCANTXT);
		CEdit* editsingleyscantxt=(CEdit*)GetDlgItem(IDC_SINGLEYSCANTXT);
		if(MouseSelectFlag==TRUE&&SingleXFlag==FALSE&&SingleYFlag==FALSE)
		{
			CRectTracker RTracker;
			RTracker.m_nStyle=CRectTracker::dottedLine;
			
			RTracker.TrackRubberBand(this,point,TRUE);
			
			m_bLMouseDown=TRUE;
			xold=point.x;
			yold=point.y;
			
			if (m_radiobmpsize==0)
			{
				if(xold<483||yold<180||xold>583||yold>280)
					return;
				else
				{
					rightstart=TRUE;
					xold=xold-483;
					yold=yold-180;
				}
			}
			
			if (m_radiobmpsize==1)
			{
				if(xold<332||yold<20||xold>732||yold>420)
					return;
				else
				{
					rightstart=TRUE;
					xold=xold-332;
					yold=yold-20;
				}
			}
			
		}
		
		if((SingleXFlag==TRUE||SingleYFlag==TRUE)/*&&MouseSelectFlag==FALSE*/)
		{
			xold=point.x;
			yold=point.y;
			
			if (m_radiobmpsize==0)
			{
				if(xold<483||yold<180||xold>583||yold>280)
					return;
				else
				{
					rightstart=TRUE;
					xold=xold-483;
					yold=yold-180;
				}
				if(SingleXFlag==TRUE)
				{
					CMeasure::OnCheckpianzhi();
					
					m_pianzhiy=oldpianzhiy+yold*oldsmallrangex/100;
					
					sprintf(timechar,"%d",m_pianzhiy);
					m_cpianzhiy.SetWindowText(timechar);
					
					sprintf(timechar,"%d",yold);
					m_csinglexscantxt.SetWindowText(timechar);
					
					Invalidate();
					UpdateWindow();
					
					for(width=0;width<100;width++)
						dc.SetPixelV(width+483,yold+180,RGB(255,0,0));
				}
				if(SingleYFlag==TRUE)
				{
					CMeasure::OnCheckpianzhi();
					
					m_pianzhix=oldpianzhix+xold*oldsmallrangex/100;
					
					sprintf(timechar,"%d",m_pianzhix);
					m_cpianzhix.SetWindowText(timechar);
					
					sprintf(timechar,"%d",xold);
					m_csingleyscantxt.SetWindowText(timechar);
					
					Invalidate();
					UpdateWindow();
					
					for(height=0;height<100;height++)
						dc.SetPixelV(xold+483,height+180,RGB(255,0,0));
				}
			}
			
			if (m_radiobmpsize==1)
			{
				if(xold<332||yold<20||xold>732||yold>420)
					return;
				else
				{
					rightstart=TRUE;
					xold=xold-332;
					yold=yold-20;
				}
				
				if(SingleXFlag==TRUE)
				{
					CMeasure::OnCheckpianzhi();
					
					m_pianzhiy=oldpianzhiy+yold*oldlargerangex/400;
					
					sprintf(timechar,"%d",m_pianzhiy);
					m_cpianzhiy.SetWindowText(timechar);
					
					sprintf(timechar,"%d",yold);
					m_csinglexscantxt.SetWindowText(timechar);
					
					Invalidate();
					UpdateWindow();
					
					for(width=0;width<400;width++)
						dc.SetPixelV(width+332,yold+20,RGB(255,0,0));
				}
				if(SingleYFlag==TRUE)
				{
					CMeasure::OnCheckpianzhi();
					
					m_pianzhix=oldpianzhix+xold*oldlargerangex/400;
					
					sprintf(timechar,"%d",m_pianzhix);
					m_cpianzhix.SetWindowText(timechar);
					
					sprintf(timechar,"%d",xold);
					m_csingleyscantxt.SetWindowText(timechar);
					
					Invalidate();
					UpdateWindow();
					
					for(height=0;height<400;height++)
						dc.SetPixelV(xold+332,height+20,RGB(255,0,0));
				}
			}
		}
}
CDialog::OnLButtonDown(nFlags, point);
}

void CMeasure::OnMouseMove(UINT nFlags, CPoint point)
{
	if(rightstart&&MouseSelectFlag==TRUE&&SingleXFlag==FALSE&&SingleYFlag==FALSE)
	{
		CClientDC dc(this);
		xnew=point.x;
		ynew=point.y;
		
		if(m_radiobmpsize==0)
		{
			if(xnew<483||ynew<180||xnew>583||ynew>280)
				return;
			if(abs(point.x-xold-483)>abs(point.y-yold-180))
				change=point.y-yold-180;
			else
				change=point.x-xold-483;			
			
			if(xnew>=483&&xnew<=583)
				xnew=xold+change;
			if(ynew>=180&&ynew<=280)
				ynew=yold+change;
		}
		if(m_radiobmpsize==1)
		{
			if(xnew<332&&ynew>20&&ynew<420)
				xnew=332;
			if(xnew>732&&ynew>20&&ynew<420)
				xnew=732;
			if(ynew<20&&xnew>332&&xnew<732)
				ynew=20;
			if(ynew>420&&xnew>332&&xnew<732)
				ynew=420;
			if(xnew<332&&ynew<20)
			{
				xnew=332;
				ynew=20;
			}
			if(xnew>732&&ynew>420)
			{
				xnew=732;
				ynew=420;
			}
			if(xnew<332&&ynew>420)
			{
				xnew=332;
				ynew=420;
			}
			if(xnew>732&&ynew<20)
			{
				xnew=732;
				ynew=20;
			}
			
			if(abs(xnew-xold-332)>abs(ynew-yold-20))
				change=abs(ynew-yold-20);
			else
				change=abs(xnew-xold-332);
			
			if((ynew-yold-20)<40||(xnew-xold-332)<40)
				return;
			
			if(xnew>=332&&xnew<=732)
				xnew=xold+change*(xnew-xold-332)/(abs(xnew-xold-332));
			if(ynew>=20&&ynew<=420)
				ynew=yold+change*(ynew-yold-20)/(abs(ynew-yold-20));
		}
		
		if(m_radiobmpsize==0)
		{
			sprintf(timechar,"%d",xold*oldsmallrangex/100);
			m_mouserangexfrom.SetWindowText(timechar);
			
			sprintf(timechar,"%d",xnew*oldsmallrangex/100);
			m_mouserangexto.SetWindowText(timechar);
			
			sprintf(timechar,"%d",yold*oldsmallrangex/100);
			m_mouserangeyfrom.SetWindowText(timechar);
			
			sprintf(timechar,"%d",ynew*oldsmallrangex/100);
			m_mouserangeyto.SetWindowText(timechar);
			
			m_largerangex=abs(change*oldsmallrangex/100);
			m_largerangey=m_largerangex;
			
			sprintf(timechar,"%d",m_largerangex);
			m_clargerangex.SetWindowText(timechar);
			
			sprintf(timechar,"%d",m_largerangey);
			m_clargerangey.SetWindowText(timechar);
			
			CMeasure::OnCheckpianzhi();
			
			m_pianzhix=oldpianzhix+xold*oldsmallrangex/100;
			m_pianzhiy=oldpianzhiy+yold*oldsmallrangex/100;
			
			sprintf(timechar,"%d",m_pianzhix);
			m_cpianzhix.SetWindowText(timechar);
			
			sprintf(timechar,"%d",m_pianzhiy);
			m_cpianzhiy.SetWindowText(timechar);
		}
		
		if(m_radiobmpsize==1)
		{
			sprintf(timechar,"%d",xold*oldlargerangex/400);
			m_mouserangexfrom.SetWindowText(timechar);
			
			sprintf(timechar,"%d",xnew*oldlargerangex/400);
			m_mouserangexto.SetWindowText(timechar);
			
			sprintf(timechar,"%d",yold*oldlargerangex/400);
			m_mouserangeyfrom.SetWindowText(timechar);
			
			sprintf(timechar,"%d",ynew*oldlargerangex/400);
			m_mouserangeyto.SetWindowText(timechar);
			
			m_largerangex=abs(change*oldlargerangex/400);
			m_largerangey=m_largerangex;
			
			sprintf(timechar,"%d",m_largerangex);
			m_clargerangex.SetWindowText(timechar);
			
			sprintf(timechar,"%d",m_largerangey);
			m_clargerangey.SetWindowText(timechar);
			
			CMeasure::OnCheckpianzhi();
			
			m_pianzhix=oldpianzhix+xold*oldlargerangex/400;
			m_pianzhiy=oldpianzhiy+yold*oldlargerangex/400;
			
			sprintf(timechar,"%d",m_pianzhix);
			m_cpianzhix.SetWindowText(timechar);
			
			sprintf(timechar,"%d",m_pianzhiy);
			m_cpianzhiy.SetWindowText(timechar);
		}
		
		Invalidate();
		CMeasure::OnPaint();
		
		if(m_radiobmpsize==0)
		{
			dc.MoveTo(xold+483,yold+180);
			dc.LineTo(xold+483,ynew+180);
			dc.MoveTo(xold+483,ynew+180);
			dc.LineTo(xnew+483,ynew+180);
			dc.MoveTo(xnew+483,ynew+180);
			dc.LineTo(xnew+483,yold+180);
			dc.MoveTo(xnew+483,yold+180);
			dc.LineTo(xold+483,yold+180);
		}
		
		if(m_radiobmpsize==1)
		{
			dc.MoveTo(xold+332,yold+20);
			dc.LineTo(xold+332,ynew+20);
			dc.MoveTo(xold+332,ynew+20);
			dc.LineTo(xnew+332,ynew+20);
			dc.MoveTo(xnew+332,ynew+20);
			dc.LineTo(xnew+332,yold+20);
			dc.MoveTo(xnew+332,yold+20);
			dc.LineTo(xold+332,yold+20);
		}
		
	}
	rightstart=FALSE;
	
	CDialog::OnMouseMove(nFlags, point);
}

void CMeasure::OnMouseselect() 
{
	
	if(!m_mouseselect)
		MouseSelectFlag=TRUE;
	
	if(m_mouseselect)
	{
		MouseSelectFlag=FALSE;
		m_largerangex = 4000;
		m_largerangey = 4000;
		
		sprintf(timechar,"%d",m_largerangex);
		m_clargerangex.SetWindowText(timechar);
		
		sprintf(timechar,"%d",m_largerangey);
		m_clargerangey.SetWindowText(timechar);
		
		m_pianzhix=0;
		m_pianzhiy=0;
		
		sprintf(timechar,"%d",m_pianzhix);
		m_cpianzhix.SetWindowText(timechar);
		
		sprintf(timechar,"%d",m_pianzhiy);
		m_cpianzhiy.SetWindowText(timechar);
		
		sprintf(timechar,"%d",0);
		m_mouserangexfrom.SetWindowText(timechar);
		
		sprintf(timechar,"%d",0);
		m_mouserangexto.SetWindowText(timechar);
		
		sprintf(timechar,"%d",0);
		m_mouserangeyfrom.SetWindowText(timechar);
		
		sprintf(timechar,"%d",0);
		m_mouserangeyto.SetWindowText(timechar);
		
		CMeasure::OnCheckpianzhi();
	}
	
	UpdateData();
}



void CMeasure::OnSelectscantime() 
{
	if(!m_selectscantime)
	{
		SelectScanTimeFlag=TRUE;
		CEdit* editselecttimes=(CEdit*)GetDlgItem(IDC_SELECTTIMES);
		editselecttimes->EnableWindow(TRUE);
	}
	if(m_selectscantime)
	{
		SelectScanTimeFlag=FALSE;
		CEdit* editselecttimes=(CEdit*)GetDlgItem(IDC_SELECTTIMES);
		editselecttimes->EnableWindow(FALSE);
	}
	
	UpdateData();
}

void CMeasure::OnSinglexscan() 
{
	if(m_direction==1)
	{
		AfxMessageBox(_T("请保持扫描方向一致"),MB_ICONEXCLAMATION | MB_OK);
		m_singlescan=-1;
		UpdateData(FALSE);
		return;
	}
	if(!m_checksinglescan)
	{
		SingleXFlag=TRUE;
		
		CEdit* editsinglexscan=(CEdit*)GetDlgItem(IDC_SINGLEXSCANTXT);
		editsinglexscan->EnableWindow(TRUE);
		CEdit* editsingleyscan=(CEdit*)GetDlgItem(IDC_SINGLEYSCANTXT);
		editsingleyscan->EnableWindow(FALSE);
		
		CButton* singlexscan=(CButton*)GetDlgItem(IDC_SINGLEXSCAN);
		singlexscan->SetCheck(1);
	}
}

void CMeasure::OnSingleyscan() 
{
	if(m_direction==0)
	{
		AfxMessageBox(_T("请保持扫描方向一致"),MB_ICONEXCLAMATION | MB_OK);
		m_singlescan=-1;
		UpdateData(FALSE);
		return;
	}		
	if(!m_checksinglescan)
	{
		SingleYFlag=TRUE;
		CEdit* editsingleyscan=(CEdit*)GetDlgItem(IDC_SINGLEYSCANTXT);
		editsingleyscan->EnableWindow(TRUE);
		CEdit* editsinglexscan=(CEdit*)GetDlgItem(IDC_SINGLEXSCANTXT);
		editsinglexscan->EnableWindow(FALSE);
		
		CButton* singleyscan=(CButton*)GetDlgItem(IDC_SINGLEYSCAN);
		singleyscan->SetCheck(1);
	}	
}

void CMeasure::OnChecksinglescan() 
{
	CEdit* editsinglexscantxt=(CEdit*)GetDlgItem(IDC_SINGLEXSCANTXT);
	CEdit* editsingleyscantxt=(CEdit*)GetDlgItem(IDC_SINGLEYSCANTXT);
	
	CButton* editsinglexscan=(CButton*)GetDlgItem(IDC_SINGLEXSCAN);
	CButton* editsingleyscan=(CButton*)GetDlgItem(IDC_SINGLEYSCAN);
	if(!m_checksinglescan)
	{
		if(m_direction==0)
		{
			editsinglexscan->EnableWindow(TRUE);
			editsinglexscantxt->EnableWindow(TRUE);
			
			editsingleyscan->EnableWindow(FALSE);
			editsingleyscantxt->EnableWindow(FALSE);
			
			OnSinglexscan();
		}
		
		if(m_direction==1)
		{
			editsinglexscan->EnableWindow(FALSE);
			editsinglexscantxt->EnableWindow(FALSE);
			
			editsingleyscan->EnableWindow(TRUE);
			editsingleyscantxt->EnableWindow(TRUE);
			
			OnSingleyscan();
		}
	}
	if(m_checksinglescan)
	{
		
		editsinglexscan->EnableWindow(FALSE);
		editsingleyscan->EnableWindow(FALSE);
		editsinglexscantxt->EnableWindow(FALSE);
		editsingleyscantxt->EnableWindow(FALSE);
		
		SingleXFlag=FALSE;
		SingleYFlag=FALSE;
		
		m_pianzhix=oldpianzhix;
		m_pianzhiy=oldpianzhiy;
		
		sprintf(timechar,"%d",m_pianzhix);
		m_cpianzhix.SetWindowText(timechar);
		
		sprintf(timechar,"%d",m_pianzhiy);
		m_cpianzhiy.SetWindowText(timechar);
		
		m_checksinglescan=-1;
		m_singlescan=-1;
		UpdateData(FALSE);
		
	}
	
	UpdateData();
	
}



void CMeasure::OnXdirection() 
{
	m_direction=0;	
}

void CMeasure::OnYdirection() 
{
	m_direction=1;	
}

/*
功能: 用来设置控件的EnabeWindow状态 
参数: type 0(DEFAULT)为默认  1(EDIT)为CEdit控件  2(BUTTON)为CButton控件   
status 想要设置的控件的状态 
num 传入的控件个数
... 控件ID
*/
void CMeasure::SetStatus(int type, int status, int num,...)
{
	va_list argp;// 保存函数参数的结构  ...部分
	int idc;  //每个控件的ID
	
	va_start(argp, num);//宏  作用传入num参数后面的第一个可选参数
	while(num > 0)
	{
		idc = va_arg(argp, int);//
		switch (type)
		{
		case 0:
			GetDlgItem(idc) -> EnableWindow(status);
			break;
		case 1:
			((CEdit*)GetDlgItem(idc)) -> EnableWindow(status);
			break;
		case 2:
			((CButton*)GetDlgItem(idc)) -> EnableWindow(status);
		default:
			GetDlgItem(idc) -> EnableWindow(status);
			break;		 
		}
		num--;
	}
}

void CMeasure::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if(pScrollBar->GetDlgCtrlID()==IDC_SPINKZ)
	{
		CString strValue;
		strValue.Format(_T("%1.1f"),(double)(nPos+10)/10);//初始值对比系数为1.0，对应于nPos为0。对比系数最大（1.6），
		((CSpinButtonCtrl*)pScrollBar)->GetBuddy()->SetWindowText(strValue);
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
