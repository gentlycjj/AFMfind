// BCAdjust.cpp : implementation file
//

#include "stdafx.h"
#include "microscope.h"
#include "microscopeView.h"
#include "BCAdjust.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBCAdjust dialog


CBCAdjust::CBCAdjust(CWnd* pParent /*=NULL*/)
	: CDialog(CBCAdjust::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBCAdjust)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBCAdjust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBCAdjust)
	DDX_Control(pDX, IDC_STATICCONTRAST, m_staticcontrast);
	DDX_Control(pDX, IDC_STATICBRIGHTNESS, m_staticbrightness);
	DDX_Control(pDX, IDC_SLIDERCONTRAST, m_slidercontrast);
	DDX_Control(pDX, IDC_SLIDERBRIGHTNESS, m_sliderbrightness);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBCAdjust, CDialog)
	//{{AFX_MSG_MAP(CBCAdjust)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCAdjust message handlers

BOOL CBCAdjust::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//������������ĳ�ʼֵ��������
	m_sliderbrightness.SetRange(-100,100,TRUE);
	m_sliderbrightness.SetTicFreq(1);

	m_slidercontrast.SetRange(-100,100,TRUE);
	m_slidercontrast.SetTicFreq(1);

    //������������ĳ�ʼ��ʾֵ
	m_sliderbrightness.SetPos(0);
	sprintf(bcchar," %d     ",0);
	m_staticbrightness.SetWindowText(bcchar);

	m_slidercontrast.SetPos(0);
	sprintf(bcchar," %d     ",0);
	m_staticcontrast.SetWindowText(bcchar);

	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)

	//��ȡԭʼͼ������

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
 
	/* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	//�õ�ͼ��ĸ߶ȺͿ��
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;

	//����DIBͼ�����ݿ��ʵ��ʹ�õ��ֽ���
	factwidth=WIDTHBYTES(width*8);

	//��̬����������ʱ�洢DIBͼ�����ݵ�����
	bmpoldbits=new BYTE*[factwidth];
	for (i=0;i<factwidth;i++)
		bmpoldbits[i]=new BYTE[height];

	bmpnewbits=new BYTE*[factwidth];
	for (i=0;i<factwidth;i++)
		bmpnewbits[i]=new BYTE[height];

	
	//��������ֵ����ά����
	for (j=0;j<height;j++)
		for (i=0;i<factwidth;i++)
			::memcpy(&bmpoldbits[i][j],lpbi+1064+i+j*factwidth,1);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBCAdjust::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	//���ȹ�����
	CSliderCtrl* sliderbrightness=(CSliderCtrl*)GetDlgItem(IDC_SLIDERBRIGHTNESS);	
    //�������ȹ�������ʾֵ
	sprintf(bcchar," %d     ",sliderbrightness->GetPos());
	m_staticbrightness.SetWindowText(bcchar);
    //�������Ȱٷֱ�
	brightness=(float)((sliderbrightness->GetPos()+100)/100.0);
	brightness=brightness*brightness;

	//�Աȶȹ�����
	CSliderCtrl* slidercontrast=(CSliderCtrl*)GetDlgItem(IDC_SLIDERCONTRAST);	
	//���¶Աȶȹ�������ʾֵ
	sprintf(bcchar," %d     ",slidercontrast->GetPos());
	m_staticcontrast.SetWindowText(bcchar);
    //����ԱȶȰٷֱ�
	contrast=(float)((slidercontrast->GetPos()+100)/100.0);
	contrast=contrast*contrast*contrast*contrast;

	//�ԱȶȺ����ȵ���
	for (i=0;i<factwidth;i++)
		{
		for (j=0;j<height;j++)
			{
				//�ı�Աȶ�
				if ((128+(bmpoldbits[i][j]-194)*contrast)<0) // 20080430, ԭ���ǣ�128
					bmpnewbits[i][j]=0;
				if ((128+(bmpoldbits[i][j]-194)*contrast)>255)
					bmpnewbits[i][j]=255;
				if ((128+(bmpoldbits[i][j]-194)*contrast)>=0&&(128+(bmpoldbits[i][j]-194)*contrast)<=255)
					bmpnewbits[i][j]=(BYTE)(128+(bmpoldbits[i][j]-194)*contrast);
				//�ı�����
				if (bmpnewbits[i][j]*brightness<0)  
					bmpnewbits[i][j]=0;
				if (bmpnewbits[i][j]*brightness>255)
					bmpnewbits[i][j]=255;
				if ((bmpnewbits[i][j]*brightness)>=0&&(bmpnewbits[i][j]*brightness)<=255)
					bmpnewbits[i][j]=(BYTE)(bmpnewbits[i][j]*brightness);
			}
		}
	//д��仯�������ֵ
	for (j=0;j<height;j++)
		for (i=0;i<factwidth;i++)
			::memcpy(lpbi+1064+i+j*factwidth,&bmpnewbits[i][j],1);

	//������ͼ
		
	//�õ�MDI����������ָ��
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	//�õ�MDI�����Ӵ���ָ��
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
		{
		return; // no active MDI child frame 
		}

	//�õ�MDI������ָ��
	CMicroscopeView* pView=(CMicroscopeView*)pChild->GetActiveView();

	pView->Invalidate(FALSE);  //�����ӣ�����������
	
}

void CBCAdjust::OnOK() 
{

	::GlobalUnlock((HGLOBAL)hDIB);

	//�ͷŶ�̬������ڴ�
	for (i=0;i<factwidth;i++)
	{
		delete bmpoldbits[i];
		delete bmpnewbits[i];
	}
	
	delete bmpoldbits;
	delete bmpnewbits;

	CDialog::OnOK();
}

void CBCAdjust::OnCancel() 
{
		//�ָ�ԭ����ͼ��
		for (j=0;j<height;j++)
			for (i=0;i<factwidth;i++)
				::memcpy(lpbi+1064+i+j*factwidth,&bmpoldbits[i][j],1);
		
		//������ͼ
		
		//�õ�MDI����������ָ��
		CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		//�õ�MDI�����Ӵ���ָ��
		CMDIChildWnd* pChild = pFrame->MDIGetActive();

		if (pChild == NULL)
			{
		return; // no active MDI child frame 
			}

		//�õ�MDI������ָ��
		CMicroscopeView* pView=(CMicroscopeView*)pChild->GetActiveView();

		pView->Invalidate(FALSE);  //�����ӣ�����������

		::GlobalUnlock((HGLOBAL)hDIB);

		//�ͷŶ�̬������ڴ�
		for (i=0;i<factwidth;i++)
		{
		delete bmpoldbits[i];
		delete bmpnewbits[i];
		}
	
		delete bmpoldbits;
		delete bmpnewbits;
	
		CDialog::OnCancel();
}
