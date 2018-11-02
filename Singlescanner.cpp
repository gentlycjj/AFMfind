// Singlescanner.cpp : implementation file
//

#include "stdafx.h"
#include "microscope.h"
#include "Singlescanner.h"
#include "Measure.h"
#include "microscopedoc.h"
#include "conio.h"
#include "dibapi.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSinglescanner dialog

PCI2003_PARA_AD  Para1;  //定义硬件参数

CSinglescanner::CSinglescanner(CWnd* pParent /*=NULL*/)
	: CDialog(CSinglescanner::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSinglescanner)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSinglescanner::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSinglescanner)
	DDX_Control(pDX, IDC_STOP, m_stop);
	DDX_Control(pDX, IDC_GOBACK, m_goback);
	DDX_Control(pDX, IDC_GETDATA, m_getdata);
	DDX_Control(pDX, IDC_CAPTURE, m_capture);
	DDX_Control(pDX, IDC_BMPFRAME, m_bmpframe);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSinglescanner, CDialog)
	//{{AFX_MSG_MAP(CSinglescanner)
	ON_BN_CLICKED(IDC_GOBACK, OnGoback)
	ON_BN_CLICKED(IDC_CAPTURE, OnCapture)
	ON_BN_CLICKED(IDC_GETDATA, OnGetdata)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSinglescanner message handlers

void CSinglescanner::OnGoback() 
{
//	if(measuredlg.scanendflag==TRUE)
//		{
		KillTimer(1);
		CDialog::OnCancel();
//		}
//	else
//	MessageBox(_T("上次扫描还未完成"),
//				_T("请稍等"), MB_ICONEXCLAMATION | MB_OK);		
}

void CSinglescanner::OnCapture() 
{
		BeginWaitCursor();

		//捕获m_bmpframe这个frame上所画的图像，分两步：
		//1.捕获这个frame上所画的图像为一个CBitmap对象，并得到此对象的BITMAP句柄
		//2.把HBITMAP转换成HDIB（DDB to DIB），从而进行显示和以后的保存。由于强制变成24bit的DIB图像，所以不需要加入调色板

		//以下是第一步，得到frame上所画的图像（即CBitmap对象）的HBITMAP句柄

		CDC memDC;  //内存设备上下文
		CClientDC	dc(this);
		CBitmap bitmap; //捕获的BMP图
		CRect	rect;  

		//得到m_curveimage这个frame的大小
		m_bmpframe.GetWindowRect(&rect);
		ScreenToClient(&rect); //把屏幕坐标转化为相对坐标
		//为frame显示设备上下文创建兼容的内存设备上下文
		memDC.CreateCompatibleDC(&dc);
		//创建一个与frame显示设备上下文兼容的位图 
		bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());
		//把新位图选到内存设备上下文中，并返回被替代的原位图的指针
		CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
		//把frame显示设备描上下文拷贝到内存设备上下文中
		memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc, rect.left, rect.top, SRCCOPY); 
		//把被替代的位图重新选入内存设备上下文，即：恢复初始状态
		memDC.SelectObject(pOldBitmap);
    
		//由CBitmap对象得到其HBITMAP句柄
		HBITMAP hBitmap=(HBITMAP)bitmap.GetSafeHandle();

		//以下是第二步，由HBITMAP句柄转换成HDIB句柄

		BITMAP bm;               // bitmap structure
		BITMAPINFOHEADER bi;     // bitmap header
		LPBITMAPINFOHEADER lpbi; // pointer to BITMAPINFOHEADER
		DWORD dwLen;             // size of memory block
		HDIB hDib, h;            // handle to DIB, temp handle
		HDC hDC;                 // handle to DC
		WORD biBits;             // bits per pixel
		UINT wLineLen;           //CBITMAP对象所表示的图像（DDB）的实际宽度的字节数（宽度要是32bit的倍数）
		DWORD dwSize;             //CBITMAP对象所表示的图像（DDB）的大小
		DWORD wColSize;          //调色板所占的字节数
   
		// check if bitmap handle is valid
		if (!hBitmap)
		{
		  return ;
		}
   
		// fill in BITMAP structure, return NULL if it didn't work
		if (!::GetObject(hBitmap, sizeof(bm), &bm))
		{
		  return ;
		}
   
		// calculate bits per pixel
		biBits = (WORD) (bm.bmPlanes * bm.bmBitsPixel);
   
		wLineLen = ( bm.bmWidth * biBits + 31 ) / 32 * 4;
		wColSize = sizeof(RGBQUAD) * (( biBits <= 8 ) ? 
		  1 << biBits : 0 );
		dwSize = sizeof( BITMAPINFOHEADER ) + wColSize +
		  (DWORD)(UINT)wLineLen * (DWORD)(UINT)bm.bmHeight;
   
		// make sure bits per pixel is valid
		if (biBits <= 1)
		  biBits = 1;
		else if (biBits <= 4)
		  biBits = 4;
		else if (biBits <= 8)
		  biBits = 8;
		else // if greater than 8-bit, force to 24-bit
		  biBits = 24;
   
		// initialize BITMAPINFOHEADER
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = bm.bmWidth;
		bi.biHeight = bm.bmHeight;
		bi.biPlanes = 1;
		bi.biBitCount = biBits;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = dwSize - sizeof(BITMAPINFOHEADER) - wColSize;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = ( biBits <= 8 ) ? 1 << biBits : 0;	
		bi.biClrImportant = 0;
   
		// calculate size of memory block required to store BITMAPINFO
		dwLen = bi.biSize + ::PaletteSize((LPSTR) &bi);
   
		// get a DC
		hDC = ::GetDC(NULL);
   
		// alloc memory block to store our bitmap
		hDib = (HDIB)::GlobalAlloc(GHND, dwLen);
   
		// if we couldn't get memory block
		if (!hDib)
		{
		  // clean up and return NULL
		  ::ReleaseDC(NULL, hDC);
  
		  return ;
		}
   
		// lock memory and get pointer to it
		lpbi = (LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)hDib);
		if (!lpbi)
		{
		  // clean up and return NULL
		  ::ReleaseDC(NULL, hDC);
      
		  return ;
		}
   
		// use our bitmap info. to fill BITMAPINFOHEADER
		*lpbi = bi;
   
		// call GetDIBits with a NULL lpBits param, so it will 
		// calculate the biSizeImage field for us
		::GetDIBits(hDC, hBitmap, 0, (WORD)bi.biHeight, NULL, 
		  (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
   
		// get the info. returned by GetDIBits and unlock 
		// memory block
		bi = *lpbi;
		bi.biClrUsed = ( biBits <= 8 ) ? 1 << biBits : 0;
		::GlobalUnlock(hDib);
   
		// if the driver did not fill in the biSizeImage field, 
		// make one up
		if (bi.biSizeImage == 0)
		  bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight;
   
   
		// realloc the buffer big enough to hold all the bits
		dwLen = bi.biSize + ::PaletteSize((LPSTR) &bi) + 
		  bi.biSizeImage;
		h = (HDIB)::GlobalReAlloc(hDib, dwLen, 0);
		if ( h )
		{
		  hDib = h;
		}
		else
		{
		  // clean up and return NULL
		  ::GlobalFree(hDib);
		  hDib = NULL;
      
		  ::ReleaseDC(NULL, hDC);
      
		  return ;
		}
   
		// lock memory block and get pointer to it
		lpbi = (LPBITMAPINFOHEADER)::GlobalLock((HGLOBAL)hDib);
		if (!lpbi)
		{
		  // clean up and return NULL
		  ::GlobalFree(hDib);
		  hDib = NULL;
      
		  ::ReleaseDC(NULL, hDC);
      
		  return ;
		}
   
		// call GetDIBits with a NON-NULL lpBits param, and 
		// actualy get the bits this time
		if (::GetDIBits(hDC, hBitmap, 0, (WORD)bi.biHeight, 
		  (LPSTR)lpbi + (WORD)lpbi->biSize + 
		  ::PaletteSize((LPSTR) lpbi), (LPBITMAPINFO)lpbi, 
		  DIB_RGB_COLORS) == 0)
		{
		  // clean up and return NULL
		  ::GlobalUnlock(hDib);
		  hDib = NULL;
      
		  ::ReleaseDC(NULL, hDC);
      
		  return ;
		}
   
		bi = *lpbi;
   
		// clean up
		::GlobalUnlock(hDib);
		::ReleaseDC(NULL, hDC);


		//打开一个新文档，显示得到的DIB图像
		CMicroscopeApp* app =(CMicroscopeApp*)AfxGetApp();

		//开启新的文档
		POSITION posTemplate = app->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate = app->GetNextDocTemplate(posTemplate);
		CMicroscopeDoc* pDocument = (CMicroscopeDoc*) pDocTemplate->OpenDocumentFile(NULL);

		//传递HDIB，并且更新视图
		pDocument->ReplaceHDIB(hDib);
		pDocument->InitDIBData();    // set up new size & palette
		pDocument->ReplaceFlag(FALSE); //不写txt信息文件
		pDocument->DrawRectTrackerflag=FALSE;	//画橡皮区矩形标志为假
		pDocument->SetModifiedFlag(TRUE);
		pDocument->UpdateAllViews(NULL);	

		EndWaitCursor();
}

//DEL void CSinglescanner::dopaint() 
//DEL {
//DEL 	CDC* pDC=GetDC();
//DEL 	CDC memDC;  //内存设备上下文
//DEL 	CBitmap memBit; //捕获的BMP图
//DEL 	CPen Pen;
//DEL                                                                                 
//DEL 	memDC.CreateCompatibleDC(pDC);                                                  
//DEL 	memBit.CreateCompatibleBitmap(pDC,400,400);                              
//DEL 	CBitmap *pOldBit = memDC.SelectObject(&memBit);                                 
//DEL                                                                                 
//DEL 	Pen.CreatePen(PS_SOLID,1,RGB(0,0,0));                                     
//DEL 	CPen * pPen=memDC.SelectObject(&Pen);                                           
//DEL 
//DEL 	memDC.MoveTo(30,150+(int)(singlexaxis));                                                                 
//DEL 	memDC.LineTo(300+399,150+(int)(singlexaxis));                                                                 
//DEL 
//DEL //	CPen pencoordinateout(PS_SOLID,2,RGB(255,0,0));
//DEL //	Pen.CreatePen(PS_SOLID,1,RGB(255,0,0));                                     
//DEL //	CPen *oldpencoordinateout=dc.SelectObject(&pencoordinateout);
//DEL 	
//DEL 	for(width=0;width<399;width++)
//DEL 		{
//DEL 			memDC.MoveTo(width+30,150+(int)(singlelargecurve[width]));
//DEL 			memDC.LineTo(width+30+1,150+(int)(singlelargecurve[width+1]));
//DEL 		}
//DEL 
//DEL 	pDC->BitBlt(14,14,182,139,&memDC,0,0,SRCCOPY);  
//DEL 	
//DEL 	ReleaseDC(pDC);
//DEL 	// Do not call CDialog::OnPaint() for painting messages
//DEL }


void CSinglescanner::OnGetdata() 
{
	//******************************pci2003*****************************************// 
    DeviceID = 0; //本台计算机系统中只有一台PCI2003
	hDevice=PCI2003_CreateDevice(DeviceID);      //创建设备对象
	if(hDevice==INVALID_HANDLE_VALUE) // 判断设备对象句柄是否有效
    {		
      	MessageBox(_T("创建设备对象失败..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);		
        return ;		
     } 
	
    //取得0号映射寄存器的线性基地址并判断是否成功
   	if(!PCI2003_GetDeviceAddr(hDevice, &LinearAddr, &PhysAddr, 0))
	{
        MessageBox(_T("取得设备地址失败..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);				
	    return ;
	}

	//对硬件AD参数进行预置
    Para1.FirstChannel = 0;                  //置首通道为0
	Para1.LastChannel = 0;                   //置末通道为0
	Para1.TriggerSource = PCI2003_IN_TRIGGER;//置AD启动模式为内部定时触发
	Para1.Gains = 1;                         //置硬件放大增益为1倍
	Para1.Frequency = 90000;                 //置AD采样频率为90KHz  
    //******************************pci2003*****************************************//


	singleoldzmax=(float)3.0;
	singleoldzmin=(float)1.0;
	singlescanendflag=FALSE;
	singlequickstopflag=FALSE;
	singlesettimerflag=TRUE;
	singleredrawflag=FALSE;

	for(i=0;i<400;i++)
		singlelargecurve[i]=0;
	for(i=0;i<200;i++)
		singlesmallcurve[i]=0;

//置探针到偏移位置处
//	int datahigh,datalow;
	//移动X方向，使之到达偏移位置处，定义D/A1为x方向
	if (singlepianzhivoltagex!=0.0)
		{
	 //**************************************pci2003**********************************************//
     DAdata=(WORD)(((singlepianzhivoltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
	 PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);   // 设置DA控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
     PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);   //0x300为DA控制寄存器
     PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //输出DA0值
     //**************************************pci2003**********************************************//
		}
	//移动Y方向，使之到达偏移位置处，定义D/A2为y方向
	if (singlepianzhivoltagey!=0.0)
		{	
     //**************************************pci2003**********************************************//
     DAdata=(WORD)(((singlepianzhivoltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
	 PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);   // 设置DA控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
     PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);   //0x300为DA控制寄存器
     PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //输出DA1值
	 //**************************************pci2003**********************************************//
		}
	m_goback.EnableWindow(FALSE);
	m_capture.EnableWindow(FALSE);
	m_stop.EnableWindow(TRUE);
	m_getdata.EnableWindow(FALSE);

	//获取计算机高精度计时器的频率
	if(QueryPerformanceFrequency(&litmp)==0)
	{

		MessageBox(_T("此计算机硬件不支持高精度计数器"),
				_T("硬件不支持"), MB_ICONEXCLAMATION | MB_OK);

		return;
	}

	singlefreq=(double)(litmp.QuadPart);

	//计算while循环所需的时间
	LONGLONG begintime,endtime;
	QueryPerformanceCounter(&litmp);
	begintime=litmp.QuadPart;
			while(1)
		{
	QueryPerformanceCounter(&litmp);
	endtime=litmp.QuadPart;
	if ((double)(endtime-begintime)/singlefreq>0.0)
		break;
		}
	QueryPerformanceCounter(&litmp);
	endtime=litmp.QuadPart;
	singlesystemdelay=(double)(endtime-begintime)/singlefreq;

  if (!SetTimer(1,5,NULL))
	{
		MessageBox(_T("此窗口没有足够的定时器"),
				_T("请重新启动计算机"), MB_ICONEXCLAMATION | MB_OK);

		return ;
	}

}

BOOL CSinglescanner::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_getdata.EnableWindow(TRUE);
	m_goback.EnableWindow(TRUE);
	m_capture.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSinglescanner::OnTimer(UINT nIDEvent) 
{

	singlescanendflag=FALSE;
	KillTimer(1);
	CClientDC dc(this);
	dc.SetMapMode(MM_TEXT);

	CPen pencurve(PS_SOLID,1,RGB(255,0,0));
	CPen *oldpencurve=dc.SelectObject(&pencurve);

	toolarge=FALSE;


	//**************************************pci2003**********************************************//
    LONG PerLsbVolt;    //定义一个足够宽度的整型变量存放单位电压值(uV)  
	register ULONG Value;
	PerLsbVolt = (LONG)((10000.0/4096)*1000.0); //取得单位电压值，并以uV为单位将其转换成整型值
    //**************************************pci2003**********************************************//



///////////////////////////////////////////////
if(direction==0)
{
	//小图实时扫描
	if (radiobmpsize==0)
	{
		dc.MoveTo(140,250);
		//检查上次扫描是否正常完成，如果正常完成就计算在值并使捕获图像按钮为能使（其实这个操作仅检查是否第一次扫描正常完成）
		if (height==100&&width==200)
		{

			tempmax=tempmin=singlesmallbmpdata[0][0];
			//量化数据并计算z平均、最大和最小
			singlezmax=singlezmin=singlesmallbmpdata[0][0];
			singlezavetemp=0;
			for (height=0;height<10;height++)
			{
				for (width=10;width<200;width++)
				{
					//以上一幅图像中的z最大和z最小值为上下界划分为255等分，进行量化
					if (singlesmallbmpdata[width][99-height]>singleoldzmax)
					singletempbmpbits=singleoldzmax;
					if (singlesmallbmpdata[width][99-height]<singleoldzmin)
					singletempbmpbits=singleoldzmin;
					if (singlesmallbmpdata[width][99-height]<=singleoldzmax&&singlesmallbmpdata[width][99-height]>=singleoldzmin)
					singletempbmpbits=singlesmallbmpdata[width][99-height];
			
					singlesmallbmpbits[width][height]=(BYTE)(255*(singletempbmpbits-singleoldzmin)/(singleoldzmax-singleoldzmin));
					singlezavetemp=singlezavetemp+singlesmallbmpdata[height][width];
					if (singlezmax<=singlesmallbmpdata[height][width])
					{
						singlezmax=singlesmallbmpdata[height][width];
					}
					if (singlezmin>=singlesmallbmpdata[height][width])
					{
						singlezmin=singlesmallbmpdata[height][width];
					}
				}
			}
			singlezave=(float)(singlezavetemp/10000.0);
			singleoldzmin=singlezmin;
			//以当前图像中z最大值的百分比作为上一幅图像的z最大值，百分比即为输入的亮度阈值
			singleoldzmax=(float)((singlezmax-singlezmin)*singlelightnessthreshold/100.0+singlezmin);

			//设置捕获图像按钮为能使
			m_capture.EnableWindow(TRUE);
		}
		//计算每一象素点的延时时间	
		singledelaytime=singletime/10000.0 - 10.0*singlesystemdelay;

//**************************************
		//小图实时扫描，发送DA信号，采集AD
		singlevoltagey=singlepianzhivoltagey;
		for(height=0;height<90;height++)
		{
		//***********************************pci2003********************************************//
		DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);     //将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);   //设置DA控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);   //0x300为DA控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //输出DA1值
        //************************************pci2003************************************************//

			singlevoltagex=singlepianzhivoltagex;
	
			if(height%90==0)
				Invalidate();		

			for(width=0;width<200;width++)
			{
				//写入x方向的电压信号
				singlevoltagex=singlevoltagex+singlemovevoltagex;

				//校正压电陶瓷的非线性
				singlevoltagex=singlevoltagex-singlevoltagex*singlemovevoltagex*singleceramicscoefficient;

		//***********************************pci2003**************************************************//
        DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //输出DA0值
        //************************************pci2003****************************************************//


				//根据用户输入的时间，程序延迟一段时间
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
					while(1)
				{
				QueryPerformanceCounter(&litmp);
				end=litmp.QuadPart;
				if ((double)(end-begin)/singlefreq>singledelaytime)
					break;
				}

  		//AD采样	
        //******************************pci2003*********************************************//
        //初始化设备对象中的AD部件,为设备操作就绪有关工作,如预置AD采集通道,采样频率等

			//在外面的循环里终止扫描
			if (singlequickstopflag==TRUE)
			{
			break;
			}					
					
		if(!PCI2003_InitDeviceProAD(hDevice, &Para1)) 
		{
			MessageBox(_T("不明确的初始化错误..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);
		return;
		}
        //判断AD转换是否完成,D0=0: 正在转换 D0=1: 完成
        do
		{
			Value = PCI2003_ReadRegisterULong(hDevice,LinearAddr,0x260)&0x00000001; 
		}
		while(Value!=1); 
		
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x00);//读Fifo信号,
		pADBuffer[0]=(WORD)PCI2003_ReadRegisterULong(hDevice, LinearAddr,0x270); 
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x01);

		if(!PCI2003_ReleaseDeviceProAD( hDevice )) //释放设备上的AD部件
		{
			MessageBox(_T("释放AD部件失败"),
				_T(""), MB_ICONEXCLAMATION | MB_OK);
		return;
		}
		
		WORD aaa=pADBuffer[0];
		if((aaa&0x0FFF)>4095)
		{
		   pADBuffer[0]=4095;
		}
		if((aaa&0x0FFF)<2048)
		{
			pADBuffer[0]=2048;
		}

		//计算实际采样电压值,电压单位为V
        Volt=(float)(((pADBuffer[0]&0x0FFF)*PerLsbVolt-5000000.0)/1000000.0);		
	    //******************************pci2003*********************************************//

	    singlesmallbmpdata[width][height]=Volt; 
		if (singlesmallbmpdata[width][height]<=0.0)
					{
					singlesmallbmpdata[width][height]=0.0;
					}

	if(height%89==0)
	{
		singlexaxis=0.0;

		if(width==199)
		{
			dc.MoveTo(140,250);
			dc.LineTo(140+199,250);
			for(i=9;i<200;i++)
			{
				if(tempmax<=singlesmallbmpdata[i][height])
					tempmax=singlesmallbmpdata[i][height];
				if(tempmin>=singlesmallbmpdata[i][height])
					tempmin=singlesmallbmpdata[i][height];
			}

			for(i=9;i<200;i++)
				singlexaxis=singlexaxis+singlesmallbmpdata[i][height];

			singlexaxis=singlexaxis/171;

			if((((tempmax-singlexaxis)*81)>72)||(((singlexaxis-tempmin)*81>72)))
				toolarge=TRUE;
			
			dc.MoveTo(140,250);

			if(toolarge==FALSE)
			{
				for(i=0;i<200;i++)
				{
					singlesmallcurve[i]=(int)((singlesmallbmpdata[i][height]-singlexaxis)*81);
					if(singlesmallcurve[i]>90)
						singlesmallcurve[i]=90;
					if(singlesmallcurve[i]<-90)
						singlesmallcurve[i]=-90;

					//在画图区画点
					dc.LineTo(i+140,250-singlesmallcurve[i]);
					dc.MoveTo(i+140,250-singlesmallcurve[i]);
				}
			}
			else
			{
				for(i=0;i<200;i++)
				{
					tempv=(tempmax-singlexaxis)>(singlexaxis-tempmin)?(tempmax-singlexaxis):(singlexaxis-tempmin);
					singlesmallcurve[i]=(int)(72*(singlesmallbmpdata[i][height]-singlexaxis)/tempv);

					if(singlesmallcurve[i]>90)
						singlesmallcurve[i]=90;
					if(singlesmallcurve[i]<-90)
						singlesmallcurve[i]=-90;

					//在画图区画点
					dc.LineTo(i+140,250-singlesmallcurve[i]);
					dc.MoveTo(i+140,250-singlesmallcurve[i]);
				}
			}

			tempmax=(tempmax-tempmin)*200;
			
			dc.TextOut(140,200-35,"Zmax:");
			sprintf(buffer,"%.1f nm",tempmax);
			dc.TextOut(140+45,200-35,buffer);
						
			dc.TextOut(30,400,"第");
			sprintf(buffer,"%d    ",singleyold);
			dc.TextOut(30+15,400,buffer);
			dc.TextOut(30+40,400,"行扫描");

			tempave=(singlexaxis-tempmin)*200;
			dc.TextOut(140,300+20,"Zave:");
			sprintf(buffer,"%.1f nm",tempave);
			dc.TextOut(140+45,300+20,buffer);
		}
	}
				//捕捉外来消息
				if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
				{
				::TranslateMessage(&message);
				::DispatchMessage(&message);
				}

			}
			//恢复x方向的伸长到最初的x方向偏移位置
			for (i=0;i<width;i++)
			{
				//因为压电陶瓷线性校正后，按其原来步长逐渐降低电压可能会小于原来设定的偏置电压值，所以需要判断语句进行判断
				//线性校正：输入电压不为线性而使得压电陶瓷的伸长量为线性
				if (singlevoltagex > singlepianzhivoltagex)
				{
					singlevoltagex=singlevoltagex-singlemovevoltagex;
				}
				if (singlevoltagex <= singlepianzhivoltagex)
				{
					singlevoltagex=singlepianzhivoltagex;
				}

		//**************************************pci2003**********************************************//        
        DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA1控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA1控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //输出DA1值
		//**************************************pci2003**********************************************//
				//根据用户输入的时间，x方向空回时延迟一段时间
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
					while(1)
					{
				QueryPerformanceCounter(&litmp);
				end=litmp.QuadPart;
				if ((double)(end-begin)/singlefreq>(singledelaytime/4.0))
					break;
					}
			}
			//保险起见，再一次置x方向为最初偏移位置
			singlevoltagex=singlepianzhivoltagex;
		//**************************************pci2003**********************************************//
		DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA1控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA1控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //输出DA1值
		//**************************************pci2003**********************************************//


		if(height%89==0)
		{
			CPen penrect(PS_SOLID,1,RGB(0,0,0));
			CPen *oldpenrect=dc.SelectObject(&penrect);

			dc.MoveTo(140,160);
			dc.LineTo(140,340);
			dc.MoveTo(140,340);
			dc.LineTo(320,340);
			dc.MoveTo(320,340);
			dc.LineTo(320,160);
			dc.MoveTo(320,160);
			dc.LineTo(140,160);

		}
		}

	}


//**************************************


	//大图实时扫描
if (radiobmpsize==1)
{

//检查上次扫描是否正常完成，如果正常完成就计算在值并使捕获图像按钮为能使（其实这个操作仅检查是否第一次扫描正常完成）
	if (height==100&&width==400)
	{
	tempmax=tempmin=singlelargebmpdata[199][99];
	//量化数据并计算z平均、最大和最小
	singlezmax=singlezmin=singlelargebmpdata[199][99];
	singlezavetemp=0;
//		for (height=0;height<100;height++)
//	{
//		singlelargecurve[height]=0;

		for (width=20;width<400;width++)
		{
		//以上一幅图像中的z最大和z最小值为上下界划分为255等分，进行量化
//		if (singlelargebmpdata[width][99-height]>singleoldzmax)
//			singletempbmpbits=singleoldzmax;
//		if (singlelargebmpdata[width][99-height]<singleoldzmin)
//			singletempbmpbits=singleoldzmin;
//		if (singlelargebmpdata[width][99-height]<=singleoldzmax&&singlelargebmpdata[width][99-height]>=singleoldzmin)
//			singletempbmpbits=singlelargebmpdata[width][99-height];
			
//		singlelargebmpbits[height][width]=(BYTE)(255*(singletempbmpbits-singleoldzmin)/(singleoldzmax-singleoldzmin));
//		singlezavetemp=singlezavetemp+singlelargebmpdata[height][width];
		if (singlezmax<=singlelargebmpdata[width][99])
			{
			singlezmax=singlelargebmpdata[width][99];
			}
		if (singlezmin>=singlelargebmpdata[width][99])
			{
			singlezmin=singlelargebmpdata[width][99];
			}
//		singlelargecurve[height]=singlelargecurve[height]+singlelargebmpdata[height][width];
		}
//	}
		singlezave=(float)(singlezavetemp/160000.0);
		singleoldzmin=singlezmin;
		//以当前图像中z最大值的百分比作为上一幅图像的z最大值，百分比即为输入的亮度阈值
//		singleoldzmax=(float)((singlezmax-singlezmin)*singlelightnessthreshold/100.0+singlezmin);
		singleoldzmax=singlezmax;

//设置捕获图像按钮为能使
	m_capture.EnableWindow(TRUE);
	}

//计算每一象素点的延时时间	
	singledelaytime=singletime/160000.0 - 2.0*singlesystemdelay;

//**************************************

//大图实时测量
	singlevoltagey=singlepianzhivoltagey;
	for(height=0;height<100;height++)
	{

		//**************************************pci2003**********************************************//		
	    DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA1控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA1控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //输出DA1值
		//**************************************pci2003**********************************************//

		singlevoltagex=singlepianzhivoltagex;

		if(height%100==0)
			Invalidate();
	
		for(width=0;width<400;width++)
		{
		//写入x方向的电压信号
		singlevoltagex=singlevoltagex+singlemovevoltagex;

		//校正压电陶瓷的非线性
		singlevoltagex=singlevoltagex-singlevoltagex*singlemovevoltagex*singleceramicscoefficient;

		//**************************************pci2003**********************************************//		
		DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA0控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA0控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata);//输出DA0值
		//**************************************pci2003**********************************************//

        //根据用户输入的时间，程序延迟一段时间
		QueryPerformanceCounter(&litmp);
		begin=litmp.QuadPart;
			while(1)
		{
		QueryPerformanceCounter(&litmp);
		end=litmp.QuadPart;
		if ((double)(end-begin)/singlefreq>singledelaytime)
			break;
		}

       //AD采样
       //**************************************pci2003**********************************************//
       //初始化设备对象中的AD部件,为设备操作就绪有关工作,如预置AD采集通道,采样频率等

		//在外面的循环里终止扫描
		if (singlequickstopflag==TRUE)
		{
			break;
		}	
			
		if(!PCI2003_InitDeviceProAD(hDevice, &Para1)) 
		{
			MessageBox(_T("不明确的初始化错误..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);
		return;
		}
        //判断AD转换是否完成,D0=0: 正在转换 D0=1: 完成
		do
		{
			Value = PCI2003_ReadRegisterULong(hDevice,LinearAddr,0x260)&0x00000001; 
		}
		while(Value!=1); 	

		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x00);//读Fifo信号
		pADBuffer[0]=(WORD)PCI2003_ReadRegisterULong(hDevice, LinearAddr, 0x270); 
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x01);

        //释放设备上的AD部件
		if(!PCI2003_ReleaseDeviceProAD( hDevice ))  
		{
			MessageBox(_T("释放AD部件失败"),
				_T(""), MB_ICONEXCLAMATION | MB_OK);

		return;
		}

        WORD aaa=pADBuffer[0];
		if((aaa&0x0FFF)>4095)
		{
		   pADBuffer[0]=4095;
		}
		if((aaa&0x0FFF)<2048)
		{
			pADBuffer[0]=2048;
		}
		//计算实际采样电压值
        Volt=(float)(((pADBuffer[0]&0x0FFF)*PerLsbVolt-5000000.0)/1000000.0);
		//**************************************pci2003**********************************************//
		
		singlelargebmpdata[width][height]=Volt;
		if (singlelargebmpdata[width][height]<=0.0)
			{
			singlelargebmpdata[width][height]=0.0;
			}

			
	if(height%99==0)
	{
		singlexaxis=0.0;

		if(width==399)
		{
			dc.MoveTo(30,225);
			dc.LineTo(30+399,225);
			for(i=20;i<400;i++)
			{
				if(tempmax<=singlelargebmpdata[i][height])
					tempmax=singlelargebmpdata[i][height];
				if(tempmin>=singlelargebmpdata[i][height])
					tempmin=singlelargebmpdata[i][height];
			}

			for(i=20;i<400;i++)
				singlexaxis=singlexaxis+singlelargebmpdata[i][height];

			singlexaxis=singlexaxis/380;

			if((((tempmax-singlexaxis)*200)>160)||(((singlexaxis-tempmin)*200>160)))
				toolarge=TRUE;
			
			dc.MoveTo(30,225);

			if(toolarge==FALSE)
			{
				for(i=0;i<400;i++)
				{
					singlelargecurve[i]=(int)((singlelargebmpdata[i][height]-singlexaxis)*200);
					if(singlelargecurve[i]>200)
						singlelargecurve[i]=200;
					if(singlelargecurve[i]<-200)
						singlelargecurve[i]=-200;

					//在画图区画点
					dc.LineTo(i+30,225-singlelargecurve[i]);
					dc.MoveTo(i+30,225-singlelargecurve[i]);
				}
			}
			else
			{
				for(i=0;i<400;i++)
				{
					tempv=(tempmax-singlexaxis)>(singlexaxis-tempmin)?(tempmax-singlexaxis):(singlexaxis-tempmin);
					singlelargecurve[i]=(int)(160*(singlelargebmpdata[i][height]-singlexaxis)/tempv);

					if(singlelargecurve[i]>200)
						singlelargecurve[i]=200;
					if(singlelargecurve[i]<-200)
						singlelargecurve[i]=-200;

					//在画图区画点
					dc.LineTo(i+30,225-singlelargecurve[i]);
					dc.MoveTo(i+30,225-singlelargecurve[i]);
				}
			}
			
			tempmax=(tempmax-tempmin)*200;
			
			dc.TextOut(30,200-45-25,"Zmax:");
			sprintf(buffer,"%.1f nm",tempmax);
			dc.TextOut(30+45,200-45-25,buffer);
						
			dc.TextOut(30,400,"第");
			sprintf(buffer,"%d    ",singleyold);
			dc.TextOut(30+15,400,buffer);
			dc.TextOut(30+40,400,"行扫描");

			tempave=(singlexaxis-tempmin)*200;
			dc.TextOut(30,300+30-25,"Zave:");
			sprintf(buffer,"%.1f nm",tempave);
			dc.TextOut(30+45,300+30-25,buffer);
		}
	}
		//捕捉外来消息
		if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
		::TranslateMessage(&message);
		::DispatchMessage(&message);
		}

		}
		//恢复x方向的伸长到最初的x方向偏移位置
		for (i=0;i<width;i++)
		{

		    //因为压电陶瓷线性校正后，按其原来步长逐渐降低电压可能会小于原来设定的偏置电压值，所以需要判断语句进行判断
			//线性校正：输入电压不为线性而使得压电陶瓷的伸长量为线性
			if (singlevoltagex > singlepianzhivoltagex)
			{
				singlevoltagex=singlevoltagex-singlemovevoltagex;
			}
			if (singlevoltagex <= singlepianzhivoltagex)
			{
				singlevoltagex=singlepianzhivoltagex;
			}

		//**************************************pci2003**********************************************//
        DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA0控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA0控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //输出DA0值
		//**************************************pci2003**********************************************//	    

		//根据用户输入的时间，x方向空回时延迟一段时间
		QueryPerformanceCounter(&litmp);
		begin=litmp.QuadPart;
			while(1)
			{
		QueryPerformanceCounter(&litmp);
		end=litmp.QuadPart;
		if ((double)(end-begin)/singlefreq>(singledelaytime/4.0))
			break;
			}
		}
		//保险起见，再一次置x方向为最初偏移位置
		singlevoltagex=singlepianzhivoltagex;
		//**************************************pci2003**********************************************//
		DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA0控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA0控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //输出DA0值	   
		//**************************************pci2003**********************************************//


	}
}
	


//**************************************
}
if(direction==1)
{
	//小图实时扫描
	if (radiobmpsize==0)
	{
		dc.MoveTo(225,140);
		//检查上次扫描是否正常完成，如果正常完成就计算在值并使捕获图像按钮为能使（其实这个操作仅检查是否第一次扫描正常完成）
		if (height==200&&width==90)
		{
			tempmax=tempmin=singlesmallbmpdata[0][0];
			//量化数据并计算z平均、最大和最小
			singlezmax=singlezmin=singlesmallbmpdata[0][0];
			singlezavetemp=0;
			for (width=0;width<90;width++)
			{
				for (height=10;height<200;height++)
				{
					//以上一幅图像中的z最大和z最小值为上下界划分为255等分，进行量化
					if (singlesmallbmpdata[height][89-width]>singleoldzmax)
						singletempbmpbits=singleoldzmax;
					if (singlesmallbmpdata[height][89-width]<singleoldzmin)
						singletempbmpbits=singleoldzmin;
					if (singlesmallbmpdata[height][89-width]<=singleoldzmax&&singlesmallbmpdata[height][89-width]>=singleoldzmin)
						singletempbmpbits=singlesmallbmpdata[height][89-width];
						
					singlesmallbmpbits[height][width]=(BYTE)(255*(singletempbmpbits-singleoldzmin)/(singleoldzmax-singleoldzmin));
					singlezavetemp=singlezavetemp+singlesmallbmpdata[height][width];
					if (singlezmax<=singlesmallbmpdata[height][width])
						{
						singlezmax=singlesmallbmpdata[height][width];
						}
					if (singlezmin>=singlesmallbmpdata[height][width])
						{
						singlezmin=singlesmallbmpdata[height][width];
						}
				}
			}
			singlezave=(float)(singlezavetemp/10000.0);
			singleoldzmin=singlezmin;
			//以当前图像中z最大值的百分比作为上一幅图像的z最大值，百分比即为输入的亮度阈值
			singleoldzmax=(float)((singlezmax-singlezmin)*singlelightnessthreshold/100.0+singlezmin);

			//设置捕获图像按钮为能使
			m_capture.EnableWindow(TRUE);
		}
		//计算每一象素点的延时时间	
		singledelaytime=singletime/10000.0 - 10.0*singlesystemdelay;

//**************************************
//小图实时扫描，发送DA信号，采集AD
		singlevoltagex=singlepianzhivoltagex;
		for(width=0;width<90;width++)
		{
		//***********************************pci2003********************************************//
		DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);     //将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);   //设置DA控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);   //0x300为DA控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //输出DA1值
        //************************************pci2003************************************************//

			singlevoltagey=singlepianzhivoltagey;

			if(width%90==0)
				Invalidate();
			for(height=0;height<200;height++)
			{
		
				//写入y方向的电压信号
				singlevoltagey=singlevoltagey+singlemovevoltagey;

				//校正压电陶瓷的非线性
				singlevoltagey=singlevoltagey-singlevoltagey*singlemovevoltagey*singleceramicscoefficient;

		//***********************************pci2003**************************************************//
        DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //输出DA0值
        //************************************pci2003****************************************************//

				//根据用户输入的时间，程序延迟一段时间
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
					while(1)
				{
				QueryPerformanceCounter(&litmp);
				end=litmp.QuadPart;
				if ((double)(end-begin)/singlefreq>singledelaytime)
					break;
				}
 
		//AD采样	
        //******************************pci2003*********************************************//
        //初始化设备对象中的AD部件,为设备操作就绪有关工作,如预置AD采集通道,采样频率等

			//在外面的循环里终止扫描
			if (singlequickstopflag==TRUE)
			{
			break;
			}					
					
		if(!PCI2003_InitDeviceProAD(hDevice, &Para1)) 
		{
			MessageBox(_T("不明确的初始化错误..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);
		return;
		}
        //判断AD转换是否完成,D0=0: 正在转换 D0=1: 完成
        do
		{
			Value = PCI2003_ReadRegisterULong(hDevice,LinearAddr,0x260)&0x00000001; 
		}
		while(Value!=1); 
		
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x00);//读Fifo信号,
		pADBuffer[0]=(WORD)PCI2003_ReadRegisterULong(hDevice, LinearAddr,0x270); 
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x01);

		if(!PCI2003_ReleaseDeviceProAD( hDevice )) //释放设备上的AD部件
		{
			MessageBox(_T("释放AD部件失败"),
				_T(""), MB_ICONEXCLAMATION | MB_OK);
		return;
		}
		
		WORD aaa=pADBuffer[0];
		if((aaa&0x0FFF)>4095)
		{
		   pADBuffer[0]=4095;
		}
		if((aaa&0x0FFF)<2048)
		{
			pADBuffer[0]=2048;
		}

		//计算实际采样电压值,电压单位为V
        Volt=(float)(((pADBuffer[0]&0x0FFF)*PerLsbVolt-5000000.0)/1000000.0);		
	    //******************************pci2003*********************************************//
		       
		        singlesmallbmpdata[width][height]=Volt; 
				if (singlesmallbmpdata[width][height]<=0.0)
					{
					singlesmallbmpdata[width][height]=0.0;
					}

	if(width%89==0)
	{
		singleyaxis=0.0;

		if(height==199)
		{
			dc.MoveTo(225,140);
			dc.LineTo(225,140+199);
			for(i=9;i<200;i++)
			{
				if(tempmax<=singlesmallbmpdata[width][i])
					tempmax=singlesmallbmpdata[width][i];
				if(tempmin>=singlesmallbmpdata[width][i])
					tempmin=singlesmallbmpdata[width][i];
			}

			for(i=9;i<200;i++)
				singleyaxis=singleyaxis+singlesmallbmpdata[width][i];

			singleyaxis=singleyaxis/171;

			if((((tempmax-singleyaxis)*81)>72)||(((singleyaxis-tempmin)*81>72)))
				toolarge=TRUE;
			
			dc.MoveTo(225,140);

			if(toolarge==FALSE)
			{
				for(i=0;i<200;i++)
				{
					singlesmallcurve[i]=(int)((singlesmallbmpdata[width][i]-singleyaxis)*81);
					if(singlesmallcurve[i]>90)
						singlesmallcurve[i]=90;
					if(singlesmallcurve[i]<-90)
						singlesmallcurve[i]=-90;

					//在画图区画点
					dc.LineTo(225+singlesmallcurve[i],140+i);
					dc.MoveTo(225+singlesmallcurve[i],140+i);
				}
			}
			else
			{
				for(i=0;i<200;i++)
				{
					tempv=(tempmax-singleyaxis)>(singleyaxis-tempmin)?(tempmax-singleyaxis):(singleyaxis-tempmin);
					singlesmallcurve[i]=(int)(72*(singlesmallbmpdata[width][i]-singleyaxis)/tempv);

					if(singlesmallcurve[i]>90)
						singlesmallcurve[i]=90;
					if(singlesmallcurve[i]<-90)
						singlesmallcurve[i]=-90;

					//在画图区画点
					dc.LineTo(225+singlesmallcurve[i],140+i);
					dc.MoveTo(225+singlesmallcurve[i],140+i);
				}
			}
			
						tempmax=(tempmax-tempmin)*200;
						
						dc.TextOut(225+45,140-20,"Zmax:");
						sprintf(buffer,"%.1f nm",tempmax);
						dc.TextOut(225+45+45,140-20,buffer);
						
						dc.TextOut(30,400,"第");
						sprintf(buffer,"%d    ",singlexold);
						dc.TextOut(30+15,400,buffer);
						dc.TextOut(30+40,400,"列扫描");

						tempave=(singleyaxis-tempmin)*200;
						dc.TextOut(225-135,140-20,"Zave:");
						sprintf(buffer,"%.1f nm",tempave);
						dc.TextOut(225-90,140-20,buffer);
		}
	}

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
				if (singlevoltagey> singlepianzhivoltagey)
				{
					singlevoltagey=singlevoltagey-singlemovevoltagey;
				}
				if (singlevoltagey <= singlepianzhivoltagey)
				{
					singlevoltagey=singlepianzhivoltagey;
				}

        //*****************************************pci2003******************************************//
        DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA0控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA0控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //输出DA0值
        //*****************************************pci2003******************************************//
				
		        //根据用户输入的时间，y方向空回时延迟一段时间
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
					while(1)
					{
				QueryPerformanceCounter(&litmp);
				end=litmp.QuadPart;
				if ((double)(end-begin)/singlefreq>(singledelaytime/4.0))
					break;
					}
			}
			//保险起见，再一次置y方向为最初偏移位置
			singlevoltagey=singlepianzhivoltagey;
		//**************************************pci2003**********************************************//
		DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA0控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA0控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //输出DA0值
        //***************************************pci2003*********************************************//


		if(width%89==0)
		{
			CPen penrect(PS_SOLID,1,RGB(0,0,0));
			CPen *oldpenrect=dc.SelectObject(&penrect);

			dc.MoveTo(135,140);
			dc.LineTo(315,140);
			dc.MoveTo(315,140);
			dc.LineTo(315,320);
			dc.MoveTo(315,320);
			dc.LineTo(135,320);
			dc.MoveTo(135,320);
			dc.LineTo(135,140);
			
		}
	}
//**************************************
}

	//大图实时扫描
if (radiobmpsize==1)
{
	dc.MoveTo(225,30);

	//检查上次扫描是否正常完成，如果正常完成就计算在值并使捕获图像按钮为能使（其实这个操作仅检查是否第一次扫描正常完成）
	if (height==400&&width==100)
	{
		tempmax=tempmin=singlelargebmpdata[0][0];
		//量化数据并计算z平均、最大和最小
		singlezmax=singlezmin=singlelargebmpdata[0][0];
		singlezavetemp=0;
		for (height=0;height<400;height++)
		{
			for (width=20;width<100;width++)
			{
				//以上一幅图像中的z最大和z最小值为上下界划分为255等分，进行量化
				if (singlelargebmpdata[width][99-height]>singleoldzmax)
					singletempbmpbits=singleoldzmax;
				if (singlelargebmpdata[width][99-height]<singleoldzmin)
					singletempbmpbits=singleoldzmin;
				if (singlelargebmpdata[width][99-height]<=singleoldzmax&&singlelargebmpdata[width][99-height]>=singleoldzmin)
					singletempbmpbits=singlelargebmpdata[width][99-height];
			
				singlelargebmpbits[height][width]=(BYTE)(255*(singletempbmpbits-singleoldzmin)/(singleoldzmax-singleoldzmin));
				singlezavetemp=singlezavetemp+singlelargebmpdata[height][width];
				if (singlezmax<=singlelargebmpdata[height][width])
					{
					singlezmax=singlelargebmpdata[height][width];
					}
				if (singlezmin>=singlelargebmpdata[height][width])
					{
					singlezmin=singlelargebmpdata[height][width];
					}
			}
		}
		singlezave=(float)(singlezavetemp/160000.0);
		singleoldzmin=singlezmin;
		//以当前图像中z最大值的百分比作为上一幅图像的z最大值，百分比即为输入的亮度阈值
		singleoldzmax=(float)((singlezmax-singlezmin)*singlelightnessthreshold/100.0+singlezmin);

//设置捕获图像按钮为能使
		m_capture.EnableWindow(TRUE);
	}

//计算每一象素点的延时时间	
	singledelaytime=singletime/160000.0 - 2.0*singlesystemdelay;

//**************************************

//大图实时测量
	singlevoltagex=singlepianzhivoltagex;
	for(width=0;width<100;width++)
	{
		//**************************************pci2003**********************************************//		
	    DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA1控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA1控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //输出DA1值
		//**************************************pci2003**********************************************//

		singlevoltagey=singlepianzhivoltagey;

		if(width%100==0)
			Invalidate();
	
		for(height=0;height<400;height++)
		{
			//写入y方向的电压信号
			singlevoltagey=singlevoltagey+singlemovevoltagey;

			//校正压电陶瓷的非线性
			singlevoltagey=singlevoltagey-singlevoltagey*singlemovevoltagey*singleceramicscoefficient;

		//**************************************pci2003**********************************************//		
		DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA0控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA0控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata);//输出DA0值
		//**************************************pci2003**********************************************//

			//根据用户输入的时间，程序延迟一段时间
			QueryPerformanceCounter(&litmp);
			begin=litmp.QuadPart;
				while(1)
			{
			QueryPerformanceCounter(&litmp);
			end=litmp.QuadPart;
			if ((double)(end-begin)/singlefreq>singledelaytime)
				break;
			}

       //AD采样
       //**************************************pci2003**********************************************//
       //初始化设备对象中的AD部件,为设备操作就绪有关工作,如预置AD采集通道,采样频率等
 
		//在外面的循环里终止扫描
		if (singlequickstopflag==TRUE)
		{
			break;
		}		
				
		if(!PCI2003_InitDeviceProAD(hDevice, &Para1)) 
		{
			MessageBox(_T("不明确的初始化错误..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);
		return;
		}
        //判断AD转换是否完成,D0=0: 正在转换 D0=1: 完成
		do
		{
			Value = PCI2003_ReadRegisterULong(hDevice,LinearAddr,0x260)&0x00000001; 
		}
		while(Value!=1); 	

		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x00);//读Fifo信号
		pADBuffer[0]=(WORD)PCI2003_ReadRegisterULong(hDevice, LinearAddr, 0x270); 
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x01);

        //释放设备上的AD部件
		if(!PCI2003_ReleaseDeviceProAD( hDevice ))  
		{
			MessageBox(_T("释放AD部件失败"),
				_T(""), MB_ICONEXCLAMATION | MB_OK);

		return;
		}

        WORD aaa=pADBuffer[0];
		if((aaa&0x0FFF)>4095)
		{
		   pADBuffer[0]=4095;
		}
		if((aaa&0x0FFF)<2048)
		{
			pADBuffer[0]=2048;
		}
		//计算实际采样电压值
        Volt=(float)(((pADBuffer[0]&0x0FFF)*PerLsbVolt-5000000.0)/1000000.0);
		//**************************************pci2003**********************************************//

		singlelargebmpdata[width][height]=Volt;
		if (singlelargebmpdata[width][height]<=0.0)
				{
				singlelargebmpdata[width][height]=0.0;
				}

	if(width%99==0)
	{
		singleyaxis=0.0;

		if(height==399)
		{
			dc.MoveTo(225,30);
			dc.LineTo(225,430);

			for(i=20;i<400;i++)
			{
				if(tempmax<=singlelargebmpdata[width][i])
					tempmax=singlelargebmpdata[width][i];
				if(tempmin>=singlelargebmpdata[width][i])
					tempmin=singlelargebmpdata[width][i];
			}

			for(i=20;i<400;i++)
				singleyaxis=singleyaxis+singlelargebmpdata[width][i];

			singleyaxis=singleyaxis/380;

			if((((tempmax-singleyaxis)*200)>160)||(((singleyaxis-tempmin)*200>160)))
				toolarge=TRUE;
			
			dc.MoveTo(225,30);

			if(toolarge==FALSE)
			{
				for(i=0;i<400;i++)
				{
					singlelargecurve[i]=(int)((singlelargebmpdata[width][i]-singleyaxis)*200);
					if(singlelargecurve[i]>200)
						singlelargecurve[i]=200;
					if(singlelargecurve[i]<-200)
						singlelargecurve[i]=-200;

					//在画图区画点
					dc.LineTo(225+singlelargecurve[i],i+30);
					dc.MoveTo(225+singlelargecurve[i],i+30);
				}
			}
			else
			{
				for(i=0;i<400;i++)
				{
					tempv=(tempmax-singleyaxis)>(singleyaxis-tempmin)?(tempmax-singleyaxis):(singleyaxis-tempmin);
					singlelargecurve[i]=(int)(160*(singlelargebmpdata[width][i]-singleyaxis)/tempv);

					if(singlelargecurve[i]>200)
						singlelargecurve[i]=200;
					if(singlelargecurve[i]<-200)
						singlelargecurve[i]=-200;

					//在画图区画点
					dc.LineTo(225+singlelargecurve[i],i+30);
					dc.MoveTo(225+singlelargecurve[i],i+30);
				}
			}
			
					tempmax=(tempmax-tempmin)*200;
					
					dc.TextOut(225+45,30,"Zmax:");
					sprintf(buffer,"%.1f nm",tempmax);
					dc.TextOut(225+45+45,30,buffer);
					
					dc.TextOut(30,400,"第");
					sprintf(buffer,"%d    ",singlexold);
					dc.TextOut(30+15,400,buffer);
					dc.TextOut(30+40,400,"列扫描");

					tempave=(singleyaxis-tempmin)*200;
					dc.TextOut(225-135,30,"Zave:");
					sprintf(buffer,"%.1f nm",tempave);
					dc.TextOut(225-90,30,buffer);
		}
	}
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
			if (singlevoltagey > singlepianzhivoltagey)
			{
				singlevoltagey=singlevoltagey-singlemovevoltagey;
			}
			if (singlevoltagey <= singlepianzhivoltagey)
			{
				singlevoltagey=singlepianzhivoltagey;
			}

		//**************************************pci2003**********************************************//
        DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA0控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA0控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //输出DA0值
		//**************************************pci2003**********************************************//	    
			
		    //根据用户输入的时间，y方向空回时延迟一段时间
			QueryPerformanceCounter(&litmp);
			begin=litmp.QuadPart;
				while(1)
				{
			QueryPerformanceCounter(&litmp);
			end=litmp.QuadPart;
			if ((double)(end-begin)/singlefreq>(singledelaytime/4.0))
				break;
				}
		}
		//保险起见，再一次置y方向为最初偏移位置
		singlevoltagey=singlepianzhivoltagey;
		//**************************************pci2003**********************************************//
		DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA0控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA0控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //输出DA0值	   
		//**************************************pci2003**********************************************//



		singleyaxis=0.0;
		singleyaxiss=0.0;
		for(i=0;i<400;i++)
		{
			singleyaxiss=(singleyaxiss*i+singlelargebmpdata[height][i]-tempmin)/(i+1);
			singleyaxis=(singleyaxis*i+singlelargecurve[i])/(i+1);
		}


	}
}


//**************************************


}
////////////////////////////////////////////
//取消偏移时加的电压，使压电陶瓷恢复到0长度
 //if (singlepianzhiflag==TRUE)
//	{
	//恢复x方向的压电陶瓷
	 if (singlepianzhivoltagex!=0.0)
		{
        DAdata=0;
		//**************************************pci2003**********************************************//
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);  // 设置DA0控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);  //0x300为DA0控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata);//输出DA0值
		//**************************************pci2003**********************************************//
		}
	 //恢复y方向的压电陶瓷
	 if (singlepianzhivoltagey!=0.0)
		{
		DAdata=0;
		//**************************************pci2003**********************************************//
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);   // 设置DA1控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);   //0x300为DA1控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //输出DA1值
		//**************************************pci2003**********************************************//
		}
//	}

	Invalidate();
//设置时间，进行下一次循环
	if (singlesettimerflag==TRUE)
	{
	SetTimer(1,5,NULL);
	}
//扫描结束标志
	singlescanendflag=TRUE;

}

void CSinglescanner::OnStop() 
{
	singlesettimerflag=FALSE;
	singleredrawflag=TRUE;
	singlequickstopflag=TRUE;
//	singlepianzhiflag=TRUE;

//设置对话框上按钮的能使
	m_goback.EnableWindow(TRUE);	
	m_getdata.EnableWindow(TRUE);
	m_stop.EnableWindow(FALSE);
	KillTimer(1);
}

void CSinglescanner::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
if(singleredrawflag==TRUE)
{
	CPen Pen;
	if(direction==0)
	{
		if(radiobmpsize==0)
		{
			Pen.CreatePen(PS_SOLID,1,RGB(0,0,0));                                     
			CPen * pPen=dc.SelectObject(&Pen);                                           

			dc.MoveTo(140,250);                                                                 
			dc.LineTo(140+199,250);                                                                 

			CPen pencoordinateout(PS_SOLID,1,RGB(255,0,0));
		//	Pen.CreatePen(PS_SOLID,1,RGB(255,0,0));                                     
			CPen *oldpencoordinateout=dc.SelectObject(&pencoordinateout);

		
			for(width=0;width<1799;width++)
				{
					dc.MoveTo(width+140,250-(int)(singlesmallcurve[width])+(int)(singlexaxis));
					dc.LineTo(width+140+1,250-(int)(singlesmallcurve[width+1])+(int)(singlexaxis));
				}
			dc.TextOut(140,200-35,"Zmax:");
			sprintf(buffer,"%.1f nm",tempmax);
			dc.TextOut(140+45,200-35,buffer);
						
			dc.TextOut(140,300+20,"Zave:");
			sprintf(buffer,"%.1f nm",tempave);
			dc.TextOut(140+45,300+20,buffer);

			dc.TextOut(30,400,"第");
			sprintf(buffer,"%d    ",singleyold);
			dc.TextOut(30+15,400,buffer);
			dc.TextOut(30+40,400,"行扫描");

			CPen penrect(PS_SOLID,1,RGB(0,0,0));
			CPen *oldpenrect=dc.SelectObject(&penrect);

			dc.MoveTo(140,160);
			dc.LineTo(140,340);
			dc.MoveTo(140,340);
			dc.LineTo(320,340);
			dc.MoveTo(320,340);
			dc.LineTo(320,160);
			dc.MoveTo(320,160);
			dc.LineTo(140,160);
		}
		if(radiobmpsize==1)
		{
			Pen.CreatePen(PS_SOLID,1,RGB(0,0,0));                                     
			CPen * pPen=dc.SelectObject(&Pen);                                           

			dc.MoveTo(30,225);                                                                 
			dc.LineTo(30+399,225);                                                                 

			CPen pencoordinateout(PS_SOLID,1,RGB(255,0,0));
		//	Pen.CreatePen(PS_SOLID,1,RGB(255,0,0));                                     
			CPen *oldpencoordinateout=dc.SelectObject(&pencoordinateout);

		
			for(width=0;width<399;width++)
				{
					dc.MoveTo(width+30,225-(int)(singlelargecurve[width])+(int)(singlexaxis));
					dc.LineTo(width+30+1,225-(int)(singlelargecurve[width+1])+(int)(singlexaxis));
				}
			dc.TextOut(30,200-45-25,"Zmax:");
			sprintf(buffer,"%.1f nm",tempmax);
			dc.TextOut(30+45,200-45-25,buffer);
						
			dc.TextOut(30,300+30-25,"Zave:");
			sprintf(buffer,"%.1f v",tempave);
			dc.TextOut(30+45,300+30-25,buffer);

			dc.TextOut(30,400,"第");
			sprintf(buffer,"%d    ",singleyold);
			dc.TextOut(30+15,400,buffer);
			dc.TextOut(30+40,400,"行扫描");
		}
	}
	if(direction==1)
	{
		if(radiobmpsize==0)
		{
			Pen.CreatePen(PS_SOLID,1,RGB(0,0,0));                                     
			CPen * pPen=dc.SelectObject(&Pen);                                           

			dc.MoveTo(225,140);                                                                 
			dc.LineTo(225,320);                                                                 

			CPen pencoordinateout(PS_SOLID,1,RGB(255,0,0));
			CPen *oldpencoordinateout=dc.SelectObject(&pencoordinateout);

			for(width=0;width<199;width++)
				{
					dc.MoveTo(225+(int)(singlesmallcurve[width])-(int)(singleyaxis),140+width);
					dc.LineTo(225+(int)(singlesmallcurve[width+1])-(int)(singleyaxis),140+width+1);
				}
			dc.TextOut(225+45,140-20,"Zmax:");
			sprintf(buffer,"%.1f nm",tempmax);
			dc.TextOut(225+45+45,140-20,buffer);
						
			dc.TextOut(225-135,140-20,"Zave:");
			sprintf(buffer,"%.1f nm",tempave);
			dc.TextOut(225-90,140-20,buffer);

			dc.TextOut(30,400,"第");
			sprintf(buffer,"%d    ",singlexold);
			dc.TextOut(30+15,400,buffer);
			dc.TextOut(30+40,400,"列扫描");

			CPen penrect(PS_SOLID,1,RGB(0,0,0));
			CPen *oldpenrect=dc.SelectObject(&penrect);

			dc.MoveTo(135,140);
			dc.LineTo(315,140);
			dc.MoveTo(315,140);
			dc.LineTo(315,320);
			dc.MoveTo(315,320);
			dc.LineTo(135,320);
			dc.MoveTo(135,320);
			dc.LineTo(135,140);
		}
		if(radiobmpsize==1)
		{
			Pen.CreatePen(PS_SOLID,1,RGB(0,0,0));                                     
			CPen * pPen=dc.SelectObject(&Pen);                                           

			dc.MoveTo(225,30);
			dc.LineTo(225,430);

			CPen pencoordinateout(PS_SOLID,1,RGB(255,0,0));
			CPen *oldpencoordinateout=dc.SelectObject(&pencoordinateout);

			for(width=0;width<399;width++)
				{
					dc.MoveTo(225+(int)(singlelargecurve[width])-(int)(singleyaxis),30+width);
					dc.LineTo(225+(int)(singlelargecurve[width+1])-(int)(singleyaxis),30+width+1);
				}
				dc.TextOut(225+45,30,"Zmax:");
				sprintf(buffer,"%.1f nm",tempmax);
				dc.TextOut(225+45+45,30,buffer);
					
				dc.TextOut(225-135,30,"Zave:");
				sprintf(buffer,"%.1f nm",tempave);
				dc.TextOut(225-90,30,buffer);

			dc.TextOut(30,400,"第");
			sprintf(buffer,"%d    ",singlexold);
			dc.TextOut(30+15,400,buffer);
			dc.TextOut(30+40,400,"列扫描");
		}
	}
}

}
