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
//����OnOk��������ʹ����Ч��Ŀ�����öԻ�����Ӧenter��		
//	CDialog::OnOK();
}

void CAtomforce::OnCancel() 
{
//����OnCancel��������ʹ����Ч��Ŀ�����öԻ�����Ӧesc��	
//	CDialog::OnCancel();
}

void CAtomforce::OnButtongoback() 
{
	CDialog::OnCancel();
}

BOOL CAtomforce::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//�ѶԻ���������Ļ���м�
	CenterWindow(CWnd::GetDesktopWindow());

	//������ʾ������Χ�ĳ�ʼֵ
	imagerange=range=500;
	sprintf(rangechar," %d     ",range);
	m_showrange.SetWindowText(rangechar);
    	
	//���ò�����Χ����ķ�Χ���ƶ������ͳ�ʼֵ
	m_sliderrange.SetRange(100,1000,TRUE);
	m_sliderrange.SetTicFreq(1);
	m_sliderrange.SetPos(range);

	//���ò���ʱ��ĳ�ʼֵ
	//����ʱ�们��ķ�Χ���ƶ������ͳ�ʼֵ
	time=300;
	m_slidertime.SetRange(100,500,TRUE);
	m_slidertime.SetTicFreq(10);
	m_slidertime.SetPos(time);

	//����ͼ�����ɫ
	backgroundcolor=0xFFFFFF; //ͼ�񱳾���ɫ
	coordinatecolor=0xFF0000; //ͼ��������ɫ
	curvecolor=0x0000FF; //ͼ��������ɫ

	//����������y�����꣬�������е����궼�ʹ��������
	pointx=345;
	pointy=60;

	//�û����ߵı�־Ϊ�٣�������Ի����������ã�������������Ȼ�ử��ȥ
	flag=FALSE;

	//��ʼ��ԭ������С����
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

	//����ʱ�������
	CSliderCtrl* slidertime=(CSliderCtrl*)GetDlgItem(IDC_SLIDERTIME);
	time=slidertime->GetPos();

	//������Χ������
	CSliderCtrl* sliderrange=(CSliderCtrl*)GetDlgItem(IDC_SLIDERRANGE);
	range=sliderrange->GetPos();
    //���²�����Χ��������ʾֵ
	sprintf(rangechar," %d     ",range);
	m_showrange.SetWindowText(rangechar);

}

void CAtomforce::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect rect;	
	
	//ͼ�񱳾���ɫ
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
	
	//������ɫ
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

	//������ɫ
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

	//ͼ�񱳾���ɫѡ�����ɫ�ı�
	m_backgroundcolor.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushBackgroundselector(backgroundcolor);
	dc.FillRect(&rect,&BrushBackgroundselector);

	//������ɫѡ�����ɫ�ı�
	m_coordinatecolor.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush Brushcoordinateselector(coordinatecolor);
	dc.FillRect(&rect,&Brushcoordinateselector);

	//������ɫѡ�����ɫ�ı�
	m_curvecolor.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush Brushcurveselector(curvecolor);
	dc.FillRect(&rect,&Brushcurveselector);

	//ͼ�񱳾���ɫ�ı�
	m_curveimage.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CBrush Brushbackground(backgroundcolor);
	dc.FillRect(&rect,&Brushbackground);

	//�������
	//�������
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
    //�����ڿ�
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
	//д�̶ȵ���ֵ
	dc.SetTextColor(coordinatecolor);
	dc.SetBkColor(backgroundcolor);
	dc.TextOut(pointx-15,pointy-5,"1");
	dc.TextOut(pointx-17,pointy-12+4*90,"-1");
	dc.TextOut(pointx-5,pointy+7+4*90,"��ʼ��");
	sprintf(buffer,"%d nm",imagerange);
	dc.TextOut(pointx+358-45,pointy+5+4*90,buffer);
	
	//���߻���
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

	//����m_curveimage���frame��������ͼ�񣬷�������
	//1.�������frame��������ͼ��Ϊһ��CBitmap���󣬲��õ��˶����BITMAP���
	//2.��HBITMAPת����HDIB��DDB to DIB�����Ӷ�������ʾ���Ժ�ı��档����ǿ�Ʊ��24bit��DIBͼ�����Բ���Ҫ�����ɫ��

	//�����ǵ�һ�����õ�frame��������ͼ�񣨼�CBitmap���󣩵�HBITMAP���

	CDC memDC;  //�ڴ��豸������
	CClientDC	dc(this);
	CBitmap bitmap; //�����BMPͼ
	CRect	rect;  

	//�õ�m_curveimage���frame�Ĵ�С
	m_curveimage.GetWindowRect(&rect);
	ScreenToClient(&rect); //����Ļ����ת��Ϊ�������
    //Ϊframe��ʾ�豸�����Ĵ������ݵ��ڴ��豸������
    memDC.CreateCompatibleDC(&dc);
	//����һ����frame��ʾ�豸�����ļ��ݵ�λͼ 
    bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());
    //����λͼѡ���ڴ��豸�������У������ر������ԭλͼ��ָ��
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	//��frame��ʾ�豸�������Ŀ������ڴ��豸��������
    memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc, rect.left, rect.top, SRCCOPY); 
    //�ѱ������λͼ����ѡ���ڴ��豸�����ģ������ָ���ʼ״̬
	memDC.SelectObject(pOldBitmap);
    
	//��CBitmap����õ���HBITMAP���
	HBITMAP hBitmap=(HBITMAP)bitmap.GetSafeHandle();

	//�����ǵڶ�������HBITMAP���ת����HDIB���

    BITMAP bm;               // bitmap structure
    BITMAPINFOHEADER bi;     // bitmap header
    LPBITMAPINFOHEADER lpbi; // pointer to BITMAPINFOHEADER
    DWORD dwLen;             // size of memory block
    HDIB hDib, h;            // handle to DIB, temp handle
    HDC hDC;                 // handle to DC
    WORD biBits;             // bits per pixel
    UINT wLineLen;           //CBITMAP��������ʾ��ͼ��DDB����ʵ�ʿ�ȵ��ֽ��������Ҫ��32bit�ı�����
    DWORD dwSize;             //CBITMAP��������ʾ��ͼ��DDB���Ĵ�С
    DWORD wColSize;          //��ɫ����ռ���ֽ���
   
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


	//��һ�����ĵ�����ʾ�õ���DIBͼ��
	CMicroscopeApp* app =(CMicroscopeApp*)AfxGetApp();

	//�����µ��ĵ�
	POSITION posTemplate = app->GetFirstDocTemplatePosition();
    CDocTemplate* pDocTemplate = app->GetNextDocTemplate(posTemplate);
    CMicroscopeDoc* pDocument = (CMicroscopeDoc*) pDocTemplate->OpenDocumentFile(NULL);

	//����HDIB�����Ҹ�����ͼ
	pDocument->ReplaceHDIB(hDib);
	pDocument->InitDIBData();    // set up new size & palette
	pDocument->ReplaceFlag(FALSE); //��дtxt��Ϣ�ļ�
	pDocument->DrawRectTrackerflag=FALSE;	//����Ƥ�����α�־Ϊ��
	pDocument->SetModifiedFlag(TRUE);
	pDocument->UpdateAllViews(NULL);	

	EndWaitCursor();

	return;
	
}


///////////////////////////////////////////////////////////////////////////////////////////

void CAtomforce::OnButtonstart() 
{
	//******************************pci2003*****************************************// 
    DeviceID = 0; //��̨�����ϵͳ��ֻ��һ̨PCI2003
	hDevice=PCI2003_CreateDevice(DeviceID);      //�����豸����
	if(hDevice==INVALID_HANDLE_VALUE) // �ж��豸�������Ƿ���Ч
    {		
      	MessageBox(_T("�����豸����ʧ��..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);		
        return ;		
     } 
	
    //ȡ��0��ӳ��Ĵ��������Ի���ַ���ж��Ƿ�ɹ�
   	if(!PCI2003_GetDeviceAddr(hDevice, &LinearAddr, &PhysAddr, 0))
	{
        MessageBox(_T("ȡ���豸��ַʧ��..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);				
	    return ;
	}

//��Ӳ��AD��������Ԥ��
    Para2.FirstChannel = 0;                  //����ͨ��Ϊ0
	Para2.LastChannel = 0;                   //��ĩͨ��Ϊ0
	Para2.TriggerSource = PCI2003_IN_TRIGGER;//��AD����ģʽΪ�ڲ���ʱ����
	Para2.Gains = 1;                         //��Ӳ���Ŵ�����Ϊ1��
	Para2.Frequency = 90000;                 //��AD����Ƶ��Ϊ90KHz  
    //******************************pci2003*****************************************//
	
	double voltage;
//	int high,low; //��8λ�͵�4λ

	//**************************************pci2003**********************************************//
    LONG PerLsbVolt;    //����һ���㹻��ȵ����ͱ�����ŵ�λ��ѹֵ(uV)  
	register ULONG Value;
	PerLsbVolt = (LONG)((10000.0/4096)*1000.0); //ȡ�õ�λ��ѹֵ������uVΪ��λ����ת��������ֵ
    //**************************************pci2003**********************************************//

	LONGLONG begin,end;
//	int tempad;  //ADת��ʱ��ת���Ƿ���ɵ���ʱ����
	float maxdata; //ԭʼԭ������С�����е����ֵ
	float mindata; //ԭʼԭ������С�����е���Сֵ
	float scale;   //��һ��ϵ��

	//��������
	flag=FALSE;
	imagerange=range;
	this->InvalidateRect(FALSE);

    //��ʼ����

	//��ȡ������߾��ȼ�ʱ����Ƶ��
	if(QueryPerformanceFrequency(&litmp)==0)
	{

		MessageBox(_T("�˼����Ӳ����֧�ָ߾��ȼ�����"),
				_T("Ӳ����֧��"), MB_ICONEXCLAMATION | MB_OK);

		return;
	}

	freq=(double)(litmp.QuadPart);

	//����whileѭ�������ʱ��
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

	//����ÿһ������ʱʱ��	
	delaytime=(time/1000.0)/100.0 - systemdelay;

	//�����ƶ�ÿһ����Ҫ�ĵ�ѹ
	movevoltage=(4.9975586*range/1000.0)/99.0;


//ʹ��DA0������z�ӵ�DA0�ϣ�����DA�źţ��ɼ�AD
	for(i=0;i<200;i++)
	{
		//д��z����ĵ�ѹ�ź�
		voltage=i*movevoltage;

		//**************************************pci2003**********************************************//		
		DAdata=(WORD)(((voltage*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA0������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA0���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata);//���DA0ֵ
		//**************************************pci2003**********************************************//


        //�����û������ʱ�䣬�����ӳ�һ��ʱ��
		QueryPerformanceCounter(&litmp);
		begin=litmp.QuadPart;
			while(1)
		{
		QueryPerformanceCounter(&litmp);
		end=litmp.QuadPart;
		if ((double)(end-begin)/freq>delaytime)
			break;
		}	
        
		
      //AD����
       //**************************************pci2003**********************************************//
       //��ʼ���豸�����е�AD����,Ϊ�豸���������йع���,��Ԥ��AD�ɼ�ͨ��,����Ƶ�ʵ�
      		
        //�ж�ADת���Ƿ����,D0=0: ����ת�� D0=1: ���
	

		//�������ѭ������ֹɨ��
			if (quickstopflag==TRUE)  
			{
				break;
			}				
					
		if(!PCI2003_InitDeviceProAD(hDevice, &Para2)) 
		{
			MessageBox(_T("����ȷ�ĳ�ʼ������..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);
		return;
		}	
			
		do
		{
			Value = PCI2003_ReadRegisterULong(hDevice,LinearAddr,0x260)&0x00000001; 
		}
		while(Value!=1); 	

		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x00);//��Fifo�ź�
		pADBuffer[0]=(WORD)PCI2003_ReadRegisterULong(hDevice, LinearAddr, 0x270); 
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x01);

        //�ͷ��豸�ϵ�AD����
		if(!PCI2003_ReleaseDeviceProAD( hDevice ))  
		{
			MessageBox(_T("�ͷ�AD����ʧ��"),
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

		//����ʵ�ʲ�����ѹֵ
        forcedata[i]=(float)(((pADBuffer[0]&0x0FFF)*PerLsbVolt-5000000.0)/1000000.0);
		//**************************************pci2003**********************************************//
		if (forcedata[i]>=5.0)
			forcedata[i]=5.0;
		if (forcedata[i]<=0.0)
			forcedata[i]=0.0;

	}

	//�ָ�z������쳤�������z����λ��
	for (i=0;i<200;i++)
	{
		voltage=voltage-movevoltage;

		//**************************************pci2003**********************************************//		
		DAdata=(WORD)(((voltage*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA0������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA0���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata);//���DA0ֵ
		//**************************************pci2003**********************************************//


	    //�����û������ʱ�䣬x����ջ�ʱ�ӳ�һ��ʱ��
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
	//�����������һ����zΪ��λ��
	voltage=0.0;

		//**************************************pci2003**********************************************//		
		DAdata=(WORD)(((voltage*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA0������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA0���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata);//���DA0ֵ
		//**************************************pci2003**********************************************//


	//�����һ�����ԭ������С��ֵ
	//�������ֵ����Сֵ
	maxdata=forcedata[0];
	mindata=forcedata[0];
	for (i=1;i<200;i++)
	{
		if (maxdata<forcedata[i])
			maxdata=forcedata[i];
		if (mindata>forcedata[i])
			mindata=forcedata[i];
	}
	//�����һ��ϵ�������õ���һ�����ԭ������С����
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
