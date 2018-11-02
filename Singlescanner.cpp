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

PCI2003_PARA_AD  Para1;  //����Ӳ������

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
//	MessageBox(_T("�ϴ�ɨ�軹δ���"),
//				_T("���Ե�"), MB_ICONEXCLAMATION | MB_OK);		
}

void CSinglescanner::OnCapture() 
{
		BeginWaitCursor();

		//����m_bmpframe���frame��������ͼ�񣬷�������
		//1.�������frame��������ͼ��Ϊһ��CBitmap���󣬲��õ��˶����BITMAP���
		//2.��HBITMAPת����HDIB��DDB to DIB�����Ӷ�������ʾ���Ժ�ı��档����ǿ�Ʊ��24bit��DIBͼ�����Բ���Ҫ�����ɫ��

		//�����ǵ�һ�����õ�frame��������ͼ�񣨼�CBitmap���󣩵�HBITMAP���

		CDC memDC;  //�ڴ��豸������
		CClientDC	dc(this);
		CBitmap bitmap; //�����BMPͼ
		CRect	rect;  

		//�õ�m_curveimage���frame�Ĵ�С
		m_bmpframe.GetWindowRect(&rect);
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
}

//DEL void CSinglescanner::dopaint() 
//DEL {
//DEL 	CDC* pDC=GetDC();
//DEL 	CDC memDC;  //�ڴ��豸������
//DEL 	CBitmap memBit; //�����BMPͼ
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
    Para1.FirstChannel = 0;                  //����ͨ��Ϊ0
	Para1.LastChannel = 0;                   //��ĩͨ��Ϊ0
	Para1.TriggerSource = PCI2003_IN_TRIGGER;//��AD����ģʽΪ�ڲ���ʱ����
	Para1.Gains = 1;                         //��Ӳ���Ŵ�����Ϊ1��
	Para1.Frequency = 90000;                 //��AD����Ƶ��Ϊ90KHz  
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

//��̽�뵽ƫ��λ�ô�
//	int datahigh,datalow;
	//�ƶ�X����ʹ֮����ƫ��λ�ô�������D/A1Ϊx����
	if (singlepianzhivoltagex!=0.0)
		{
	 //**************************************pci2003**********************************************//
     DAdata=(WORD)(((singlepianzhivoltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
	 PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);   // ����DA������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
     PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);   //0x300ΪDA���ƼĴ���
     PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //���DA0ֵ
     //**************************************pci2003**********************************************//
		}
	//�ƶ�Y����ʹ֮����ƫ��λ�ô�������D/A2Ϊy����
	if (singlepianzhivoltagey!=0.0)
		{	
     //**************************************pci2003**********************************************//
     DAdata=(WORD)(((singlepianzhivoltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
	 PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);   // ����DA������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
     PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);   //0x300ΪDA���ƼĴ���
     PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //���DA1ֵ
	 //**************************************pci2003**********************************************//
		}
	m_goback.EnableWindow(FALSE);
	m_capture.EnableWindow(FALSE);
	m_stop.EnableWindow(TRUE);
	m_getdata.EnableWindow(FALSE);

	//��ȡ������߾��ȼ�ʱ����Ƶ��
	if(QueryPerformanceFrequency(&litmp)==0)
	{

		MessageBox(_T("�˼����Ӳ����֧�ָ߾��ȼ�����"),
				_T("Ӳ����֧��"), MB_ICONEXCLAMATION | MB_OK);

		return;
	}

	singlefreq=(double)(litmp.QuadPart);

	//����whileѭ�������ʱ��
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
		MessageBox(_T("�˴���û���㹻�Ķ�ʱ��"),
				_T("���������������"), MB_ICONEXCLAMATION | MB_OK);

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
    LONG PerLsbVolt;    //����һ���㹻��ȵ����ͱ�����ŵ�λ��ѹֵ(uV)  
	register ULONG Value;
	PerLsbVolt = (LONG)((10000.0/4096)*1000.0); //ȡ�õ�λ��ѹֵ������uVΪ��λ����ת��������ֵ
    //**************************************pci2003**********************************************//



///////////////////////////////////////////////
if(direction==0)
{
	//Сͼʵʱɨ��
	if (radiobmpsize==0)
	{
		dc.MoveTo(140,250);
		//����ϴ�ɨ���Ƿ�������ɣ����������ɾͼ�����ֵ��ʹ����ͼ��ťΪ��ʹ����ʵ�������������Ƿ��һ��ɨ��������ɣ�
		if (height==100&&width==200)
		{

			tempmax=tempmin=singlesmallbmpdata[0][0];
			//�������ݲ�����zƽ����������С
			singlezmax=singlezmin=singlesmallbmpdata[0][0];
			singlezavetemp=0;
			for (height=0;height<10;height++)
			{
				for (width=10;width<200;width++)
				{
					//����һ��ͼ���е�z����z��СֵΪ���½绮��Ϊ255�ȷ֣���������
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
			//�Ե�ǰͼ����z���ֵ�İٷֱ���Ϊ��һ��ͼ���z���ֵ���ٷֱȼ�Ϊ�����������ֵ
			singleoldzmax=(float)((singlezmax-singlezmin)*singlelightnessthreshold/100.0+singlezmin);

			//���ò���ͼ��ťΪ��ʹ
			m_capture.EnableWindow(TRUE);
		}
		//����ÿһ���ص����ʱʱ��	
		singledelaytime=singletime/10000.0 - 10.0*singlesystemdelay;

//**************************************
		//Сͼʵʱɨ�裬����DA�źţ��ɼ�AD
		singlevoltagey=singlepianzhivoltagey;
		for(height=0;height<90;height++)
		{
		//***********************************pci2003********************************************//
		DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);     //����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);   //����DA������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);   //0x300ΪDA���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //���DA1ֵ
        //************************************pci2003************************************************//

			singlevoltagex=singlepianzhivoltagex;
	
			if(height%90==0)
				Invalidate();		

			for(width=0;width<200;width++)
			{
				//д��x����ĵ�ѹ�ź�
				singlevoltagex=singlevoltagex+singlemovevoltagex;

				//У��ѹ���մɵķ�����
				singlevoltagex=singlevoltagex-singlevoltagex*singlemovevoltagex*singleceramicscoefficient;

		//***********************************pci2003**************************************************//
        DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //���DA0ֵ
        //************************************pci2003****************************************************//


				//�����û������ʱ�䣬�����ӳ�һ��ʱ��
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
					while(1)
				{
				QueryPerformanceCounter(&litmp);
				end=litmp.QuadPart;
				if ((double)(end-begin)/singlefreq>singledelaytime)
					break;
				}

  		//AD����	
        //******************************pci2003*********************************************//
        //��ʼ���豸�����е�AD����,Ϊ�豸���������йع���,��Ԥ��AD�ɼ�ͨ��,����Ƶ�ʵ�

			//�������ѭ������ֹɨ��
			if (singlequickstopflag==TRUE)
			{
			break;
			}					
					
		if(!PCI2003_InitDeviceProAD(hDevice, &Para1)) 
		{
			MessageBox(_T("����ȷ�ĳ�ʼ������..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);
		return;
		}
        //�ж�ADת���Ƿ����,D0=0: ����ת�� D0=1: ���
        do
		{
			Value = PCI2003_ReadRegisterULong(hDevice,LinearAddr,0x260)&0x00000001; 
		}
		while(Value!=1); 
		
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x00);//��Fifo�ź�,
		pADBuffer[0]=(WORD)PCI2003_ReadRegisterULong(hDevice, LinearAddr,0x270); 
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x01);

		if(!PCI2003_ReleaseDeviceProAD( hDevice )) //�ͷ��豸�ϵ�AD����
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

		//����ʵ�ʲ�����ѹֵ,��ѹ��λΪV
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

					//�ڻ�ͼ������
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

					//�ڻ�ͼ������
					dc.LineTo(i+140,250-singlesmallcurve[i]);
					dc.MoveTo(i+140,250-singlesmallcurve[i]);
				}
			}

			tempmax=(tempmax-tempmin)*200;
			
			dc.TextOut(140,200-35,"Zmax:");
			sprintf(buffer,"%.1f nm",tempmax);
			dc.TextOut(140+45,200-35,buffer);
						
			dc.TextOut(30,400,"��");
			sprintf(buffer,"%d    ",singleyold);
			dc.TextOut(30+15,400,buffer);
			dc.TextOut(30+40,400,"��ɨ��");

			tempave=(singlexaxis-tempmin)*200;
			dc.TextOut(140,300+20,"Zave:");
			sprintf(buffer,"%.1f nm",tempave);
			dc.TextOut(140+45,300+20,buffer);
		}
	}
				//��׽������Ϣ
				if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
				{
				::TranslateMessage(&message);
				::DispatchMessage(&message);
				}

			}
			//�ָ�x������쳤�������x����ƫ��λ��
			for (i=0;i<width;i++)
			{
				//��Ϊѹ���մ�����У���󣬰���ԭ�������𽥽��͵�ѹ���ܻ�С��ԭ���趨��ƫ�õ�ѹֵ��������Ҫ�ж��������ж�
				//����У���������ѹ��Ϊ���Զ�ʹ��ѹ���մɵ��쳤��Ϊ����
				if (singlevoltagex > singlepianzhivoltagex)
				{
					singlevoltagex=singlevoltagex-singlemovevoltagex;
				}
				if (singlevoltagex <= singlepianzhivoltagex)
				{
					singlevoltagex=singlepianzhivoltagex;
				}

		//**************************************pci2003**********************************************//        
        DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA1������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA1���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //���DA1ֵ
		//**************************************pci2003**********************************************//
				//�����û������ʱ�䣬x����ջ�ʱ�ӳ�һ��ʱ��
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
			//�����������һ����x����Ϊ���ƫ��λ��
			singlevoltagex=singlepianzhivoltagex;
		//**************************************pci2003**********************************************//
		DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA1������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA1���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //���DA1ֵ
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


	//��ͼʵʱɨ��
if (radiobmpsize==1)
{

//����ϴ�ɨ���Ƿ�������ɣ����������ɾͼ�����ֵ��ʹ����ͼ��ťΪ��ʹ����ʵ�������������Ƿ��һ��ɨ��������ɣ�
	if (height==100&&width==400)
	{
	tempmax=tempmin=singlelargebmpdata[199][99];
	//�������ݲ�����zƽ����������С
	singlezmax=singlezmin=singlelargebmpdata[199][99];
	singlezavetemp=0;
//		for (height=0;height<100;height++)
//	{
//		singlelargecurve[height]=0;

		for (width=20;width<400;width++)
		{
		//����һ��ͼ���е�z����z��СֵΪ���½绮��Ϊ255�ȷ֣���������
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
		//�Ե�ǰͼ����z���ֵ�İٷֱ���Ϊ��һ��ͼ���z���ֵ���ٷֱȼ�Ϊ�����������ֵ
//		singleoldzmax=(float)((singlezmax-singlezmin)*singlelightnessthreshold/100.0+singlezmin);
		singleoldzmax=singlezmax;

//���ò���ͼ��ťΪ��ʹ
	m_capture.EnableWindow(TRUE);
	}

//����ÿһ���ص����ʱʱ��	
	singledelaytime=singletime/160000.0 - 2.0*singlesystemdelay;

//**************************************

//��ͼʵʱ����
	singlevoltagey=singlepianzhivoltagey;
	for(height=0;height<100;height++)
	{

		//**************************************pci2003**********************************************//		
	    DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA1������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA1���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //���DA1ֵ
		//**************************************pci2003**********************************************//

		singlevoltagex=singlepianzhivoltagex;

		if(height%100==0)
			Invalidate();
	
		for(width=0;width<400;width++)
		{
		//д��x����ĵ�ѹ�ź�
		singlevoltagex=singlevoltagex+singlemovevoltagex;

		//У��ѹ���մɵķ�����
		singlevoltagex=singlevoltagex-singlevoltagex*singlemovevoltagex*singleceramicscoefficient;

		//**************************************pci2003**********************************************//		
		DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
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
		if ((double)(end-begin)/singlefreq>singledelaytime)
			break;
		}

       //AD����
       //**************************************pci2003**********************************************//
       //��ʼ���豸�����е�AD����,Ϊ�豸���������йع���,��Ԥ��AD�ɼ�ͨ��,����Ƶ�ʵ�

		//�������ѭ������ֹɨ��
		if (singlequickstopflag==TRUE)
		{
			break;
		}	
			
		if(!PCI2003_InitDeviceProAD(hDevice, &Para1)) 
		{
			MessageBox(_T("����ȷ�ĳ�ʼ������..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);
		return;
		}
        //�ж�ADת���Ƿ����,D0=0: ����ת�� D0=1: ���
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

					//�ڻ�ͼ������
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

					//�ڻ�ͼ������
					dc.LineTo(i+30,225-singlelargecurve[i]);
					dc.MoveTo(i+30,225-singlelargecurve[i]);
				}
			}
			
			tempmax=(tempmax-tempmin)*200;
			
			dc.TextOut(30,200-45-25,"Zmax:");
			sprintf(buffer,"%.1f nm",tempmax);
			dc.TextOut(30+45,200-45-25,buffer);
						
			dc.TextOut(30,400,"��");
			sprintf(buffer,"%d    ",singleyold);
			dc.TextOut(30+15,400,buffer);
			dc.TextOut(30+40,400,"��ɨ��");

			tempave=(singlexaxis-tempmin)*200;
			dc.TextOut(30,300+30-25,"Zave:");
			sprintf(buffer,"%.1f nm",tempave);
			dc.TextOut(30+45,300+30-25,buffer);
		}
	}
		//��׽������Ϣ
		if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
		::TranslateMessage(&message);
		::DispatchMessage(&message);
		}

		}
		//�ָ�x������쳤�������x����ƫ��λ��
		for (i=0;i<width;i++)
		{

		    //��Ϊѹ���մ�����У���󣬰���ԭ�������𽥽��͵�ѹ���ܻ�С��ԭ���趨��ƫ�õ�ѹֵ��������Ҫ�ж��������ж�
			//����У���������ѹ��Ϊ���Զ�ʹ��ѹ���մɵ��쳤��Ϊ����
			if (singlevoltagex > singlepianzhivoltagex)
			{
				singlevoltagex=singlevoltagex-singlemovevoltagex;
			}
			if (singlevoltagex <= singlepianzhivoltagex)
			{
				singlevoltagex=singlepianzhivoltagex;
			}

		//**************************************pci2003**********************************************//
        DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA0������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA0���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //���DA0ֵ
		//**************************************pci2003**********************************************//	    

		//�����û������ʱ�䣬x����ջ�ʱ�ӳ�һ��ʱ��
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
		//�����������һ����x����Ϊ���ƫ��λ��
		singlevoltagex=singlepianzhivoltagex;
		//**************************************pci2003**********************************************//
		DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA0������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA0���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //���DA0ֵ	   
		//**************************************pci2003**********************************************//


	}
}
	


//**************************************
}
if(direction==1)
{
	//Сͼʵʱɨ��
	if (radiobmpsize==0)
	{
		dc.MoveTo(225,140);
		//����ϴ�ɨ���Ƿ�������ɣ����������ɾͼ�����ֵ��ʹ����ͼ��ťΪ��ʹ����ʵ�������������Ƿ��һ��ɨ��������ɣ�
		if (height==200&&width==90)
		{
			tempmax=tempmin=singlesmallbmpdata[0][0];
			//�������ݲ�����zƽ����������С
			singlezmax=singlezmin=singlesmallbmpdata[0][0];
			singlezavetemp=0;
			for (width=0;width<90;width++)
			{
				for (height=10;height<200;height++)
				{
					//����һ��ͼ���е�z����z��СֵΪ���½绮��Ϊ255�ȷ֣���������
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
			//�Ե�ǰͼ����z���ֵ�İٷֱ���Ϊ��һ��ͼ���z���ֵ���ٷֱȼ�Ϊ�����������ֵ
			singleoldzmax=(float)((singlezmax-singlezmin)*singlelightnessthreshold/100.0+singlezmin);

			//���ò���ͼ��ťΪ��ʹ
			m_capture.EnableWindow(TRUE);
		}
		//����ÿһ���ص����ʱʱ��	
		singledelaytime=singletime/10000.0 - 10.0*singlesystemdelay;

//**************************************
//Сͼʵʱɨ�裬����DA�źţ��ɼ�AD
		singlevoltagex=singlepianzhivoltagex;
		for(width=0;width<90;width++)
		{
		//***********************************pci2003********************************************//
		DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);     //����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);   //����DA������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);   //0x300ΪDA���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //���DA1ֵ
        //************************************pci2003************************************************//

			singlevoltagey=singlepianzhivoltagey;

			if(width%90==0)
				Invalidate();
			for(height=0;height<200;height++)
			{
		
				//д��y����ĵ�ѹ�ź�
				singlevoltagey=singlevoltagey+singlemovevoltagey;

				//У��ѹ���մɵķ�����
				singlevoltagey=singlevoltagey-singlevoltagey*singlemovevoltagey*singleceramicscoefficient;

		//***********************************pci2003**************************************************//
        DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //���DA0ֵ
        //************************************pci2003****************************************************//

				//�����û������ʱ�䣬�����ӳ�һ��ʱ��
				QueryPerformanceCounter(&litmp);
				begin=litmp.QuadPart;
					while(1)
				{
				QueryPerformanceCounter(&litmp);
				end=litmp.QuadPart;
				if ((double)(end-begin)/singlefreq>singledelaytime)
					break;
				}
 
		//AD����	
        //******************************pci2003*********************************************//
        //��ʼ���豸�����е�AD����,Ϊ�豸���������йع���,��Ԥ��AD�ɼ�ͨ��,����Ƶ�ʵ�

			//�������ѭ������ֹɨ��
			if (singlequickstopflag==TRUE)
			{
			break;
			}					
					
		if(!PCI2003_InitDeviceProAD(hDevice, &Para1)) 
		{
			MessageBox(_T("����ȷ�ĳ�ʼ������..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);
		return;
		}
        //�ж�ADת���Ƿ����,D0=0: ����ת�� D0=1: ���
        do
		{
			Value = PCI2003_ReadRegisterULong(hDevice,LinearAddr,0x260)&0x00000001; 
		}
		while(Value!=1); 
		
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x00);//��Fifo�ź�,
		pADBuffer[0]=(WORD)PCI2003_ReadRegisterULong(hDevice, LinearAddr,0x270); 
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x01);

		if(!PCI2003_ReleaseDeviceProAD( hDevice )) //�ͷ��豸�ϵ�AD����
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

		//����ʵ�ʲ�����ѹֵ,��ѹ��λΪV
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

					//�ڻ�ͼ������
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

					//�ڻ�ͼ������
					dc.LineTo(225+singlesmallcurve[i],140+i);
					dc.MoveTo(225+singlesmallcurve[i],140+i);
				}
			}
			
						tempmax=(tempmax-tempmin)*200;
						
						dc.TextOut(225+45,140-20,"Zmax:");
						sprintf(buffer,"%.1f nm",tempmax);
						dc.TextOut(225+45+45,140-20,buffer);
						
						dc.TextOut(30,400,"��");
						sprintf(buffer,"%d    ",singlexold);
						dc.TextOut(30+15,400,buffer);
						dc.TextOut(30+40,400,"��ɨ��");

						tempave=(singleyaxis-tempmin)*200;
						dc.TextOut(225-135,140-20,"Zave:");
						sprintf(buffer,"%.1f nm",tempave);
						dc.TextOut(225-90,140-20,buffer);
		}
	}

				//��׽������Ϣ
				if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
				{
				::TranslateMessage(&message);
				::DispatchMessage(&message);
				}

			}
			//�ָ�y������쳤�������y����ƫ��λ��
			for (i=0;i<height;i++)
			{
				//��Ϊѹ���մ�����У���󣬰���ԭ�������𽥽��͵�ѹ���ܻ�С��ԭ���趨��ƫ�õ�ѹֵ��������Ҫ�ж��������ж�
				//����У���������ѹ��Ϊ���Զ�ʹ��ѹ���մɵ��쳤��Ϊ����
				if (singlevoltagey> singlepianzhivoltagey)
				{
					singlevoltagey=singlevoltagey-singlemovevoltagey;
				}
				if (singlevoltagey <= singlepianzhivoltagey)
				{
					singlevoltagey=singlepianzhivoltagey;
				}

        //*****************************************pci2003******************************************//
        DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA0������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA0���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //���DA0ֵ
        //*****************************************pci2003******************************************//
				
		        //�����û������ʱ�䣬y����ջ�ʱ�ӳ�һ��ʱ��
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
			//�����������һ����y����Ϊ���ƫ��λ��
			singlevoltagey=singlepianzhivoltagey;
		//**************************************pci2003**********************************************//
		DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA0������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA0���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //���DA0ֵ
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

	//��ͼʵʱɨ��
if (radiobmpsize==1)
{
	dc.MoveTo(225,30);

	//����ϴ�ɨ���Ƿ�������ɣ����������ɾͼ�����ֵ��ʹ����ͼ��ťΪ��ʹ����ʵ�������������Ƿ��һ��ɨ��������ɣ�
	if (height==400&&width==100)
	{
		tempmax=tempmin=singlelargebmpdata[0][0];
		//�������ݲ�����zƽ����������С
		singlezmax=singlezmin=singlelargebmpdata[0][0];
		singlezavetemp=0;
		for (height=0;height<400;height++)
		{
			for (width=20;width<100;width++)
			{
				//����һ��ͼ���е�z����z��СֵΪ���½绮��Ϊ255�ȷ֣���������
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
		//�Ե�ǰͼ����z���ֵ�İٷֱ���Ϊ��һ��ͼ���z���ֵ���ٷֱȼ�Ϊ�����������ֵ
		singleoldzmax=(float)((singlezmax-singlezmin)*singlelightnessthreshold/100.0+singlezmin);

//���ò���ͼ��ťΪ��ʹ
		m_capture.EnableWindow(TRUE);
	}

//����ÿһ���ص����ʱʱ��	
	singledelaytime=singletime/160000.0 - 2.0*singlesystemdelay;

//**************************************

//��ͼʵʱ����
	singlevoltagex=singlepianzhivoltagex;
	for(width=0;width<100;width++)
	{
		//**************************************pci2003**********************************************//		
	    DAdata=(WORD)(((singlevoltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA1������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA1���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //���DA1ֵ
		//**************************************pci2003**********************************************//

		singlevoltagey=singlepianzhivoltagey;

		if(width%100==0)
			Invalidate();
	
		for(height=0;height<400;height++)
		{
			//д��y����ĵ�ѹ�ź�
			singlevoltagey=singlevoltagey+singlemovevoltagey;

			//У��ѹ���մɵķ�����
			singlevoltagey=singlevoltagey-singlevoltagey*singlemovevoltagey*singleceramicscoefficient;

		//**************************************pci2003**********************************************//		
		DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
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
			if ((double)(end-begin)/singlefreq>singledelaytime)
				break;
			}

       //AD����
       //**************************************pci2003**********************************************//
       //��ʼ���豸�����е�AD����,Ϊ�豸���������йع���,��Ԥ��AD�ɼ�ͨ��,����Ƶ�ʵ�
 
		//�������ѭ������ֹɨ��
		if (singlequickstopflag==TRUE)
		{
			break;
		}		
				
		if(!PCI2003_InitDeviceProAD(hDevice, &Para1)) 
		{
			MessageBox(_T("����ȷ�ĳ�ʼ������..."),
				_T(""), MB_ICONEXCLAMATION | MB_OK);
		return;
		}
        //�ж�ADת���Ƿ����,D0=0: ����ת�� D0=1: ���
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

					//�ڻ�ͼ������
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

					//�ڻ�ͼ������
					dc.LineTo(225+singlelargecurve[i],i+30);
					dc.MoveTo(225+singlelargecurve[i],i+30);
				}
			}
			
					tempmax=(tempmax-tempmin)*200;
					
					dc.TextOut(225+45,30,"Zmax:");
					sprintf(buffer,"%.1f nm",tempmax);
					dc.TextOut(225+45+45,30,buffer);
					
					dc.TextOut(30,400,"��");
					sprintf(buffer,"%d    ",singlexold);
					dc.TextOut(30+15,400,buffer);
					dc.TextOut(30+40,400,"��ɨ��");

					tempave=(singleyaxis-tempmin)*200;
					dc.TextOut(225-135,30,"Zave:");
					sprintf(buffer,"%.1f nm",tempave);
					dc.TextOut(225-90,30,buffer);
		}
	}
			//��׽������Ϣ
			if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
			}

		}
		//�ָ�y������쳤�������y����ƫ��λ��
		for (i=0;i<height;i++)
		{

		    //��Ϊѹ���մ�����У���󣬰���ԭ�������𽥽��͵�ѹ���ܻ�С��ԭ���趨��ƫ�õ�ѹֵ��������Ҫ�ж��������ж�
			//����У���������ѹ��Ϊ���Զ�ʹ��ѹ���մɵ��쳤��Ϊ����
			if (singlevoltagey > singlepianzhivoltagey)
			{
				singlevoltagey=singlevoltagey-singlemovevoltagey;
			}
			if (singlevoltagey <= singlepianzhivoltagey)
			{
				singlevoltagey=singlepianzhivoltagey;
			}

		//**************************************pci2003**********************************************//
        DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA0������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA0���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //���DA0ֵ
		//**************************************pci2003**********************************************//	    
			
		    //�����û������ʱ�䣬y����ջ�ʱ�ӳ�һ��ʱ��
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
		//�����������һ����y����Ϊ���ƫ��λ��
		singlevoltagey=singlepianzhivoltagey;
		//**************************************pci2003**********************************************//
		DAdata=(WORD)(((singlevoltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);// ����DA0������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);//0x300ΪDA0���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata); //���DA0ֵ	   
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
//ȡ��ƫ��ʱ�ӵĵ�ѹ��ʹѹ���մɻָ���0����
 //if (singlepianzhiflag==TRUE)
//	{
	//�ָ�x�����ѹ���մ�
	 if (singlepianzhivoltagex!=0.0)
		{
        DAdata=0;
		//**************************************pci2003**********************************************//
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);  // ����DA0������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);  //0x300ΪDA0���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x310, DAdata);//���DA0ֵ
		//**************************************pci2003**********************************************//
		}
	 //�ָ�y�����ѹ���մ�
	 if (singlepianzhivoltagey!=0.0)
		{
		DAdata=0;
		//**************************************pci2003**********************************************//
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x240, 0x02);   // ����DA1������,0x240Ϊ�����ʽѡ��Ĵ�����������0x02��ʾD/A���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x300, 0x02);   //0x300ΪDA1���ƼĴ���
        PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x320, DAdata); //���DA1ֵ
		//**************************************pci2003**********************************************//
		}
//	}

	Invalidate();
//����ʱ�䣬������һ��ѭ��
	if (singlesettimerflag==TRUE)
	{
	SetTimer(1,5,NULL);
	}
//ɨ�������־
	singlescanendflag=TRUE;

}

void CSinglescanner::OnStop() 
{
	singlesettimerflag=FALSE;
	singleredrawflag=TRUE;
	singlequickstopflag=TRUE;
//	singlepianzhiflag=TRUE;

//���öԻ����ϰ�ť����ʹ
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

			dc.TextOut(30,400,"��");
			sprintf(buffer,"%d    ",singleyold);
			dc.TextOut(30+15,400,buffer);
			dc.TextOut(30+40,400,"��ɨ��");

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

			dc.TextOut(30,400,"��");
			sprintf(buffer,"%d    ",singleyold);
			dc.TextOut(30+15,400,buffer);
			dc.TextOut(30+40,400,"��ɨ��");
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

			dc.TextOut(30,400,"��");
			sprintf(buffer,"%d    ",singlexold);
			dc.TextOut(30+15,400,buffer);
			dc.TextOut(30+40,400,"��ɨ��");

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

			dc.TextOut(30,400,"��");
			sprintf(buffer,"%d    ",singlexold);
			dc.TextOut(30+15,400,buffer);
			dc.TextOut(30+40,400,"��ɨ��");
		}
	}
}

}
