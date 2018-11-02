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
	
	//设置两个滑块的初始值及起属性
	m_sliderbrightness.SetRange(-100,100,TRUE);
	m_sliderbrightness.SetTicFreq(1);

	m_slidercontrast.SetRange(-100,100,TRUE);
	m_slidercontrast.SetTicFreq(1);

    //设置两个滑块的初始显示值
	m_sliderbrightness.SetPos(0);
	sprintf(bcchar," %d     ",0);
	m_staticbrightness.SetWindowText(bcchar);

	m_slidercontrast.SetPos(0);
	sprintf(bcchar," %d     ",0);
	m_staticcontrast.SetWindowText(bcchar);

	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)

	//读取原始图像数据

	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
 
	/* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	//得到图像的高度和宽度
    width=lpbmi->bmiHeader.biWidth;
	height=lpbmi->bmiHeader.biHeight;

	//计算DIB图像数据宽度实际使用的字节数
	factwidth=WIDTHBYTES(width*8);

	//动态申请用来临时存储DIB图像数据的数组
	bmpoldbits=new BYTE*[factwidth];
	for (i=0;i<factwidth;i++)
		bmpoldbits[i]=new BYTE[height];

	bmpnewbits=new BYTE*[factwidth];
	for (i=0;i<factwidth;i++)
		bmpnewbits[i]=new BYTE[height];

	
	//复制索引值到二维数组
	for (j=0;j<height;j++)
		for (i=0;i<factwidth;i++)
			::memcpy(&bmpoldbits[i][j],lpbi+1064+i+j*factwidth,1);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBCAdjust::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	//亮度滚动条
	CSliderCtrl* sliderbrightness=(CSliderCtrl*)GetDlgItem(IDC_SLIDERBRIGHTNESS);	
    //更新亮度滚动条显示值
	sprintf(bcchar," %d     ",sliderbrightness->GetPos());
	m_staticbrightness.SetWindowText(bcchar);
    //计算亮度百分比
	brightness=(float)((sliderbrightness->GetPos()+100)/100.0);
	brightness=brightness*brightness;

	//对比度滚动条
	CSliderCtrl* slidercontrast=(CSliderCtrl*)GetDlgItem(IDC_SLIDERCONTRAST);	
	//更新对比度滚动条显示值
	sprintf(bcchar," %d     ",slidercontrast->GetPos());
	m_staticcontrast.SetWindowText(bcchar);
    //计算对比度百分比
	contrast=(float)((slidercontrast->GetPos()+100)/100.0);
	contrast=contrast*contrast*contrast*contrast;

	//对比度和亮度调整
	for (i=0;i<factwidth;i++)
		{
		for (j=0;j<height;j++)
			{
				//改变对比度
				if ((128+(bmpoldbits[i][j]-194)*contrast)<0) // 20080430, 原来是－128
					bmpnewbits[i][j]=0;
				if ((128+(bmpoldbits[i][j]-194)*contrast)>255)
					bmpnewbits[i][j]=255;
				if ((128+(bmpoldbits[i][j]-194)*contrast)>=0&&(128+(bmpoldbits[i][j]-194)*contrast)<=255)
					bmpnewbits[i][j]=(BYTE)(128+(bmpoldbits[i][j]-194)*contrast);
				//改变亮度
				if (bmpnewbits[i][j]*brightness<0)  
					bmpnewbits[i][j]=0;
				if (bmpnewbits[i][j]*brightness>255)
					bmpnewbits[i][j]=255;
				if ((bmpnewbits[i][j]*brightness)>=0&&(bmpnewbits[i][j]*brightness)<=255)
					bmpnewbits[i][j]=(BYTE)(bmpnewbits[i][j]*brightness);
			}
		}
	//写入变化后的索引值
	for (j=0;j<height;j++)
		for (i=0;i<factwidth;i++)
			::memcpy(lpbi+1064+i+j*factwidth,&bmpnewbits[i][j],1);

	//更新视图
		
	//得到MDI程序主窗口指针
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	//得到MDI程序活动子窗口指针
	CMDIChildWnd* pChild = pFrame->MDIGetActive();

	if (pChild == NULL)
		{
		return; // no active MDI child frame 
		}

	//得到MDI程序活动视指针
	CMicroscopeView* pView=(CMicroscopeView*)pChild->GetActiveView();

	pView->Invalidate(FALSE);  //更新视，不背景擦除
	
}

void CBCAdjust::OnOK() 
{

	::GlobalUnlock((HGLOBAL)hDIB);

	//释放动态申请的内存
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
		//恢复原来的图像
		for (j=0;j<height;j++)
			for (i=0;i<factwidth;i++)
				::memcpy(lpbi+1064+i+j*factwidth,&bmpoldbits[i][j],1);
		
		//更新视图
		
		//得到MDI程序主窗口指针
		CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		//得到MDI程序活动子窗口指针
		CMDIChildWnd* pChild = pFrame->MDIGetActive();

		if (pChild == NULL)
			{
		return; // no active MDI child frame 
			}

		//得到MDI程序活动视指针
		CMicroscopeView* pView=(CMicroscopeView*)pChild->GetActiveView();

		pView->Invalidate(FALSE);  //更新视，不背景擦除

		::GlobalUnlock((HGLOBAL)hDIB);

		//释放动态申请的内存
		for (i=0;i<factwidth;i++)
		{
		delete bmpoldbits[i];
		delete bmpnewbits[i];
		}
	
		delete bmpoldbits;
		delete bmpnewbits;
	
		CDialog::OnCancel();
}
