// Atomforce.cpp : implementation file
//

#include "stdafx.h"
#include "microscope.h"
#include "microscopeDoc.h"
#include "Atomforce.h"
#include "conio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAtomforce dialog

PCI2003_PARA_AD  Para2;

CAtomforce::CAtomforce(CWnd* pParent /*=NULL*/)
	: CDialog(CAtomforce::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAtomforce)
	//}}AFX_DATA_INIT
}


void CAtomforce::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAtomforce)
	DDX_Control(pDX, IDC_CURVEIMAGE, m_curveimage);
	DDX_Control(pDX, IDC_CURVECOLOR, m_curvecolor);
	DDX_Control(pDX, IDC_COORDINATECOLOR, m_coordinatecolor);
	DDX_Control(pDX, IDC_BACKGROUNDCOLOR, m_backgroundcolor);
	DDX_Control(pDX, IDC_SLIDERTIME, m_slidertime);
	DDX_Control(pDX, IDC_SLIDERRANGE, m_sliderrange);
	DDX_Control(pDX, IDC_SHOWRANGE, m_showrange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAtomforce, CDialog)
	//{{AFX_MSG_MAP(CAtomforce)
	ON_BN_CLICKED(IDC_BUTTONGOBACK, OnButtongoback)
	ON_WM_HSCROLL()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTONCAPTURE, OnButtoncapture)
	ON_BN_CLICKED(IDC_BUTTONSTART, OnButtonstart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAtomforce message handlers

void CAtomforce::OnOK() 
{
//重载OnOk函数，并使其无效，目的是让对话框不响应enter键		
//	CDialog::OnOK();
}

void CAtomforce::OnCancel() 
{
//重载OnCancel函数，并使其无效，目的是让对话框不响应esc键	
//	CDialog::OnCancel();
}

void CAtomforce::OnButtongoback() 
{
	CDialog::OnCancel();
}

BOOL CAtomforce::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//把对话框置于屏幕的中间
	CenterWindow(CWnd::GetDesktopWindow());

	//设置显示测量范围的初始值
	imagerange=range=500;
	sprintf(rangechar," %d     ",range);
	m_showrange.SetWindowText(rangechar);
    	
	//设置测量范围滑块的范围、移动步长和初始值
	m_sliderrange.SetRange(100,1000,TRUE);
	m_sliderrange.SetTicFreq(1);
	m_sliderrange.SetPos(range);

	//设置测量时间的初始值
	//设置时间滑块的范围、移动步长和初始值
	time=300;
	m_slidertime.SetRange(100,500,TRUE);
	m_slidertime.SetTicFreq(10);
	m_slidertime.SetPos(time);

	//设置图像的颜色
	backgroundcolor=0xFFFFFF; //图像背景颜色
	coordinatecolor=0xFF0000; //图像坐标颜色
	curvecolor=0x0000FF; //图像曲线颜色

	//设置坐标轴y的坐标，其它所有的坐标都和此坐标相关
	pointx=345;
	pointy=60;

	//置画曲线的标志为假，这里仅对画曲线起作用，背景和坐标仍然会画上去
	flag=FALSE;

	//初始化原子力大小数据
	for (i=0;i<200;i++)
	{
		forcedata[i]=0.0;
		lastforcedata[i]=pointy+4*90;
	}
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAtomforce::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	//测量时间滚动条
	CSliderCtrl* slidertime=(CSliderCtrl*)GetDlgItem(IDC_SLIDERTIME);
	time=slidertime->GetPos();

	//测量范围滚动条
	CSliderCtrl* sliderrange=(CSliderCtrl*)GetDlgItem(IDC_SLIDERRANGE);
	range=sliderrange->GetPos();
    //更新测量范围滚动条显示值
	sprintf(rangechar," %d     ",range);
	m_showrange.SetWindowText(rangechar);

}

void CAtomforce::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect rect;	
	
	//图像背景颜色
	m_backgroundcolor.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlgbackground(backgroundcolor,CC_FULLOPEN);
		if(dlgbackground.DoModal()==IDOK)
		{
		backgroundcolor=dlgbackground.GetColor();	
		this->InvalidateRect(FALSE);
		}
	}
	
	//坐标颜色
	m_coordinatecolor.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlgcoordinate(coordinatecolor,CC_FULLOPEN);
		if(dlgcoordinate.DoModal()==IDOK)
		{
		coordinatecolor=dlgcoordinate.GetColor();
		this->InvalidateRect(FALSE);
		}
	}

	//曲线颜色
	m_curvecolor.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlgcurve(curvecolor,CC_FULLOPEN);
		if(dlgcurve.DoModal()==IDOK)
		{
		curvecolor=dlgcurve.GetColor();
		this->InvalidateRect(FALSE);
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CAtomforce::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;

	//图像背景颜色选择框颜色改变
	m_backgroundcolor.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushBackgroundselector(backgroundcolor);
	dc.FillRect(&rect,&BrushBackgroundselector);

	//坐标颜色选择框颜色改变
	m_coordinatecolor.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush Brushcoordinateselector(coordinatecolor);
	dc.FillRect(&rect,&Brushcoordinateselector);

	//曲线颜色选择框颜色改变
	m_curvecolor.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush Brushcurveselector(curvecolor);
	dc.FillRect(&rect,&Brushcurveselector);

	//图像背景颜色改变
	m_curveimage.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CBrush Brushbackground(backgroundcolor);
	dc.FillRect(&rect,&Brushbackground);

	//坐标绘制
	//绘制外框
	CPen pencoordinateout(PS_SOLID,2,coordinatecolor);
	CPen *oldpencoordinateout=dc.SelectObject(&pencoordinateout);

	dc.MoveTo(pointx,pointy);
	dc.LineTo(pointx,pointy+360); 
	dc.MoveTo(pointx+358,pointy);
	dc.LineTo(pointx+358,pointy+360); 
	dc.MoveTo(pointx,pointy);
	dc.LineTo(pointx+358,pointy); 
	dc.MoveTo(pointx,pointy+360);
	dc.LineTo(pointx+358,pointy+360); 
	dc.SelectObject(oldpencoordinateout);
    //绘制内框
    dc.SetBkColor(backgroundcolor);
	CPen pencoordinateint(PS_DOT,1,coordinatecolor);
	CPen *oldpencoordinateint=dc.SelectObject(&pencoordinateint);

	dc.MoveTo(pointx,pointy+72);
	dc.LineTo(pointx+358,pointy+72);
	dc.MoveTo(pointx,pointy+2*72);
	dc.LineTo(pointx+358,pointy+2*72);
	dc.MoveTo(pointx,pointy+3*72);
	dc.LineTo(pointx+358,pointy+3*72);
	dc.MoveTo(pointx,pointy+4*72);
	dc.LineTo(pointx+358,pointy+4*72);
	dc.MoveTo(pointx+71,pointy);
	dc.LineTo(pointx+71,pointy+360);
	dc.MoveTo(pointx+2*71+1,pointy);
	dc.LineTo(pointx+2*71+1,pointy+360);
	dc.MoveTo(pointx+3*71+2,pointy);
	dc.LineTo(pointx+3*71+2,pointy+360);
	dc.MoveTo(pointx+4*71+2,pointy);
	dc.LineTo(pointx+4*71+2,pointy+360);
	dc.SelectObject(oldpencoordinateint);
	//写刻度的数值
	dc.SetTextColor(coordinatecolor);
	dc.SetBkColor(backgroundcolor);
	dc.TextOut(pointx-15,pointy-5,"1");
	dc.TextOut(pointx-17,pointy-12+4*90,"-1");
	dc.TextOut(pointx-5,pointy+7+4*90,"起始点");
	sprintf(buffer,"%d nm",imagerange);
	dc.TextOut(pointx+358-45,pointy+5+4*90,buffer);
	
	//曲线绘制
	if (flag==TRUE)
	{
	CPen pencurve(PS_SOLID,1,curvecolor);
	CPen *oldpencurve=dc.SelectObject(&pencurve);

	dc.MoveTo(pointx,pointy+4*90-lastforcedata[0]);
	for (i=1;i<200;i++)
		{
		dc.LineTo(pointx+i*2,pointy+4*90-lastforcedata[i]);
		dc.MoveTo(pointx+i*2,pointy+4*90-lastforcedata[i]);
		}
	dc.SelectObject(oldpencurve);
	}

}

void CAtomforce::OnButtoncapture() 
{
	BeginWaitCursor();

	//捕获m_curveimage这个frame上所画的图像，分两步：
	//1.捕获这个frame上所画的图像为一个CBitmap对象，并得到此对象的BITMAP句柄
	//2.把HBITMAP转换成HDIB（DDB to DIB），从而进行显示和以后的保存。由于强制变成24bit的DIB图像，所以不需要加入调色板

	//以下是第一步，得到frame上所画的图像（即CBitmap对象）的HBITMAP句柄

	CDC memDC;  //内存设备上下文
	CClientDC	dc(this);
	CBitmap bitmap; //捕获的BMP图
	CRect	rect;  

	//得到m_curveimage这个frame的大小
	m_curveimage.GetWindowRect(&rect);
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

	return;
	
}


///////////////////////////////////////////////////////////////////////////////////////////

void CAtomforce::OnButtonstart() 
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
    Para2.FirstChannel = 0;                  //置首通道为0
	Para2.LastChannel = 0;                   //置末通道为0
	Para2.TriggerSource = PCI2003_IN_TRIGGER;//置AD启动模式为内部定时触发
	Para2.Gains = 1;                         //置硬件放大增益为1倍
	Para2.Frequency = 90000;                 //置AD采样频率为90KHz  
    //******************************pci2003*****************************************//
	
	double voltage;
//	int high,low; //高8位和低4位

	//**************************************pci2003**********************************************//
    LONG PerLsbVolt;    //定义一个足够宽度的整型变量存放单位电压值(uV)  
	register ULONG Value;
	PerLsbVolt = (LONG)((10000.0/4096)*1000.0); //取得单位电压值，并以uV为单位将其转换成整型值
    //**************************************pci2003**********************************************//

	LONGLONG begin,end;
//	int tempad;  //AD转换时读转换是否完成的临时变量
	float maxdata; //原始原子力大小数据中的最大值
	float mindata; //原始原子力大小数据中的最小值
	float scale;   //归一化系数

	//首先清屏
	flag=FALSE;
	imagerange=range;
	this->InvalidateRect(FALSE);

    //开始测量

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

	//计算每一步的延时时间	
	delaytime=(time/1000.0)/100.0 - systemdelay;

	//计算移动每一步需要的电压
	movevoltage=(4.9975586*range/1000.0)/99.0;


//使用DA0，即把z接到DA0上，发送DA信号，采集AD
	for(i=0;i<200;i++)
	{
		//写入z方向的电压信号
		voltage=i*movevoltage;

		//**************************************pci2003**********************************************//		
		DAdata=(WORD)(((voltage*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
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
		if ((double)(end-begin)/freq>delaytime)
			break;
		}	
        
		
      //AD采样
       //**************************************pci2003**********************************************//
       //初始化设备对象中的AD部件,为设备操作就绪有关工作,如预置AD采集通道,采样频率等
      		
        //判断AD转换是否完成,D0=0: 正在转换 D0=1: 完成
	

		//在外面的循环里终止扫描
			if (quickstopflag==TRUE)  
			{
				break;
			}				
					
		if(!PCI2003_InitDeviceProAD(hDevice, &Para2)) 
		{
			MessageBox(_T("不明确的初始化错误..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);
		return;
		}	
			
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
        forcedata[i]=(float)(((pADBuffer[0]&0x0FFF)*PerLsbVolt-5000000.0)/1000000.0);
		//**************************************pci2003**********************************************//
		if (forcedata[i]>=5.0)
			forcedata[i]=5.0;
		if (forcedata[i]<=0.0)
			forcedata[i]=0.0;

	}

	//恢复z方向的伸长到最初的z方向位置
	for (i=0;i<200;i++)
	{
		voltage=voltage-movevoltage;

		//**************************************pci2003**********************************************//		
		DAdata=(WORD)(((voltage*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA0控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA0控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata);//输出DA0值
		//**************************************pci2003**********************************************//


	    //根据用户输入的时间，x方向空回时延迟一段时间
		QueryPerformanceCounter(&litmp);
		begin=litmp.QuadPart;
			while(1)
			{
		QueryPerformanceCounter(&litmp);
		end=litmp.QuadPart;
		if ((double)(end-begin)/freq>(delaytime/2.0))
			break;
			}
	}
	//保险起见，再一次置z为零位置
	voltage=0.0;

		//**************************************pci2003**********************************************//		
		DAdata=(WORD)(((voltage*1000.0+5000.0)*4096)/10000.0);//将电压值转换成LSB码制
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// 设置DA0控制字,0x240为输出方式选择寄存器，控制字0x02表示D/A输出
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300为DA0控制寄存器
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata);//输出DA0值
		//**************************************pci2003**********************************************//


	//计算归一化后的原子力大小数值
	//计算最大值和最小值
	maxdata=forcedata[0];
	mindata=forcedata[0];
	for (i=1;i<200;i++)
	{
		if (maxdata<forcedata[i])
			maxdata=forcedata[i];
		if (mindata>forcedata[i])
			mindata=forcedata[i];
	}
	//计算归一化系数，并得到归一化后的原子力大小数据
	if (maxdata!=mindata)
	{
		scale=(float)(360.0/(maxdata-mindata));
		for (i=0;i<200;i++)
		{
		lastforcedata[i]=(int)((forcedata[i]-mindata)*scale);
		if (lastforcedata[i]>=360)
			lastforcedata[i]=360;
		if (lastforcedata[i]<=0)
			lastforcedata[i]=0;
		}
	}
	if (maxdata==mindata)
		for (i=0;i<200;i++)
			lastforcedata[i]=360;

	flag=TRUE;
	imagerange=range;
	this->InvalidateRect(FALSE);
	
}
