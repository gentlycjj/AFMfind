// Measure.cpp : implementation file

// AFMԴ�������PCI2003����ȫ��4000_008_200��ת��ɫr10g07b03���ȶԱȶȸı�,ɨ��ü�����ʾ,�������,ƽ����ʾ�Ҷ�,Win2000_98��ԭ��������_�ı�������ɫ

// 2008��4��30�գ��޸ĶԱȶȵ��ڣ��ı�Աȶ�:
// if ((128+(bmpoldbits[i][j]-194)*contrast)<0) // 20080430,ԭ���ǣ�128, ��5��

// 2008��1��09�գ�����Topview��ʾ�ĻҶ�����ʾ����CSize�����С���ˣ��˰汾���ܰ�����
// ��ȫ��4000_12_200��ת��ɫɨ��ü�����ʾ�������Win2000_98��ԭ��������_�ı�������ɫ_TopVeiw��ʾ�Ҷ�ֵ

// ���´򿪵��κ�ͼ�񣬲���������400*400�㣬����m*m��(m<400)�����ü�ʱ�����ѡ����ʾ��Χ����ȷ��
// �����Ҫ��������...�ü�����ʾ��Χ���в���ȷ���󣬽���취�ǰѶ��βü���ͼ����̣��ٴ򿪲ü����ɡ�
// ���ߣ����еĲü������ӵ�һ���򿪵�ͼ��ʼ��

// 2006��4��30�գ����������ΪWindows2000/98�����ͣ��Ķ�֮����:
//        ���������ѭ������ֹɨ��if (quickstopflag==TRUE){break;}
//        ǰ�Ƶ�if(!PCI2003_InitDeviceProAD(hDevice, &Para))֮ǰ 
//        ���ҽ�if (singlequickstopflag==TRUE){break;}��䣬
//        ǰ�Ƶ�if(!PCI2003_InitDeviceProAD(hDevice, &Para1)) ֮ǰ����8����

// 2007��5��21�գ�Ϊ��ԭ���������ܹ���Windows2000/98�¼������У��Ķ�֮���ǣ�   


// 2014��5�£����������˶���������ܣ�������ɨ����ʾ�����µ�ɫ�塢������ʾ�����µ�ɫ�塢��������µ�ɫ�塢��ά��߻Ҷ��������µ�ɫ�塢��ѹϵ��500nm/V��
// ɨ�����ظ�Ϊ400*400��200*200��������һ��ͼ�������Сֵʱ��ƽ��������ʹ���������Ҳ��Ӱ����һ��ɨ��ͼ��Աȶȡ����Ӻ���ģʽ�µ�ͼ������б���ܡ�
// ��ʼɨ��ʱ��ɨ��10�м��������Сֵ��Ϊ��һ������ɨ��ͼ��������ޡ�  ���Word�ı���

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

	hDevice = NULL;  //20150212,Ϊ��ֹͣɨ��ʱ��X��Y��ѹ����

	//}}AFX_DATA_INIT
}

CMeasure::~CMeasure()
{
	if(hDevice != NULL) PCI8620_ReleaseDevice(hDevice);   //20150212��Ϊ��ֹͣɨ��ʱX��Y��ѹ����
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
	
	//�ѶԻ���������Ļ���м�
	CenterWindow(CWnd::GetDesktopWindow());
	
	scanendflag=TRUE;  //����ɨ�������־Ϊ��
	redrawflag=FALSE;  //�����ػ���־Ϊ��
	
	//���ø����ؼ��ĳ�ʼֵ
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
    
	//������ʾʱ��ĳ�ʼֵ
	timelast=time=0; // 2005.10.30 //2017USB��30�޸�Ϊ2
	wsprintf(timechar," %d     ",time);
	m_showtime.SetWindowText(timechar);
    
	//����spin�ؼ��ķ�Χ
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
	strNumber.Format("%1.1f",0.6); //�Աȶ�ϵ��Ĭ��ֵ
	SetDlgItemText(IDC_KZ,strNumber);

	//����ʱ�们��ķ�Χ���ƶ������ͳ�ʼֵ
	m_slidertime.SetRange(0,900,TRUE); // 2005.10.30 //
	m_slidertime.SetTicFreq(1);
	m_slidertime.SetPos(time);
	
	//����������ֵ�ĳ�ʼֵ
	lightnessthresholdlast=lightnessthreshold=80;
	sprintf(thresholdchar,"%1.2f     ",(float)(lightnessthreshold/100.0));
	m_showthreshold.SetWindowText(thresholdchar);
	
	//����������ֵ����ķ�Χ���ƶ������ͳ�ʼֵ
	m_sliderthreshold.SetRange(60,100,TRUE);
	m_sliderthreshold.SetTicFreq(1);
	m_sliderthreshold.SetPos(lightnessthreshold);
	
	//���ñ༭����ʹ 0708
	SetStatus(1, FALSE, 10, IDC_SMALLRANGEX, IDC_SMALLRANGEY, IDC_SELECTTIMES, IDC_SINGLEXSCANTXT,
		IDC_SINGLEYSCANTXT, IDC_SINGLEXSCAN, IDC_SINGLEYSCAN, IDC_YDIRECTION, IDC_PIANZHIX, IDC_PIANZHIY);
	SetStatus(1, TRUE, 3, IDC_LARGERANGEY, IDC_LARGERANGEX, IDC_XDIRECTION);
	
	m_direction=0;
	UpdateData(FALSE);
	
	//�����ĸ���ť����ʹ
	m_buttonstartscan.EnableWindow(TRUE);
	m_buttongoback.EnableWindow(TRUE);
	m_buttonbmpcapture.EnableWindow(FALSE);
	m_buttonstopscan.EnableWindow(FALSE);
	
	
	//д����BMPͼ����Ϣͷ�͵�ɫ��
	//дsmallbmp����Ϣͷ
	smallbmpinfoheader.biSize=40;
	smallbmpinfoheader.biWidth=100;
	smallbmpinfoheader.biHeight=100;
	smallbmpinfoheader.biPlanes=1;
	smallbmpinfoheader.biBitCount=8;
	smallbmpinfoheader.biCompression=0;
	smallbmpinfoheader.biSizeImage=10000;  //ͼ��ɼ��õ���дͼ��X��������������      //���ɼ���ͼ����й���Ϣ����BMP��Ϣͷ�У���ΪBMP��Ϣͷ����Щ�����������ɸĶ�������ʹBMPͼʧЧ
	smallbmpinfoheader.biXPelsPerMeter=3780; //ͼ��ɼ��õ���дͼ��Y��������������
	smallbmpinfoheader.biYPelsPerMeter=3780; //ͼ��ɼ��õ���дͼ��ɼ�ʱ����С��ѹֵ
	smallbmpinfoheader.biClrUsed=0;           
	smallbmpinfoheader.biClrImportant=0;   //ͼ��ɼ��õ���дͼ��ɼ�ʱ������ѹֵ
	//дlargebmp����Ϣͷ
	largebmpinfoheader.biSize=40;
	largebmpinfoheader.biWidth=400;
	largebmpinfoheader.biHeight=400;
	largebmpinfoheader.biPlanes=1;
	largebmpinfoheader.biBitCount=8;
	largebmpinfoheader.biCompression=0;
	largebmpinfoheader.biSizeImage=160000; //ͼ��ɼ��õ���дͼ��X��������������
	largebmpinfoheader.biXPelsPerMeter=3780; //ͼ��ɼ��õ���дͼ��Y��������������
	largebmpinfoheader.biYPelsPerMeter=3780;  //ͼ��ɼ��õ���дͼ��ɼ�ʱ����С��ѹֵ
	largebmpinfoheader.biClrUsed=0;            
	largebmpinfoheader.biClrImportant=0;      //ͼ��ɼ��õ���дͼ��ɼ�ʱ������ѹֵ
	
	
	//д��ɫ��
	/*
	for (i=0;i<=255;i++)
	{
	bmpcolorpalette[i].rgbRed=i;
	bmpcolorpalette[i].rgbGreen=i*0.7; // ԭ��ֵR��G��B��i //
	bmpcolorpalette[i].rgbBlue=i*0.3; // 2005��9��23�� //
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
	
	//����Сͼ������
	for (i=0;i<100;i++)
	{
		for (j=0;j<100;j++)
		{
			smallbmpbits[i][j]=192;
			smallbmpdata[i][j]=(float)(3.7647);
		}
	}
    //���ô�ͼ������
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
	
	//ʱ�������
	CSliderCtrl* slidertime=(CSliderCtrl*)GetDlgItem(IDC_SLIDERTIME);
	time=slidertime->GetPos();
    //����ʱ���������ʾֵ
	sprintf(timechar," %d     ",time);
	m_showtime.SetWindowText(timechar);
	
	//������ֵ������
	CSliderCtrl* sliderthreshold=(CSliderCtrl*)GetDlgItem(IDC_SLIDERTHRESHOLD);
	lightnessthreshold=sliderthreshold->GetPos();
    //����������ֵ��������ʾֵ
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
/*	//��ʼ�������������
	if(!PCI8620_InitDeviceProAD(hDevice, &para)) 
	{
		MessageBox(_T("����ȷ�ĳ�ʼ������..."),
			_T(""), MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
	//����AD			
	PCI8620_StartDeviceProAD(hDevice);
	//�ǿղɼ����ݣ�����ֻ����һ�����ݣ�Ϊ�˼�С���������Բɼ�������ݣ�Ȼ��ȡ��ֵ��
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
	
	//����ʵ�ʲ�����ѹֵ	
	volt = (float)(PerLsbVolt * (PCI8620_ADBuffer & 0x1FFF) - 5000000.0) / 1000000.0;
*/
  return true;

}



void CMeasure::OnButtonstartscan() 
{
//	Para.ADMode = PCI8620_ADMODE_SEQUENCE; //��������
//	Para.FirstChannel = 0;                     //����ͨ��Ϊ0
//	Para.LastChannel = 0;                      //��ĩͨ��Ϊ0
	Para.Frequency = 160000;                    //��AD����Ƶ��Ϊ160KHz
//	Para.InputRange = PCI8620_INPUT_N5000_P5000mV; //
//	Para.Gains = PCI8620_GAINS_1MULT;
//	Para.TriggerMode = PCI8620_TRIGMODE_SOFT;
    //******************************pci8620*****************************************/

	//******************************2017USB*****************************************//
	if(usb_open==0)//����豸δ�򿪣�����豸
	{
		if(!OpenUsbV20())
			usb_open=1;
		else
		{
			MessageBox(_T("���豸ʧ��..."),
			_T(""), MB_ICONEXCLAMATION | MB_OK);
			return;
		}
	}
	//******************************2017USB*****************************************//

	if (scanendflag==TRUE)
	{
		
		//���¶Ի����и��ؼ�����ֵ
		if (UpdateData(TRUE)==0)
			return;
		
		//����ɨ��ʱ���������ֵ���������鵱ʱ�����ݣ���Ϊ���Ǽ�¼�����ݲ�����UpdateData����������
		timelast=time;
		lightnessthresholdlast=lightnessthreshold;
		
		//���õ�ǰͼ��ĳ�ʼz����z��Сֵ,�Լ���һ��ͼ��ĳ�ʼz���ֵ��z��Сֵ
		zmax=(float)5.0;
		zmin=(float)0.0;
		oldzmax=(float)2.9;  //// ԭ��ֵΪ2.0 
		oldzmin=(float)1.5;  //// ԭ��ֵΪ1.0
		pianzhivoltagex=0.0;
		pianzhivoltagey=0.0;
		
		//�ж�ɨ�跶Χ��Ӧ��ɨ��ͼ���С
		if (m_radiorange==0)
		{
			if(m_radiobmpsize==1)
			{
				if(m_smallrangex<4||m_smallrangey<4)
				{
					MessageBox(_T("2 �� 20���׵���ɨ�跶ΧС��4����ʱ��ѡ��ɨ��200��200���ص�ͼ��"),
						_T("D/A�ֱ��������޷�ɨ��400��400���ص�ͼ��"), MB_ICONEXCLAMATION | MB_OK);
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
					MessageBox(_T("400 �� 4000���׵���ɨ�跶ΧС��400����ʱ��ѡ��ɨ��200��200���ص�ͼ��"),
						_T("D/A�ֱ��������޷�ɨ��400��400���ص�ͼ��"), MB_ICONEXCLAMATION | MB_OK);
					return;
				}
				
			}
		}
		
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
		
		//�ж���Ʒƫ������ֵ�Ƿ���ȷ������ȷ�ͼ���ƫ�Ƶ�ѹ����Ԫ��ѹ
		if (m_radiorange==0)
		{
			if (((m_smallrangex+m_pianzhix)>20||m_pianzhix<0||(m_smallrangey+m_pianzhiy)>20||m_pianzhiy<0)&&SingleXFlag==FALSE&&SingleYFlag==FALSE)
			{
				MessageBox(_T("������������Ʒƫ��ֵ"),
					_T("��Ʒƫ������ֵ����"), MB_ICONEXCLAMATION | MB_OK);
				
				//���ɨ�跶Χ�д���ѡ��check�ؼ���ʹƫ��edit�ؼ�ʹ��
				m_enablepianzhiflag=TRUE;
				checkflag=FALSE;
				UpdateData(FALSE);
				checkflag=TRUE;
				CEdit* editpianzhix=(CEdit*)GetDlgItem(IDC_PIANZHIX);
				editpianzhix->EnableWindow(TRUE);
				CEdit* editpianzhiy=(CEdit*)GetDlgItem(IDC_PIANZHIY);
				editpianzhiy->EnableWindow(TRUE);
				//����ƫ�þ�̬�ı��ؼ�����ʾ��Χ�Ϳ�������������Сֵ��spin�ؼ���ֵ
				sprintf(timechar,"0 �� %d     ",(20-m_smallrangex));
				m_pianzhirangex.SetWindowText(timechar);
				
				sprintf(timechar,"0 �� %d     ",(20-m_smallrangey));
				m_pianzhirangey.SetWindowText(timechar);
				
				m_spinpianzhix.SetRange(0,20-m_smallrangex);
				m_spinpianzhiy.SetRange(0,20-m_smallrangey);
				
				return;
			}
			//����ƫ�Ƶ�ѹ���ƶ�ÿ����Ԫ��Ҫ�ĵ�ѹ
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
				MessageBox(_T("������������Ʒƫ��ֵ"),
					_T("��Ʒƫ������ֵ����"), MB_ICONEXCLAMATION | MB_OK);
				
				//���ɨ�跶Χ�д���ѡ��check�ؼ���ʹƫ��edit�ؼ�ʹ��
				m_enablepianzhiflag=TRUE;
				checkflag=FALSE;
				UpdateData(FALSE);
				checkflag=TRUE;
				CEdit* editpianzhix=(CEdit*)GetDlgItem(IDC_PIANZHIX);
				editpianzhix->EnableWindow(TRUE);
				CEdit* editpianzhiy=(CEdit*)GetDlgItem(IDC_PIANZHIY);
				editpianzhiy->EnableWindow(TRUE);
				//����ƫ�þ�̬�ı��ؼ�����ʾ��Χ�Ϳ�������������Сֵ��spin�ؼ���ֵ
				sprintf(timechar,"0 �� %d     ",(4000-m_largerangex));
				m_pianzhirangex.SetWindowText(timechar);
				
				sprintf(timechar,"0 �� %d     ",(4000-m_largerangey));
				m_pianzhirangey.SetWindowText(timechar);
				
				m_spinpianzhix.SetRange(0,4000-m_largerangex);
				m_spinpianzhiy.SetRange(0,4000-m_largerangey);
				
				return;
			}
			//����ƫ�Ƶ�ѹ���ƶ�ÿ����Ԫ��Ҫ�ĵ�ѹ
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
		
		//����ѹ���մɷ�����У��ϵ��
		ceramicscoefficient=((CMicroscopeApp*) AfxGetApp())->ceramics;
		
		if(SingleXFlag==FALSE&&SingleYFlag==FALSE)
		{
			//����ʱ���������WM_TIMER��Ϣ�н��в����ͻ�ͼ
			if (!SetTimer(1,5,NULL))
			{
				MessageBox(_T("�˴���û���㹻�Ķ�ʱ��"),
					_T("���������������"), MB_ICONEXCLAMATION | MB_OK);
				
				return ;
			}
		}
		
		//��̽�뵽ƫ��λ�ô�
		//	int datahigh,datalow;
		//�ƶ�X����ʹ֮����ƫ��λ�ô�������D/A1Ϊx����
		if (pianzhivoltagex!=0.0)
		{
			//******************************2017USB*****************************************//
			DAdata=(WORD)(((pianzhivoltagex*1000.0)*4096)/4250.0); //��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
			DASingleOutV20(XDIRECT,DAdata);
			//******************************2017USB*****************************************//
		}
		
		//�ƶ�Y����ʹ֮����ƫ��λ�ô�������D/A2Ϊy����
		if (pianzhivoltagey!=0.0)
		{	
			//******************************2017USB*****************************************//
			DAdata=(WORD)(((pianzhivoltagey*1000.0)*4096)/4250.0);//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
			DASingleOutV20(YDIRECT,DAdata);
			//******************************2017USB*****************************************//
		}
		
		//���ó�ʼ����ֵ
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
		
		//������ʾx��y��Χ��static�ؼ�����ʾ���ݼ��䵥λ
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
		//������ʾz������С���䵥λ��static�ؼ�����ʾ����Ϊ��
		sprintf(timechar," ");
		m_showzmax.SetWindowText(timechar);
		m_showzmaxdanwei.SetWindowText(timechar);
		m_showzmindanwei.SetWindowText(timechar);
		m_showzmin.SetWindowText(timechar);
		
		//���ôֲڶ�Ra,Ry��Rzֵ���䵥λ��static�ؼ�����ʾ����Ϊ��
		sprintf(timechar," ");
		m_showRa.SetWindowText(timechar);
		m_showRy.SetWindowText(timechar);
		m_showRz.SetWindowText(timechar);
		m_showRadanwei.SetWindowText(timechar);
		m_showRydanwei.SetWindowText(timechar);
		m_showRzdanwei.SetWindowText(timechar);
		
		
		//�����ĸ���ť����ʹ
		m_buttongoback.EnableWindow(FALSE);
		m_buttonbmpcapture.EnableWindow(FALSE);
		m_buttonstopscan.EnableWindow(TRUE);
		m_buttonstartscan.EnableWindow(FALSE);
		
		// 07/08 ʹ��SetStatus�������򻯿ؼ�����
		SetStatus(1, FALSE, 9, IDC_LARGERANGEX, IDC_LARGERANGEY, IDC_SMALLRANGEX, 
			IDC_SMALLRANGEY, IDC_PIANZHIX, IDC_PIANZHIY, IDC_SELECTTIMES,
			IDC_SINGLEXSCANTXT, IDC_SINGLEYSCANTXT);
		SetStatus(2, FALSE, 12, IDC_RADIOBMPSIZE, IDC_RADIOBMPSIZE2, IDC_RADIORANGE, 
			IDC_RADIORANGE2, IDC_CHECKPIANZHI, IDC_MOUSESELECT, IDC_SELECTSCANTIME,
			IDC_CHECKSINGLESCAN, IDC_XDIRECTION, IDC_YDIRECTION, IDC_SINGLEXSCAN,IDC_SINGLEYSCAN);
		
		//��ֹ�Ի����ϵ����пؼ�
		m_slidertime.EnableWindow(FALSE);
		m_sliderthreshold.EnableWindow(FALSE);
		
		if(SingleXFlag==TRUE||SingleYFlag==TRUE)
		{
			//��ʼ�������� 
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
	  MessageBox(_T("�ϴ�ɨ�軹δ���"),
	  _T("���Ե�"), MB_ICONEXCLAMATION | MB_OK);
  
}

void CMeasure::OnButtonstopscan() 
{
	
	settimerflag=FALSE;
	redrawflag=TRUE;
	quickstopflag=TRUE;
	pianzhiflag=TRUE;
	
	//���öԻ����ϰ�ť����ʹ
	m_buttongoback.EnableWindow(TRUE);	
	m_buttonstartscan.EnableWindow(TRUE);
	m_buttonstopscan.EnableWindow(FALSE);
	
	SetStatus(2, TRUE, 10, IDC_RADIOBMPSIZE, IDC_RADIOBMPSIZE2,
		IDC_RADIORANGE, IDC_RADIORANGE2, IDC_MOUSESELECT, IDC_SELECTSCANTIME, 
		IDC_CHECKSINGLESCAN, IDC_XDIRECTION, IDC_YDIRECTION, IDC_CHECKPIANZHI);
	
	//�ָ��Ի����Ͽؼ�����ʹ
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
	PCI8620_ReleaseDevice(hDevice);//�ͷ��豸������ռ�õ�ϵͳ��Դ���豸�������� 20150212��Ϊ��ֹͣɨ��ʱX��Y��ѹ����	
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
	///ÿ�ΰ���"ֹͣɨ��"����ʾ���һ��ɨ����
	if (redrawflag==TRUE)
	{
		///Сͼ����ʾ
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
		
		//��ͼ����ʾ	
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
    /*2017USB   LONG PerLsbVolt;    //����һ���㹻��ȵ����ͱ�����ŵ�λ��ѹֵ(mV)  
	PerLsbVolt = (LONG)((10000.0/4096)*1000.0); //ȡ�õ�λ��ѹֵ������mVΪ��λ����ת��������ֵ
    //**************************************pci2003**********************************************/

	//******************************2017USB*****************************************//
	LONG PerLsbVolt;    //����һ���㹻��ȵı�����ŵ�λ��ѹֵ(mW)  
	PerLsbVolt = (LONG)((4350.0/4096)*1000.0); //ȡ�õ�λ��ѹֵ������mVΪ��λ����ת��������ֵ//4350.0Ϊ������ȡ��ֵ
	//******************************2017USB*****************************************//
	
	
	//*************************************** X����Сͼɨ���X�����ͼɨ�� *****************************************//
	if(m_direction==0)  
	{
	
		//*********************************** X����Сͼɨ�� ***************************************************//
		if (m_radiobmpsize==0) // X����Сͼʵʱɨ��
		{
		//***** ��X����Ԥɨ��10�У�����ʾͼ�񣬽����������ֵ����Сֵ����Ϊ��һ������ɨ��ͼ��Ļ�׼ 20140521
		int m_height = 5;

		if(finishtimes == 0)  
		{
			delaytime=time/10000.0 - 10.0*systemdelay;
//			delaytime=time/10000.0;

			voltagey=pianzhivoltagey;
			for(height=0;height < m_height;height++)
			{
				//д��y����ĵ�ѹ�ź�
				voltagey=voltagey+movevoltagey;
				
				voltagey=voltagey-voltagey*movevoltagey*ceramicscoefficient; //У��ѹ���մɵķ�����
				//******************************2017USB******************************************//
//	            DAdata=(WORD)(((voltagey*1000.0)*4096)/4250.0);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
		        DAdata=(WORD)(voltagey*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
			    DASingleOutV20(YDIRECT,DAdata);
			    //******************************2017USB******************************************//
						
				voltagex=pianzhivoltagex;
					
				for(width=0;width<100;width++)
				{
					//д��x����ĵ�ѹ�ź�
					voltagex=voltagex+movevoltagex; 
						
					voltagex=voltagex-voltagex*movevoltagex*ceramicscoefficient; //У��ѹ���մɵķ�����
		            DAdata=(WORD)(voltagex*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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

                    ///////////�������ѭ������ֹɨ�裬ͬʱ��X��Yѹ���մɹ��� 20150209///////////////////////////
					if (quickstopflag==TRUE)
					{

					//******************************2017USB*****************************************//
					voltagex=0;
		            DAdata=(WORD)(voltagex*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
					DASingleOutV20(XDIRECT,DAdata);

					voltagey=0;
		            DAdata=(WORD)(voltagey*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
				 	DASingleOutV20(YDIRECT,DAdata);//
					//******************************2017USB*****************************************//
					break;
					}
                    ///////////�������ѭ������ֹɨ�裬ͬʱ��X��Yѹ���մɹ��� 20150209///////////////////////////


					    //****************************** AD���� ****************************************//
					    //******************************2017USB*****************************************//	
					    ADSingleV20(0,&adResult);//��USB�ɼ�����ȡ0ͨ������
					    Volt=(float)(PerLsbVolt*adResult/1000);//����ʵ�ʵ�ѹֵ,��λV
					    //******************************2017USB*****************************************//


			        	//****************** X�����Y����ͼ������б���� ***************************//
						CString m_cstr;
						GetDlgItemText(IDC_KX, m_cstr);
						m_kx = atof(m_cstr)/100.0; //-0.10��Ԥ��ľ���ֵ
						Volt=(float)(Volt+m_kx*(50-width)/50); // X����ͼ����б�ȵ��� 20140506
						
						GetDlgItemText(IDC_KY, m_cstr);
						m_ky = atof(m_cstr)/100.0; //+0.19��Ԥ��ľ���ֵ
						Volt=(float)(Volt+m_ky*(50-height)/50); // Y����ͼ����б�ȵ��� 20140506
						
						if(Volt<0)Volt=0;
						////////////////////////////////////////////////////////////////////////////	
					
						smallbmpdata[width][height]=Volt; 
						
						//��׽������Ϣ
						if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
						{
							::TranslateMessage(&message);
							::DispatchMessage(&message);
						}
				

					      //////// Ԥɨ��ʱ��Ҳ��ʾɨ���У��������Ϊ����û������ɨ�� /////////
				          if (smallbmpdata[width][height]>2.5)
					      tempbmpbits=2.5;
				          if (smallbmpdata[width][height]<1.0)
					      tempbmpbits=1.0;
			              if (smallbmpdata[width][height]<=2.5&&smallbmpdata[width][height]>=1.0)
					      tempbmpbits=smallbmpdata[width][height];
			              tempindex=(BYTE)(255*(tempbmpbits-1.0)/(2.5-1.0));
				          ////////////////////////////////////////////////////////////////////////////// 20140506�µ�ɫ��
				          if(tempindex>=128) 
						  {	dc.SetPixelV(width+483,height+180,RGB(255,tempindex,2*(tempindex-128)+1)); // 20140518��ԭ��423,150
						  }
				          if(tempindex<128) 
						  {
				        	dc.SetPixelV(width+483,height+180,RGB(2*tempindex,tempindex,0)); 
						  }
				          ///////////////////////////////////////////////////////////////////////////// 20140506�µ�ɫ��
	
					}	
				    //�ָ�x������쳤�������x����ƫ��λ��
		     	    voltagex=pianzhivoltagex;
		            DAdata=(WORD)(voltagex*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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

		        //�ָ�y������쳤�������y����ƫ��λ��
		        voltagey=pianzhivoltagey;
		        DAdata=(WORD)(voltagey*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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
        
			
			//********** Process������ǰ10��ɨ������ֵ����Сֵ ***********//
			float m_ZMax0 = 0.0, m_ZMin0 = 5.0;
			float m_temp;
			
			for(height = 0; height < m_height; height++)
			{
				for(width = 20; width < 100; width++) //����ÿ��ǰ20�����Ӱ��
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
			float m_kz = atof(m_cstr)/2.0; //m_kzΪɨ�������ʾ�ĶԱ�ϵ����һ��
			//*****************2017USB****************************//			
        	oldzmin =(float)(m_ZMax0-m_ZMin0)*m_kz+m_ZMin0; //20140521 ǰ10�е����ֵ����Сֵ���ݣ���Ϊ��һ������ɨ��ͼ�����ʾ���ݡ��洢����  
			oldzmax = m_ZMax0;
			
			//������ʾСͼ��z������Сֵ���䵥λ
			sprintf(timechar," %1.2f",m_ZMax0);
			m_showzmax.SetWindowText(timechar);
			sprintf(timechar,"V");
			m_showzmaxdanwei.SetWindowText(timechar);
			m_showzmindanwei.SetWindowText(timechar);
			sprintf(timechar," %1.2f",m_ZMin0);
			m_showzmin.SetWindowText(timechar);
			//UpdateData(FALSE);
		}
		//************** X����СͼԤɨ��10�н�����������������Сֵ����ΪX����Сͼɨ���׼��ֱ������X�����Сͼʵʱɨ�迪ʼ ********20140521   
		

		
		//************** X��������Сͼʵʱɨ�裬����ǵ�1��ɨ�裬�������˶Σ�ֱ��ȥСͼʵʱɨ�裻����ǵ�2�Σ������˶Σ������1��ͼ������ֵ����Сֵ�ȣ��Դ����� *********** 
		if (height==100&&width==100)
		{
			//********* �Ըո��������ɨ���ͼ�񣨶���Ϊ��1��ͼ�񣩵����ݽ�������������z��ѹƽ��ֵ�����ֵ����Сֵ *********//
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
					
					//****** ���ڵ�1������ɨ���ͼ������ǰ10�е�oldzmax��oldzminֵ������1��ͼ��ĵ�ѹֵ��һ����255�ȷֵĻҶ�ֵ���Ա㲶��ͼ���ֹͣɨ��ʱ�����1��ͼ������ ******/
					//****** ���ڵ�2������ɨ���ͼ�����ݵ�1��ͼ���oldzmax��oldzminֵ������2��ͼ��ĵ�ѹֵ��һ����255�ȷֵĻҶ�ֵ���Ա㲶��ͼ���ֹͣɨ��ʱ�����2��ͼ������ ******/
					//****** �Դ����� ******/
					
					//****** ���ڵ��Ե����½�������ԭ�㣬�����Ҫ��AFMͼ���������任
					smallbmpbits[height][width]=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin)); 
					
					//****** �����1��ͼ��ĵ�ѹƽ��ֵ���Ա��ڵ�1��ͼ��ɨ��һ����ʱ����ʾ����
					zavetemp=zavetemp+smallbmpdata[width][height]; 
				}
			}
			zave=(float)(zavetemp/10000.0); //��1��ͼ��ĵ�ѹƽ��ֵ���ڵ�1��ͼ��ɨ��һ��������ʾ����
			
			
			//***** �����1��ͼ������ֵ����Сֵǰ����ƽ���������ı�ͼ��������Ϊ�����1��ͼ�����ݵ����ݣ���������2��ͼ����Ϊ����ʾ���ݣ�20140521 */
			float m_ZMax1 = 0.0, m_ZMin1 = 5.0;
			float m_temp;
			
			for(height = 1; height < 99; height++)
			{
				for(width = 20; width <99; width++) //����ÿ��ǰ20�����Ӱ��
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
			
			//********************* ��Щ���ݣ�ֻ�Ǵ���ͼ��洢֮��20140521 ********************//
			zmin = m_ZMin1;   
			zmax = m_ZMax1;	
			//*********************************************************************************//
			
			
			//������ʾ��1������ɨ���Сͼ��z���ֵ����Сֵ���䵥λ
			sprintf(timechar," %1.2f",m_ZMax1); //��1��ͼ��ĵ�ѹ���ֵ���ڵ�1��ͼ��ɨ��һ�����󡢵�2��ͼ��ɨ�������һֱ��ʾ
			m_showzmax.SetWindowText(timechar);
			sprintf(timechar,"V");
			m_showzmaxdanwei.SetWindowText(timechar);
			m_showzmindanwei.SetWindowText(timechar);
			sprintf(timechar," %1.2f",m_ZMin1); //��1��ͼ��ĵ�ѹ��Сֵ���ڵ�1��ͼ��ɨ��һ�����󡢵�2��ͼ��ɨ�������һֱ��ʾ
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
			
			//������ʾСͼ��Ra,Ry,Rzֵ���䵥λ
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
			
			//���ò���ͼ��ťΪ��ʹ
			m_buttonbmpcapture.EnableWindow(TRUE);
			
			
			//********* ��󣬽�oldzmax��oldzmin������2��ͼ����Ϊ��2��ͼ����ʾ�����ݣ��Դ����� *********//
			//*****************2017USB****************************//
			CString m_cstr;
			GetDlgItemText(IDC_KZ, m_cstr);
			float m_kz = atof(m_cstr)/2.0; 
			//*****************2017USB****************************//	
			oldzmin=(float)(m_ZMax1-m_ZMin1)*m_kz+m_ZMin1; //20140521  
			oldzmax=m_ZMax1;		
		}
		//*********************************************** X����Сͼ���ݼ��㴦����� **************************************//
		


		//********************************************** X�����Сͼʵʱɨ����ʽ��ʼ *************************************//
		voltagey=pianzhivoltagey;

		for(height=0;height<100;height++)
		{
			//д��y����ĵ�ѹ�ź�
			voltagey=voltagey+movevoltagey;
						
			voltagey=voltagey-voltagey*movevoltagey*ceramicscoefficient; //У��ѹ���մɵķ�����
//	       	DAdata=(WORD)(((voltagey*1000.0)*4096)/4250.0);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
		    DAdata=(WORD)(voltagey*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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
				//д��x����ĵ�ѹ�ź�
				voltagex=voltagex+movevoltagex;
				
				voltagex=voltagex-voltagex*movevoltagex*ceramicscoefficient; //У��ѹ���մɵķ�����
		        DAdata=(WORD)(voltagex*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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


                ////////////////////////�������ѭ������ֹɨ�裬ͬʱ��X��Yѹ���մɹ��� 20150209////////////////
				if (quickstopflag==TRUE)
				{
					//******************************2017USB**************************************//
					voltagex=0;
		            DAdata=(WORD)(voltagex*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
					DASingleOutV20(XDIRECT,DAdata);

					voltagey=0;
		            DAdata=(WORD)(voltagey*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
					DASingleOutV20(YDIRECT,DAdata);//
					//******************************2017USB*****************************************//
					break;
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				//***************************** AD���� *****************************************//
				ADSingleV20(0,&adResult);//��USB�ɼ�����ȡ0ͨ������
				Volt=(float)(PerLsbVolt*adResult/1000);//����ʵ�ʵ�ѹֵ,��λV
				//******************************2017USB*****************************************//


				//****************** X�����Y����ͼ������б���� ***************************//
				CString m_cstr;
				GetDlgItemText(IDC_KX, m_cstr);
				m_kx = atof(m_cstr)/100.0; 
				Volt=(float)(Volt+m_kx*(50-width)/50); // X����ͼ����б�ȵ��� 20140506
				
				GetDlgItemText(IDC_KY, m_cstr);
				m_ky = atof(m_cstr)/100.0;
				Volt=(float)(Volt+m_ky*(50-height)/50); // Y����ͼ����б�ȵ��� 20140506
				////////////////////////////////////////////////////////////////////////////		
				

				smallbmpdata[width][height]=Volt; //������б��ĵ�ѹֵ�������Сͼ��ͼ������
				if (smallbmpdata[width][height]<=0.0)
				{
					smallbmpdata[width][height]=0.0;
				}
				
				//����һ��ͼ���д��ݹ�����z����z��Сֵ���Ե�һ��ͼ��ɨ����ԣ�����ǰ10��ɨ�裩Ϊ���½绮��Ϊ255�ȷֽ�������������ɨ�������ʵʱ��ʾ
				if (smallbmpdata[width][height]>oldzmax)
					tempbmpbits=oldzmax;
				if (smallbmpdata[width][height]<oldzmin)
					tempbmpbits=oldzmin;
				if (smallbmpdata[width][height]<=oldzmax&&smallbmpdata[width][height]>=oldzmin)
					tempbmpbits=smallbmpdata[width][height];
				
				tempindex=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin));
				

				////////////////////////////////////////////////////////////////////////////// 20140506�µ�ɫ��
				if(tempindex>=128) 
				{	dc.SetPixelV(width+483,height+180,RGB(255,tempindex,2*(tempindex-128)+1)); // 20140518��ԭ��423,150
				}
				
				if(tempindex<128) 
				{
					dc.SetPixelV(width+483,height+180,RGB(2*tempindex,tempindex,0)); 
				}
				////////////////////////////////////////////////////////////////////////////// 20140506�µ�ɫ��
				
				//��׽������Ϣ
				if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
				{
					::TranslateMessage(&message);
					::DispatchMessage(&message);
				}
				
			}
					
			//�ָ�x������쳤�������x����ƫ��λ��
		    voltagex=pianzhivoltagex;
			if(voltagex<=0)voltagex=0;
			DAdata=(WORD)(voltagex*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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
			
		   //�ָ�y������쳤�������y����ƫ��λ��
		   voltagey=pianzhivoltagey;
           if(voltagey<=0)voltagey=0;
		   DAdata=(WORD)(voltagey*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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
	//************************ X����Сͼɨ��������������ɨ�裬���ص���ʼif (height==200&&width==200)��������ɨ�� ************************//
		

	
	//****************************************** X�����ͼɨ�裬X�����ͼɨ�裬X�����ͼɨ�� *********************************************//
	if (m_radiobmpsize==1) // X�����ͼʵʱɨ��
	{
        //***** ��X����Ԥɨ��10�У�����ʾͼ�񣬽����������ֵ����Сֵ����Ϊ��һ������ɨ��ͼ��Ļ�׼ 20140521
		int m_height = 5;

		if(finishtimes == 0) // 
		{
		    delaytime=time/40000.0 - 10.0*systemdelay;
//          delaytime=time/40000.0;
			voltagey=pianzhivoltagey;

			for(height=0;height < m_height;height++)
			{
				//д��y����ĵ�ѹ�ź�
				voltagey=voltagey+movevoltagey;
				voltagey=voltagey-voltagey*movevoltagey*ceramicscoefficient; //У��ѹ���մɵķ�����
//	         	DAdata=(WORD)(((voltagey*1000.0)*4096)/4250.0);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
		        DAdata=(WORD)(voltagey*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
				DASingleOutV20(YDIRECT,DAdata);
				//******************************2017USB******************************************//
							
				voltagex=pianzhivoltagex;
								
				for(width=0;width<400;width++)
				{
					//д��x����ĵ�ѹ�ź�
					voltagex=voltagex+movevoltagex; //20140520��ɨ���0,10,20,30.....�У���Ϊ��һ��ͼ��Ļ�׼
					voltagex=voltagex-voltagex*movevoltagex*ceramicscoefficient; //У��ѹ���մɵķ�����
		            DAdata=(WORD)(voltagex*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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


					////////////////////////�������ѭ������ֹɨ�裬ͬʱ��X��Yѹ���մɹ��� 20150209///////////
					if (quickstopflag==TRUE)
					{
					//******************************2017USB*****************************************//
					voltagex=0;
		            DAdata=(WORD)(voltagex*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
					DASingleOutV20(XDIRECT,DAdata);

					voltagey=0;
		            DAdata=(WORD)(voltagey*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
					DASingleOutV20(YDIRECT,DAdata);//
					//******************************2017USB*****************************************//
                    break;
					}
                    /////////////////////////////////////////////////////////////////////////////////////////
                    
					//AD����
					//******************************2017USB*****************************************//
					ADSingleV20(0,&adResult);//��USB�ɼ�����ȡ0ͨ������
					Volt=(float)(PerLsbVolt*adResult/1000);//����ʵ�ʵ�ѹֵ,��λV
					//******************************2017USB*****************************************//
					

				    //*********************** X�����Y����ͼ������б���� ***************************//
					CString m_cstr;
					GetDlgItemText(IDC_KX, m_cstr);
					m_kx = atof(m_cstr)/100.0; 
					Volt=(float)(Volt+m_kx*(200-width)/200); // X����ͼ����б�ȵ��� 20140506
					
					GetDlgItemText(IDC_KY, m_cstr);
					m_ky = atof(m_cstr)/100.0;
					Volt=(float)(Volt+m_ky*(200-height)/200); // Y����ͼ����б�ȵ��� 20140506
					
					if(Volt<0)Volt=0;
					//////////////////////////////////////////////////////////////////////////////////	

					largebmpdata[width][height]=Volt; 

					      //////// Ԥɨ��ʱ��Ҳ��ʾɨ���У��������Ϊ����û������ɨ�� /////////
				          if (largebmpdata[width][height]>2.5)
					      tempbmpbits=2.5;
				          if (largebmpdata[width][height]<1.0)
					      tempbmpbits=1.0;
			              if (largebmpdata[width][height]<=2.5&&largebmpdata[width][height]>=1.0)
					      tempbmpbits=largebmpdata[width][height];
			              tempindex=(BYTE)(255*(tempbmpbits-1.0)/(2.5-1.0));
				          ////////////////////////////////////////////////////////////////////////////// 20140506�µ�ɫ��
				          if(tempindex>=128) 
						  {	dc.SetPixelV(width+332,height+20,RGB(255,tempindex,2*(tempindex-128)+1)); // 20140518��ԭ��423,150
						  }
				          if(tempindex<128) 
						  {
				        	dc.SetPixelV(width+332,height+20,RGB(2*tempindex,tempindex,0)); 
						  }
				          ///////////////////////////////////////////////////////////////////////////// 20140506�µ�ɫ��

	
					//��׽������Ϣ
					if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
					{
						::TranslateMessage(&message);
						::DispatchMessage(&message);
					}
					
				}

			    //�ָ�x������쳤�������x����ƫ��λ��
			    voltagex=pianzhivoltagex;
		        DAdata=(WORD)(voltagex*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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

    		//�ָ�y������쳤�������y����ƫ��λ��
			voltagey=pianzhivoltagey;
		    DAdata=(WORD)(voltagey*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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
				
							
			//********** Process������ǰ10��ɨ������ֵ����Сֵ ***********//
			float m_ZMax0 = 0.0, m_ZMin0 = 5.0;
			float m_temp;
			
			for(height = 0; height < m_height; height++)
			{	for(width = 40; width < 400; width++) //����ÿ��ǰ40�����Ӱ��
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
            oldzmin =(float)(m_ZMax0-m_ZMin0)*m_kz+m_ZMin0; //20140521 ǰ10�е����ֵ����Сֵ���ݣ���Ϊ��һ������ɨ��ͼ�����ʾ���ݡ��洢����  
			oldzmax = m_ZMax0;
			
			//������ʾСͼ��z������Сֵ���䵥λ
			sprintf(timechar," %1.2f",m_ZMax0);
			m_showzmax.SetWindowText(timechar);
			sprintf(timechar,"V");
			m_showzmaxdanwei.SetWindowText(timechar);
			m_showzmindanwei.SetWindowText(timechar);
			sprintf(timechar," %1.2f",m_ZMin0);
			m_showzmin.SetWindowText(timechar);
		}
		//************** X�����ͼԤɨ��10�н�����������������Сֵ����ΪX�����ͼɨ���׼��ֱ������X����Ĵ�ͼʵʱɨ�迪ʼ ********20140521   
		
		

		//************** X����������ͼ���ݼ��㣬����ǵ�1��ɨ�裬�������˶Σ�ֱ��ȥ��ͼʵʱɨ�裻����ǵ�2�Σ������˶Σ������1��ͼ������ֵ����Сֵ�ȣ��Դ����� *********** 
		if (height==400&&width==400)
		{
			//********* �Ըո��������ɨ���ͼ�񣨶���Ϊ��1��ͼ�񣩵����ݽ�������������z��ѹƽ��ֵ�����ֵ����Сֵ *********//
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
					
					//****** ���ڵ�1������ɨ���ͼ������ǰ10�е�oldzmax��oldzminֵ������1��ͼ��ĵ�ѹֵ��һ����255�ȷֵĻҶ�ֵ���Ա㲶��ͼ���ֹͣɨ��ʱ�����1��ͼ������ ******/
					//****** ���ڵ�2������ɨ���ͼ�����ݵ�1��ͼ���oldzmax��oldzminֵ������2��ͼ��ĵ�ѹֵ��һ����255�ȷֵĻҶ�ֵ���Ա㲶��ͼ���ֹͣɨ��ʱ�����2��ͼ������ ******/
					//****** �Դ����� ******/
					
					//****** ���ڵ�����Ļ���½�������ԭ�㣬�����Ҫ��AFMͼ���������任
					largebmpbits[height][width]=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin)); 
					
					//****** �����1��ͼ��ĵ�ѹƽ��ֵ���Ա��ڵ�1��ͼ��ɨ��һ����ʱ����ʾ����
					zavetemp=zavetemp+largebmpdata[width][height]; 
				}
			}
			zave=(float)(zavetemp/160000.0); //��1��ͼ��ĵ�ѹƽ��ֵ���ڵ�1��ͼ��ɨ��һ��������ʾ����
			
			
			//***** �����1��ͼ������ֵ����Сֵǰ����ƽ���������ı�ͼ��������Ϊ�����1��ͼ�����ݵ����ݣ���������2��ͼ����Ϊ����ʾ���ݣ�20140521 */
			float m_ZMax1 = 0.0, m_ZMin1 = 5.0;
			float m_temp;
			
			for(height = 1; height < 399; height++)
			{
				for(width = 40; width <399; width++) //����ÿ��ǰ40�����Ӱ��
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
			
			//********************* ��Щ���ݣ�ֻ�Ǵ���ͼ��洢֮��20140521 ********************//
			zmin = m_ZMin1;   
			zmax = m_ZMax1;	
			//*********************************************************************************//
			
			
			//������ʾ��1������ɨ��Ĵ�ͼ��z���ֵ����Сֵ���䵥λ
			sprintf(timechar," %1.2f",m_ZMax1); //��1��ͼ��ĵ�ѹ���ֵ���ڵ�1��ͼ��ɨ��һ�����󡢵�2��ͼ��ɨ�������һֱ��ʾ
			m_showzmax.SetWindowText(timechar);
			sprintf(timechar,"V");
			m_showzmaxdanwei.SetWindowText(timechar);
			m_showzmindanwei.SetWindowText(timechar);
			sprintf(timechar," %1.2f",m_ZMin1); //��1��ͼ��ĵ�ѹ��Сֵ���ڵ�1��ͼ��ɨ��һ�����󡢵�2��ͼ��ɨ�������һֱ��ʾ
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
			
			//������ʾСͼ��Ra,Ry,Rzֵ���䵥λ
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
			
			//���ò���ͼ��ťΪ��ʹ
			m_buttonbmpcapture.EnableWindow(TRUE);
			
			
			//********* ��󣬽�oldzmax��oldzmin������2��ͼ����Ϊ��2��ͼ����ʾ�����ݣ��Դ����� *********//
			//*****************2017USB****************************//
			CString m_cstr;
			GetDlgItemText(IDC_KZ, m_cstr);
			float m_kz = atof(m_cstr)/2.0; 
			//*****************2017USB****************************//	
			oldzmin=(float)(m_ZMax1-m_ZMin1)*m_kz+m_ZMin1; //20140521  
			oldzmax=m_ZMax1;		
		}
		//*********************************************** X�����ͼ���ݼ��㴦����� **************************************//
		
		
		//********************************************** X����Ĵ�ͼʵʱɨ����ʽ��ʼ *************************************//
		voltagey=pianzhivoltagey;
		for(height=0;height<400;height++)
		{
			//д��y����ĵ�ѹ�ź�
			voltagey=voltagey+movevoltagey;
		
			voltagey=voltagey-voltagey*movevoltagey*ceramicscoefficient; //У��ѹ���մɵķ�����
			//******************************2017USB******************************************//
//	        DAdata=(WORD)(((voltagey*1000.0)*4096)/4250.0);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
		    DAdata=(WORD)(voltagey*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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
				//д��x����ĵ�ѹ�ź�
				voltagex=voltagex+movevoltagex;
				voltagex=voltagex-voltagex*movevoltagex*ceramicscoefficient; //У��ѹ���մɵķ�����
		        DAdata=(WORD)(voltagex*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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

			
				////////////////////////�������ѭ������ֹɨ�裬ͬʱ��X��Yѹ���մɹ��� 20150209///////////////
				if (quickstopflag==TRUE)  
				{
					//******************************2017USB*****************************************//
					voltagex=0;
	            	DAdata=(WORD)(voltagex*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
					DASingleOutV20(XDIRECT,DAdata);

					voltagey=0;
	             	DAdata=(WORD)(voltagey*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
					DASingleOutV20(YDIRECT,DAdata);//
					//******************************2017USB*****************************************//
                    break;
				}		
				/////////////////////////////////////////////////////////////////////////////////////////////
				

				//AD����
				//******************************2017USB************************************//
				ADSingleV20(0,&adResult);//��USB�ɼ�����ȡ0ͨ������
				Volt=(float)(PerLsbVolt*adResult/1000);//����ʵ�ʵ�ѹֵ,��λV
				//******************************2017USB************************************//
				

				//****************** X�����Y����ͼ������б���� ***************************//
				CString m_cstr;
				GetDlgItemText(IDC_KX, m_cstr);
				m_kx = atof(m_cstr)/100.0; 
				Volt=(float)(Volt+m_kx*(200-width)/200); // X����ͼ����б�ȵ��� 20140506
				
				GetDlgItemText(IDC_KY, m_cstr);
				m_ky = atof(m_cstr)/100.0;
				Volt=(float)(Volt+m_ky*(200-height)/200); // Y����ͼ����б�ȵ��� 20140506
				////////////////////////////////////////////////////////////////////////////		
				

				largebmpdata[width][height]=Volt; //������б��ĵ�ѹֵ�������Сͼ��ͼ������
				if (largebmpdata[width][height]<=0.0)
				{
					largebmpdata[width][height]=0.0;
				}
				
				//����һ��ͼ���д��ݹ�����z����z��Сֵ���Ե�һ��ͼ��ɨ����ԣ�����ǰ10��ɨ�裩Ϊ���½绮��Ϊ255�ȷֽ�������������ɨ�������ʵʱ��ʾ
				if (largebmpdata[width][height]>oldzmax)
					tempbmpbits=oldzmax;
				if (largebmpdata[width][height]<oldzmin)
					tempbmpbits=oldzmin;
				if (largebmpdata[width][height]<=oldzmax&&largebmpdata[width][height]>=oldzmin)
					tempbmpbits=largebmpdata[width][height];
				
				tempindex=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin));
				
				////////////////////////////////////////////////////////////////////////////// 20140506�µ�ɫ��
				if(tempindex>=128) 
				{	dc.SetPixelV(width+332,height+20,RGB(255,tempindex,2*(tempindex-128)+1)); // 20140518��ԭ��423,150
				}
				
				if(tempindex<128) 
				{
					dc.SetPixelV(width+332,height+20,RGB(2*tempindex,tempindex,0)); 
				}
				////////////////////////////////////////////////////////////////////////////// 20140506�µ�ɫ��
				
				//��׽������Ϣ
				if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
				{
					::TranslateMessage(&message);
					::DispatchMessage(&message);
				}
				
			}
			//�ָ�x������쳤�������x����ƫ��λ��
			voltagex=pianzhivoltagex;
		    DAdata=(WORD)(voltagex*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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

		   //�ָ�y������쳤�������y����ƫ��λ��
		   voltagey=pianzhivoltagey;
		   DAdata=(WORD)(voltagey*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
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
//************************ X�����ͼɨ��������������ɨ�裬���ص���ʼif (height==400&&width==400)��������ɨ�� ************************//

}
//************************ X����ɨ��ȫ������ **********************************************************************//



//*************************************** Y����Сͼɨ���Y�����ͼɨ�裬���ڹرմ˹��� 20140521 ************************************//
if(m_direction==1) // Y�����ɨ��  Y�����ɨ��
{
	// Y����ɨ���Сͼʵʱɨ��
	if (m_radiobmpsize==0)
	{
		//����ϴ�ɨ���Ƿ�������ɣ����������ɾͼ�����ֵ��ʹ����ͼ��ťΪ��ʹ����ʵ�������������Ƿ��һ��ɨ��������ɣ�
		if (height==100&&width==100)
		{
			//�������ݲ�����zƽ����������С
			zmax=zmin=smallbmpdata[0][0];
			zavetemp=0;
			for (width=0;width<100;width++)
			{
				for (height=0;height<100;height++)
				{
					//����һ��ͼ���е�z����z��СֵΪ���½绮��Ϊ255�ȷ֣���������
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
			//�Ե�ǰͼ����z���ֵ�İٷֱ���Ϊ��һ��ͼ���z���ֵ���ٷֱȼ�Ϊ�����������ֵ
			//			oldzmax=(float)((zmax-zmin)*lightnessthreshold/100.0+zmin);
			oldzmax=zmax;
			
			//������ʾСͼ��z������Сֵ���䵥λ
			sprintf(timechar," %1.2f",zmax);
			m_showzmax.SetWindowText(timechar);
			sprintf(timechar,"V");
			m_showzmaxdanwei.SetWindowText(timechar);
			m_showzmindanwei.SetWindowText(timechar);
			sprintf(timechar," %1.2f",zmin);
			m_showzmin.SetWindowText(timechar);
			
			//����ֲڶ�Ra��Ry��Rz
			//Ra����
			//��smallbmpbits�������õ������ĵ�ѹֵ�����������е�ƽ��ֵ�����ֵ����Сֵ
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
			
			//������ʾСͼ��Ra,Ry,Rzֵ���䵥λ
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
			
			
			
			//���ò���ͼ��ťΪ��ʹ
			m_buttonbmpcapture.EnableWindow(TRUE);
		}
		//����ÿһ���ص����ʱʱ��	
        delaytime=time/10000.0 - 10.0*systemdelay;	
//		delaytime=time/10000.0;
		
		//*************************************************************************************
		// Y�����Сͼʵʱɨ�迪ʼ������DA�źţ��ɼ�AD
		voltagex=pianzhivoltagex;
		for(width=0;width<100;width++)
		{
			//д��x����ĵ�ѹ�ź�
			voltagex=voltagex+movevoltagex;
			
			//У��ѹ���մɵķ�����
			voltagex=voltagex-voltagex*movevoltagex*ceramicscoefficient;
			//			voltagex=voltagex-voltagex*movevoltagex*0.10;
			
			//***********************************pci2003********************************************//
			DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);     //����ѹֵת����LSB����
			//2014/7 pci2003--->pci8620
			PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
			//************************************pci2003************************************************//
			
			voltagey=pianzhivoltagey;
			
			QueryPerformanceCounter(&litmp);
			rowscanbegin=litmp.QuadPart;
			
			for(height=0;height<200;height++)
			{
				
				//д��y����ĵ�ѹ�ź�
				voltagey=voltagey+movevoltagey;
				
				//У��ѹ���մɵķ�����
				voltagey=voltagey-voltagey*movevoltagey*ceramicscoefficient;
				//				voltagey=voltagey-voltagey*movevoltagey*0.10;
				
				//***********************************pci2003**************************************************//
				DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
				PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
				//************************************pci2003****************************************************//
				
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
				//******************************pci2003*********************************************//
				//��ʼ���豸�����е�AD����,Ϊ�豸���������йع���,��Ԥ��AD�ɼ�ͨ��,����Ƶ�ʵ�
				
				//�������ѭ������ֹɨ��
				if (quickstopflag==TRUE)
				{
					//**************************************pci2003**********************************************//	
					//2014/7 pci2003--->pci8620
                    voltagex=0;
					DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
					PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
					//**************************************pci2003**********************************************//

					//**************************************pci2003**********************************************//	
					//2014/7 pci2003--->pci8620
                    voltagey=0;
					DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
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
				
				//����һ��ͼ���е�z����z��СֵΪ���½绮��Ϊ255�ȷ֣���������
				if (smallbmpdata[width][height]>oldzmax)
					tempbmpbits=oldzmax;
				if (smallbmpdata[width][height]<oldzmin)
					tempbmpbits=oldzmin;
				if (smallbmpdata[width][height]<=oldzmax&&smallbmpdata[width][height]>=oldzmin)
					tempbmpbits=smallbmpdata[width][height];
				
				
				tempindex=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin));
				//�ڻ�ͼ������
				
				////////////////////////////////////////////////////////////////////////////// 2014
				
				if(tempindex>=128) 
				{	dc.SetPixelV(width+483,height+180,RGB(255,tempindex,2*(tempindex-128)+1)); 
				}
				
				if(tempindex<128) 
				{
					dc.SetPixelV(width+483,height+180,RGB(2*tempindex,tempindex,0)); 
				}
				////////////////////////////////////////////////////////////////////////////// 2014
				
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
				if (voltagey> pianzhivoltagey)
				{
					voltagey=voltagey-movevoltagey;
				}
				if (voltagey <= pianzhivoltagey)
				{
					voltagey=pianzhivoltagey;
				}
				
				//*****************************************pci2003******************************************//
				DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
				PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
				//*****************************************pci2003******************************************//
				
				//�����û������ʱ�䣬y����ջ�ʱ�ӳ�һ��ʱ��
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
			//�����������һ����y����Ϊ���ƫ��λ��
			voltagey=pianzhivoltagey;
			//**************************************pci2003**********************************************//
			DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
			PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
			//***************************************pci2003*********************************************//
			
			QueryPerformanceCounter(&litmp);
			rowscanend=litmp.QuadPart;		
		}
		//�ָ�x������쳤�������x����ƫ��λ��
		
		for (i=0;i<width;i++)
		{
			//��Ϊѹ���մ�����У���󣬰���ԭ�������𽥽��͵�ѹ���ܻ�С��ԭ���趨��ƫ�õ�ѹֵ��������Ҫ�ж��������ж�
			//����У���������ѹ��Ϊ���Զ�ʹ��ѹ���մɵ��쳤��Ϊ����
			if (voltagex > pianzhivoltagex)
			{
				voltagex=voltagey-movevoltagex;
			}
			if (voltagex <= pianzhivoltagex)
			{
				voltagex=pianzhivoltagex;
			}
			//**************************************pci2003**********************************************//        
			DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
			PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
			//**************************************pci2003**********************************************//
			//�����û������ʱ�䣬x����ջ�ʱ�ӳ�һ��ʱ��
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
		
		//�����������һ����x����Ϊ���ƫ��λ��
		voltagex=pianzhivoltagex;
		//**************************************pci2003**********************************************//
		DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
		PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
		//**************************************pci2003**********************************************//
	}
	
	// *******************************************************************************************
	// Y����Ĵ�ͼʵʱɨ��
	if (m_radiobmpsize==1)
	{
		//����ϴ�ɨ���Ƿ�������ɣ����������ɾͼ�����ֵ��ʹ����ͼ��ťΪ��ʹ����ʵ�������������Ƿ��һ��ɨ��������ɣ�
		if (height==400&&width==400)
		{
			//�������ݲ�����zƽ����������С
			zmax=zmin=largebmpdata[0][0];
			zavetemp=0;
			for (height=0;height<400;height++)
			{
				for (width=0;width<400;width++)
				{
					//����һ��ͼ���е�z����z��СֵΪ���½绮��Ϊ255�ȷ֣���������
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
			//�Ե�ǰͼ����z���ֵ�İٷֱ���Ϊ��һ��ͼ���z���ֵ���ٷֱȼ�Ϊ�����������ֵ
			//			oldzmax=(float)((zmax-zmin)*lightnessthreshold/100.0+zmin);
			oldzmax=zmax;
			
			//������ʾ��ͼ��z������Сֵ���䵥λ
			sprintf(timechar," %1.2f",zmax);
			m_showzmax.SetWindowText(timechar);
			sprintf(timechar,"V");
			m_showzmaxdanwei.SetWindowText(timechar);
			m_showzmindanwei.SetWindowText(timechar);
			sprintf(timechar," %1.2f",zmin);
			m_showzmin.SetWindowText(timechar);
			
			//����ֲڶ�Ra��Ry��Rz
			//Ra����
			//��smallbmpbits�������õ������ĵ�ѹֵ�����������е�ƽ��ֵ�����ֵ����Сֵ
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
			
			//������ʾ��ͼ��Ra,Ry,Rzֵ���䵥λ
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
			
			
			//���ò���ͼ��ťΪ��ʹ
			m_buttonbmpcapture.EnableWindow(TRUE);
		}
		
		//����ÿһ���ص����ʱʱ��	
		delaytime=time/40000.0 - 10.0*systemdelay;
//      delaytime=time/40000.0;
		
		
		//****************************************************************************************
		
		// Y����Ĵ�ͼʵʱɨ�迪ʼ
		voltagex=pianzhivoltagex;
		for(width=0;width<400;width++)
		{
			//д��x����ĵ�ѹ�ź�
			voltagex=voltagex+movevoltagex;
			
			//У��ѹ���մɵķ�����
			voltagex=voltagex-voltagex*movevoltagex*ceramicscoefficient;
			//			voltagex=voltagex-voltagex*movevoltagex*0.10;
			
			//**************************************pci2003**********************************************//		
			DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
			PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
			//**************************************pci2003**********************************************//
			
			voltagey=pianzhivoltagey;
			
			QueryPerformanceCounter(&litmp);
			rowscanbegin=litmp.QuadPart;
			
			for(height=0;height<400;height++)
			{
				//д��y����ĵ�ѹ�ź�
				voltagey=voltagey+movevoltagey;
				
				//У��ѹ���մɵķ�����
				voltagey=voltagey-voltagey*movevoltagey*ceramicscoefficient;
				//				voltagey=voltagey-voltagey*movevoltagey*0.10;
				
				//**************************************pci2003**********************************************//		
				DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
				PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
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
				
				//�������ѭ������ֹɨ��
				if (quickstopflag==TRUE)
				{
					//**************************************pci2003**********************************************//	
					//2014/7 pci2003--->pci8620
                    voltagex=0;
					DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
					PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
					//**************************************pci2003**********************************************//

					//**************************************pci2003**********************************************//	
					//2014/7 pci2003--->pci8620
                    voltagey=0;
					DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
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
				
				//����һ��ͼ���е�z����z��СֵΪ���½绮��Ϊ255�ȷ֣���������
				if (largebmpdata[width][height]>oldzmax)
					tempbmpbits=oldzmax;
				if (largebmpdata[width][height]<oldzmin)
					tempbmpbits=oldzmin;
				if (largebmpdata[width][height]<=oldzmax&&largebmpdata[width][height]>=oldzmin)
					tempbmpbits=largebmpdata[width][height];
				
				tempindex=(BYTE)(255*(tempbmpbits-oldzmin)/(oldzmax-oldzmin));
				//�ڻ�ͼ������
				//				dc.SetPixelV(width+332,height+20,RGB(tempindex,tempindex*0.7,tempindex*0.3));  // 2005��9��23�� //
				
				////////////////////////////////////////////////////////////////////////////// 2014
				
				if(tempindex>=128) 
				{	dc.SetPixelV(width+332,height+20,RGB(255,tempindex,2*(tempindex-128)+1)); 
				}
				
				if(tempindex<128) 
				{
					dc.SetPixelV(width+332,height+20,RGB(2*tempindex,tempindex,0)); 
				}
				////////////////////////////////////////////////////////////////////////////// 2014
				
				
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
				if (voltagey > pianzhivoltagey)
				{
					voltagey=voltagey-movevoltagey;
				}
				if (voltagey <= pianzhivoltagey)
				{
					voltagey=pianzhivoltagey;
				}
				
				//**************************************pci2003**********************************************//
				DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
				PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
				//**************************************pci2003**********************************************//	    
				
				//�����û������ʱ�䣬y����ջ�ʱ�ӳ�һ��ʱ��
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
			//�����������һ����y����Ϊ���ƫ��λ��
			voltagey=pianzhivoltagey;
			//**************************************pci2003**********************************************//
			DAdata=(WORD)(((voltagey*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
			PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);   
			//**************************************pci2003**********************************************//
			
			
			QueryPerformanceCounter(&litmp);
			rowscanend=litmp.QuadPart;
			
		}
		//�ָ�x������쳤�������x����ƫ��λ��
		for (i=0;i<width;i++)
		{
			//��Ϊѹ���մ�����У���󣬰���ԭ�������𽥽��͵�ѹ���ܻ�С��ԭ���趨��ƫ�õ�ѹֵ��������Ҫ�ж��������ж�
			//����У���������ѹ��Ϊ���Զ�ʹ��ѹ���մɵ��쳤��Ϊ����
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
			DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
			PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
			
			//**************************************pci2003**********************************************//	   
			
			//�����û������ʱ�䣬x����ջ�ʱ�ӳ�һ��ʱ��
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
		//�����������һ����x����Ϊ���ƫ��λ��
		voltagex=pianzhivoltagex;
		//**************************************pci2003**********************************************//		
		DAdata=(WORD)(((voltagex*1000.0+5000.0)*4096)/10000.0);//����ѹֵת����LSB����
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
//ȡ��ƫ��ʱ�ӵĵ�ѹ��ʹѹ���մɻָ���0����
if (pianzhiflag==TRUE)
{
	//�ָ�x�����ѹ���մ�
	if (pianzhivoltagex!=0.0)
	{
        DAdata=0;
//		PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, XDIRECT);
//	       	DAdata=(WORD)(((voltagex*1000.0)*4096)/4250.0);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
//		    DAdata=(WORD)(voltagex*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
			DASingleOutV20(XDIRECT,DAdata);
	}
	//�ָ�y�����ѹ���մ�
	if (pianzhivoltagey!=0.0)
	{
		DAdata=0;
//		PCI8620_WriteDeviceDA(hDevice, OUTPUTRANGE, DAdata, YDIRECT);
//	       	DAdata=(WORD)(((voltagey*1000.0)*4096)/4250.0);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
//		    DAdata=(WORD)(voltagey*963.8);//����ѹֵת����LSB����//��5000.0��Ϊ4250.0��ʹ������ֵ�ӽ�USB�ɼ���������3.3V
			DASingleOutV20(YDIRECT,DAdata);
	}
}
//�ػ���ͼ��ʾ��
if (redrawflag==TRUE)
{
	Invalidate();
}

//����ʱ�䣬������һ��ѭ��
if (settimerflag==TRUE)
{
	SetTimer(1,5,NULL);
}
//ɨ�������־
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
	//���¶Ի����и��ؼ�����ֵ
	if (UpdateData(TRUE)==0)
	{
		//���ɨ�跶Χ�����д��󣬱���check״̬��ԭʼ״̬
		//checkflag��DoDataExchange�����ı�־��Ŀ����ֻ�öԻ������check�ؼ��������ݽ���	
		checkflag=FALSE;
		UpdateData(FALSE);
		checkflag=TRUE;
		return;
	}
	//����ƫ��ѡ����ѡ���������ʹƫ��������״̬
	if (m_enablepianzhiflag==TRUE)
	{
		CEdit* editpianzhix=(CEdit*)GetDlgItem(IDC_PIANZHIX);
		editpianzhix->EnableWindow(TRUE);
		CEdit* editpianzhiy=(CEdit*)GetDlgItem(IDC_PIANZHIY);
		editpianzhiy->EnableWindow(TRUE);
		//����ƫ�þ�̬�ı��ؼ�����ʾ��Χ�Ϳ�������������Сֵ��spin�ؼ���ֵ
		if (m_radiorange==0)
		{
			sprintf(timechar,"0 �� %d     ",(20-m_smallrangex));
			m_pianzhirangex.SetWindowText(timechar);
			
			sprintf(timechar,"0 �� %d     ",(20-m_smallrangey));
			m_pianzhirangey.SetWindowText(timechar);
			
			m_spinpianzhix.SetRange(0,20-m_smallrangex);
			m_spinpianzhiy.SetRange(0,20-m_smallrangey);
			
		}
		if (m_radiorange==1)
		{
			sprintf(timechar,"0 �� %d     ",(4000-m_largerangex));
			m_pianzhirangex.SetWindowText(timechar);
			
			sprintf(timechar,"0 �� %d     ",(4000-m_largerangey));
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
		//����ƫ�þ�̬�ı��ؼ�����ʾ��Χ�Ϳ�������������Сֵ��spin�ؼ���ֵ
		if (m_radiorange==0)
		{
			sprintf(timechar,"0 �� %d     ",(20-m_smallrangex));
			m_pianzhirangex.SetWindowText(timechar);
			
			sprintf(timechar,"0 �� %d     ",(20-m_smallrangey));
			m_pianzhirangey.SetWindowText(timechar);
			
			m_spinpianzhix.SetRange(0,20-m_smallrangex);
			m_spinpianzhiy.SetRange(0,20-m_smallrangey);
			
		}
		if (m_radiorange==1)
		{
			sprintf(timechar,"0 �� %d     ",(4000-m_largerangex));
			m_pianzhirangex.SetWindowText(timechar);
			
			sprintf(timechar,"0 �� %d     ",(4000-m_largerangey));
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
		MessageBox(_T("�ϴ�ɨ�軹δ���"),
		_T("���Ե�"), MB_ICONEXCLAMATION | MB_OK);	
}

void CMeasure::OnCancel() 
{
	//����OnCancel��������ʹ����Ч��Ŀ�����öԻ�����Ӧesc��	
	//	CDialog::OnCancel();
}

void CMeasure::OnOK() 
{
	//����OnOk��������ʹ����Ч��Ŀ�����öԻ�����Ӧenter��		
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
		AfxMessageBox(_T("�뱣��ɨ�跽��һ��"),MB_ICONEXCLAMATION | MB_OK);
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
		AfxMessageBox(_T("�뱣��ɨ�跽��һ��"),MB_ICONEXCLAMATION | MB_OK);
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
����: �������ÿؼ���EnabeWindow״̬ 
����: type 0(DEFAULT)ΪĬ��  1(EDIT)ΪCEdit�ؼ�  2(BUTTON)ΪCButton�ؼ�   
status ��Ҫ���õĿؼ���״̬ 
num ����Ŀؼ�����
... �ؼ�ID
*/
void CMeasure::SetStatus(int type, int status, int num,...)
{
	va_list argp;// ���溯�������Ľṹ  ...����
	int idc;  //ÿ���ؼ���ID
	
	va_start(argp, num);//��  ���ô���num��������ĵ�һ����ѡ����
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
		strValue.Format(_T("%1.1f"),(double)(nPos+10)/10);//��ʼֵ�Ա�ϵ��Ϊ1.0����Ӧ��nPosΪ0���Ա�ϵ�����1.6����
		((CSpinButtonCtrl*)pScrollBar)->GetBuddy()->SetWindowText(strValue);
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
