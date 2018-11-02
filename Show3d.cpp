// Show3d.cpp : implementation file
//

#include "stdafx.h"
#include "afx.h"
#include "microscope.h"
#include "Show3d.h"
#include "microscopeDoc.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShow3d dialog

GLubyte m_RasterFont[][13] = //unsigned char
{
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x36, 0x36, 0x36}, 
	{0x00, 0x00, 0x00, 0x66, 0x66, 0xff, 0x66, 0x66, 0xff, 0x66, 0x66, 0x00, 0x00}, 
	{0x00, 0x00, 0x18, 0x7e, 0xff, 0x1b, 0x1f, 0x7e, 0xf8, 0xd8, 0xff, 0x7e, 0x18}, 
	{0x00, 0x00, 0x0e, 0x1b, 0xdb, 0x6e, 0x30, 0x18, 0x0c, 0x76, 0xdb, 0xd8, 0x70}, 
	{0x00, 0x00, 0x7f, 0xc6, 0xcf, 0xd8, 0x70, 0x70, 0xd8, 0xcc, 0xcc, 0x6c, 0x38}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1c, 0x0c, 0x0e}, 
	{0x00, 0x00, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c}, 
	{0x00, 0x00, 0x30, 0x18, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x18, 0x30}, 
	{0x00, 0x00, 0x00, 0x00, 0x99, 0x5a, 0x3c, 0xff, 0x3c, 0x5a, 0x99, 0x00, 0x00}, 
	{0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x00, 0x00}, 
	{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x60, 0x60, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03}, 
	{0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c}, 
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18}, 
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e}, 
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e}, 
	{0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c}, 
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff}, 
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e}, 
	{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0x03, 0xff}, 
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e}, 
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e}, 
	{0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x30, 0x18, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06}, 
	{0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x06, 0x0c, 0x18, 0x30, 0x60}, 
	{0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x18, 0x0c, 0x06, 0x03, 0xc3, 0xc3, 0x7e}, 
	{0x00, 0x00, 0x3f, 0x60, 0xcf, 0xdb, 0xd3, 0xdd, 0xc3, 0x7e, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18}, 
	{0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe}, 
	{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e}, 
	{0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc}, 
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff}, 
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff}, 
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e}, 
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, 
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e}, 
	{0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06}, 
	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3}, 
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0}, 
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3}, 
	{0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3}, 
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e}, 
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe}, 
	{0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c}, 
	{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe}, 
	{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e}, 
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff}, 
	{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, 
	{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, 
	{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, 
	{0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3}, 
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3}, 
	{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff}, 
	{0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c}, 
	{0x00, 0x03, 0x03, 0x06, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x60}, 
	{0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x3c}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18}, 
	{0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x30, 0x70}, 
	{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0x7f, 0x03, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0}, 
	{0x00, 0x00, 0x7e, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x03, 0x03, 0x03, 0x03, 0x03}, 
	{0x00, 0x00, 0x7f, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x33, 0x1e}, 
	{0x7e, 0xc3, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0x7e, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0}, 
	{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00}, 
	{0x38, 0x6c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x00}, 
	{0x00, 0x00, 0xc6, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc, 0xc6, 0xc0, 0xc0, 0xc0, 0xc0}, 
	{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78}, 
	{0x00, 0x00, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xfe, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfc, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00}, 
	{0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0x00, 0x00, 0x00, 0x00}, 
	{0x03, 0x03, 0x03, 0x7f, 0xc3, 0xc3, 0xc3, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xfe, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xfe, 0x03, 0x03, 0x7e, 0xc0, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x1c, 0x36, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x00}, 
	{0x00, 0x00, 0x7e, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xdb, 0xc3, 0xc3, 0xc3, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00}, 
	{0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0xff, 0x60, 0x30, 0x18, 0x0c, 0x06, 0xff, 0x00, 0x00, 0x00, 0x00}, 
	{0x00, 0x00, 0x0f, 0x18, 0x18, 0x18, 0x38, 0xf0, 0x38, 0x18, 0x18, 0x18, 0x0f}, 
	{0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18}, 
	{0x00, 0x00, 0xf0, 0x18, 0x18, 0x18, 0x1c, 0x0f, 0x1c, 0x18, 0x18, 0x18, 0xf0}, 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x8f, 0xf1, 0x60, 0x00, 0x00, 0x00} 
};


CShow3d::CShow3d(CWnd* pParent /*=NULL*/)
	: CDialog(CShow3d::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShow3d)
	//}}AFX_DATA_INIT
	m_pDC = NULL;

	m_xRotate = -84.0;   //******** 原来值是-73.8 *******//
	m_yRotate = -45.0;   //******** 原来值是-23.3 *******//

	m_LeftButtonDown=FALSE;
	m_BackgroundColor=RGB(208,208,208);  // 2008.01.05 改变背景颜色 //        
	m_LineColor=RGB(0.0,0.0,0.0);  // 2008.01.05 改变背景颜色 // 
	m_nFontOffset	= 0;
	contrast=(float)1.0;
	brightness=(float)1.0;
	solid=(float)1.0;
	position=-(float)(solid/2.0);
	axisflag=FALSE;
	legendflag=FALSE;
	xrotateflag=FALSE;
	yrotateflag=FALSE;
	randomrotateflag=FALSE;
	stoprotateflag=FALSE;
}


void CShow3d::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShow3d)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShow3d, CDialog)
	//{{AFX_MSG_MAP(CShow3d)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_POPUP_BACKGROUND, OnPopupBackground)
	ON_COMMAND(ID_POPUP_FRONT, OnPopupFront)	
	ON_COMMAND(ID_POPUP_LINE, OnPopupLine)
	ON_COMMAND(ID_POPUP_ADDCONTRAST, OnPopupAddcontrast)
	ON_COMMAND(ID_POPUP_RESETCONTRAST, OnPopupResetcontrast)
	ON_COMMAND(ID_POPUP_SUBTRACTCONTRAST, OnPopupSubtractcontrast)
	ON_COMMAND(ID_POPUP_ADDBRIGHTNESS, OnPopupAddbrightness)
	ON_COMMAND(ID_POPUP_RESETBRIGHTNESS, OnPopupResetbrightness)
	ON_COMMAND(ID_POPUP_SUBTRACTBRIGHTNESS, OnPopupSubtractbrightness)
	ON_COMMAND(ID_POPUP_ADDSOLID, OnPopupAddsolid)
	ON_COMMAND(ID_POPUP_RESETSOLID, OnPopupResetsolid)
	ON_COMMAND(ID_POPUP_SUBTRACTSOLID, OnPopupSubtractsolid)
	ON_COMMAND(ID_POPUP_IMAGEUPPER, OnPopupImageupper)
	ON_COMMAND(ID_POPUP_IMAGELOWER, OnPopupImagelower)
	ON_COMMAND(ID_POPUP_IMAGERESET, OnPopupImagereset)
	ON_COMMAND(ID_POPUP_CAPTUREIMAGE, OnPopupCaptureimage)
	ON_COMMAND(ID_POPUP_CLOSE, OnPopupClose)
	ON_COMMAND(ID_POPUP_ADDAXIS, OnPopupAddaxis)
	ON_COMMAND(ID_POPUP_SUBTRACTAXIS, OnPopupSubtractaxis)
	ON_COMMAND(ID_POPUP_ADDLEGEND, OnPopupAddlegend)
	ON_COMMAND(ID_POPUP_DELETELEGEND, OnPopupDeletelegend)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_POPUP_XROTATE, OnPopupXrotate)
	ON_COMMAND(ID_POPUP_YROTATE, OnPopupYrotate)
	ON_COMMAND(ID_POPUP_RANDOMROTATE, OnPopupRandomrotate)
	ON_COMMAND(ID_POPUP_STOPTOTATE, OnPopupStoptotate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShow3d message handlers

void CShow3d::OnOK() 
{	
//	CDialog::OnOK();   使之无效
}

void CShow3d::OnCancel() 
{
	CDialog::OnCancel();  
}





BOOL CShow3d::PreCreateWindow(CREATESTRUCT& cs) 
{
	// An OpenGL window must be created with the following flags and must not
	// include CS_PARENTDC for the class style. Refer to SetPixelFormat
	// documentation in the "Comments" section for further information.
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	
	return CDialog::PreCreateWindow(cs);
}

void CShow3d::OnDestroy() 
{
	CDialog::OnDestroy();
	
    wglMakeCurrent(NULL,NULL);  //释放与m_hDC 对应的 RC  将当前绘图情景对象赋空

	if (m_hRC)
		wglDeleteContext(m_hRC);//删除RC

	if (m_pDC)
        delete m_pDC;   //删除DC

	//释放动态申请的内存
	for (i=0;i<oldfactwidth;i++)
	{
		delete bmpoldbits[i];
	}
	
	delete bmpoldbits;
}

int CShow3d::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	BeginWaitCursor();
	
	Init();	

	glOrtho(-1.0,1.0,-1.0,1.0,-11.0,11.0);//?????

	EndWaitCursor();

    //	设置时间间隔
	SetTimer(0,5,NULL);
	return 0;
}

void CShow3d::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	DrawScene();  //具体的绘图函数，在RC中绘制

	//把RC中所绘传到当前的DC上，从而在屏幕上显示
	SwapBuffers(wglGetCurrentDC());// 交换前后缓存
	
}

void CShow3d::Init()
{
	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);

	if (!bSetupPixelFormat())
		return;
	
	m_hRC = wglCreateContext(m_pDC->GetSafeHdc());//创建 RC
	wglMakeCurrent(m_pDC->GetSafeHdc(),m_hRC);//使RC与当前DC相关联

}

BOOL CShow3d::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER |			// double buffered
		  PFD_STEREO_DONTCARE,          // do not care stereo   
		PFD_TYPE_RGBA,                  // RGBA type
		32,                             // 24-bit color depth
		8, 16, 8, 8, 8, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		64,                              // no accumulation buffer
		16, 16, 16, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		8,                              // 8 stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;

	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
	{
		MessageBox("ChoosePixelFormat failed");
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat failed");
		return FALSE;
	}

	return TRUE;

}

void CShow3d::DrawScene()
{
//将颜色缓存清为glClearColor命令所设置的颜色，即背景色
	glClearColor((float)GetRValue(m_BackgroundColor)/255.0f,(float)GetGValue(m_BackgroundColor)/255.0f,(float)GetBValue(m_BackgroundColor)/255.0f,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	LONG ihalf;         //图像宽度的半值
	LONG jhalf;         //图像高度的半值
	ihalf=oldwidth/2;  
	jhalf=oldheight/2;  
	BYTE redcolor; //亮度、对比度调整后新红色的数值
	BYTE bluecolor; //亮度、对比度调整后新蓝色的数值
	BYTE greencolor; //亮度、对比度调整后新绿色的数值

	BYTE redcolor1; //亮度、对比度调整后新红色的数值
	BYTE bluecolor1; //亮度、对比度调整后新蓝色的数值
	BYTE greencolor1; //亮度、对比度调整后新绿色的数值

	float zvalue;  //图像z方向调整后z的新数值
	float zvalue1;
	float zvaluemax;
	float temp;

	glPushMatrix();

		glTranslated(0.0, 0.0, (double)solid);//平移
		glRotated(m_xRotate, 1.0, 0.0, 0.0);//绕x向旋转
		glRotated(m_yRotate, 0.0, 1.0, 0.0);//绕y向旋转
        glShadeModel(GL_SMOOTH);//?????

//***************************************************//加标尺
		temp=0;
	for(i=0;i<oldwidth;i++)
	    for(j=0;j<oldheight;j++)
		{
			temp=temp+bmpoldbits[i][j]*solid/255.0+position;
		}
		temp=temp/(oldwidth*oldheight);

 if(axisflag==TRUE)
	{
		zvalue=(float)(bmpoldbits[0][0]*solid/255.0+position);
        zvaluemax=(float)(bmpoldbits[0][0]*solid/255.0+position);
		for(i=-ihalf;i<ihalf;i++)
		{
          for(j=jhalf-2;j>-jhalf-1;j--)
		  {
		    if(zvalue>(float)(bmpoldbits[i+ihalf][j+jhalf]*solid/255.0+position))
		     zvalue=(float)(bmpoldbits[i+ihalf][j+jhalf]*solid/255.0+position);

			if(zvaluemax<(float)(bmpoldbits[i+ihalf][j+jhalf]*solid/255.0+position))
		     zvaluemax=(float)(bmpoldbits[i+ihalf][j+jhalf]*solid/255.0+position);
		  }
		}
		glBegin(GL_LINES);

//		glColor3d(1.0,1.0,0.0); //Draw3DLine

	    glColor3d((float)GetRValue(m_LineColor)/255.0f,(float)GetGValue(m_LineColor)/255.0f,(float)GetBValue(m_LineColor)/255.0f);

        zvalue=zvalue-0.8;

		glVertex3d((double)(-ihalf)/oldwidth,zvalue,(double)(-jhalf)/oldheight);
		glVertex3d((double)(-ihalf)/oldwidth,zvalue,(double)(jhalf)/oldheight);//画y向坐标线

        glVertex3d((double)(-ihalf)/oldwidth,zvalue,(double)(-1*jhalf/5)/oldheight);
		glVertex3d((double)(-ihalf)/oldwidth,(zvalue+0.1),(double)(-1*jhalf/5)/oldheight);//画y向坐标线2/5刻度

        glVertex3d((double)(-ihalf)/oldwidth,zvalue,(double)(-3*jhalf/5)/oldheight);
		glVertex3d((double)(-ihalf)/oldwidth,(zvalue+0.1),(double)(-3*jhalf/5)/oldheight);//画y向坐标线1/5刻度

        glVertex3d((double)(-ihalf)/oldwidth,zvalue,(double)(1*jhalf/5)/oldheight);
		glVertex3d((double)(-ihalf)/oldwidth,(zvalue+0.1),(double)(1*jhalf/5)/oldheight);//画y向坐标线3/5刻度

		glVertex3d((double)(-ihalf)/oldwidth,zvalue,(double)(3*jhalf/5)/oldheight);
		glVertex3d((double)(-ihalf)/oldwidth,(zvalue+0.1),(double)(3*jhalf/5)/oldheight);//画y向坐标线4/5刻度

        glVertex3d((double)(-ihalf)/oldwidth-0.001,zvalue,(double)(-jhalf)/oldheight);
		glVertex3d((double)(ihalf)/oldwidth-0.001,zvalue,(double)(-jhalf)/oldheight);//画x向坐标线

		glVertex3d((double)(-3*ihalf/5)/oldwidth,zvalue,(double)(-jhalf)/oldheight);
		glVertex3d((double)(-3*ihalf/5)/oldwidth,(zvalue+0.1),(double)(-jhalf)/oldheight);//画x向坐标线1/5刻度

		glVertex3d((double)(-ihalf/5)/oldwidth,zvalue,(double)(-jhalf)/oldheight);
		glVertex3d((double)(-ihalf/5)/oldwidth,(zvalue+0.1),(double)(-jhalf)/oldheight);//画x向坐标线2/5刻度

        glVertex3d((double)(1*ihalf/5)/oldwidth,zvalue,(double)(-jhalf)/oldheight);
		glVertex3d((double)(1*ihalf/5)/oldwidth,(zvalue+0.1),(double)(-jhalf)/oldheight);//画x向坐标线3/5刻度

		glVertex3d((double)(3*ihalf/5)/oldwidth,zvalue,(double)(-jhalf)/oldheight);
		glVertex3d((double)(3*ihalf/5)/oldwidth,(zvalue+0.1),(double)(-jhalf)/oldheight);//画x向坐标线4/5刻度

        glVertex3d((double)(-ihalf)/oldwidth,zvalue,(double)(-jhalf)/oldheight);
        zvalue1=(float)(bmpoldbits[0][1]*solid/255.0+position);
		glVertex3d((double)(-ihalf)/oldwidth,zvalue1,(double)(-jhalf)/oldheight);//画z向坐标线
        		
        glVertex3d((double)(-ihalf)/oldwidth+0.001,zvalue,(double)(jhalf)/oldheight);
        //zvalue1=(float)(bmpoldbits[0][0]*solid/255.0+position);
		//glVertex3d((double)(-ihalf)/oldwidth,zvalue1,(double)(-jhalf)/oldheight);//画左上角坐标线
        //glVertex3d((double)(-ihalf)/oldwidth,zvalue1,(double)(-jhalf)/oldheight);
	    glVertex3d((double)(-ihalf)/oldwidth+0.001,zvaluemax+0.11,(double)(jhalf)/oldheight);//画到最高刻度

		glVertex3d((double)(-ihalf)/oldwidth-0.005,zvaluemax,(double)(jhalf)/oldheight-0.005);
        glVertex3d((double)(-ihalf)/oldwidth+0.005,zvaluemax,(double)(jhalf)/oldheight+0.005);//z向最高刻度

        //glVertex3d((double)(-ihalf)/oldwidth-0.01,temp,(double)(-jhalf)/oldheight-0.01);
        //glVertex3d((double)(-ihalf)/oldwidth+0.01,temp,(double)(-jhalf)/oldheight+0.01);//z向平均刻度线
 
        glVertex3d((double)(ihalf)/oldwidth,zvalue,(double)(-jhalf)/oldheight+0.001);
        zvalue1=(float)(bmpoldbits[2*(ihalf-1)+1][1]*solid/255.0+position);
		glVertex3d((double)(ihalf)/oldwidth,zvalue1,(double)(-jhalf)/oldheight+0.001);//画右下角坐标线

       // glVertex3d((double)(ihalf)/oldwidth,zvalue,(double)(-jhalf)/oldheight);
	   // glVertex3d((double)(ihalf)/oldwidth,zvalue,(double)(jhalf)/oldheight);//画x向坐标线
	
        //ihalf为y向,jhalf为x向
        //(ihalf/oldwith,-jhalf/oldwidth)为左下角顶点
		//(ihalf/oldwith,jhalf/oldwidth)为右下角顶点
		//(-ihalf/oldwith,-jhalf/oldwidth)为左上角顶点
        //(-ihalf/oldwith,jhalf/oldwidth)为右上角顶点
        //画刻度线以及每条刻度线对应的值
	    glEnd();

		wglUseFontBitmaps(wglGetCurrentDC(),0,256,1);
        glListBase(1); 
		glRasterPos3d((double)(ihalf)/oldwidth+0.02,zvalue-0.22,(double)(-jhalf)/oldheight+0.03);
        glCallLists(4,GL_UNSIGNED_BYTE,"  nm");//写单位
        
		//***********************************************************************//
		//写x向刻度值
		glRasterPos3d((double)(-ihalf)/oldwidth-0.06,zvalue,(double)(-jhalf)/oldheight-0.05);
        glCallLists(1,GL_UNSIGNED_BYTE,"0");//x最大刻度值

		glRasterPos3d((double)(-3*ihalf/5)/oldwidth-0.04,zvalue,(double)(-jhalf)/oldheight-0.06);
        sprintf(timechar,"%d",(int)scanrangex/5);
		glCallLists(takeinteger((double)scanrangex/5),GL_UNSIGNED_BYTE,timechar);//x1/5刻度值

		glRasterPos3d((double)(-ihalf/5)/oldwidth-0.04,zvalue,(double)(-jhalf)/oldheight-0.06);
        sprintf(timechar,"%d",(int)2*scanrangex/5);
		glCallLists(takeinteger((double)2*scanrangex/5),GL_UNSIGNED_BYTE,timechar);//x2/5刻度值
        
		int i=0;
		float j=0.00;
		i=takeinteger((double)3*scanrangex/5);
		if(i>3)
		 j=0.00;
		glRasterPos3d((double)(ihalf/5)/oldwidth-0.04,zvalue,(double)(-jhalf)/oldheight-0.06-j);
        sprintf(timechar,"%d",(int)3*scanrangex/5);
		glCallLists(takeinteger((double)3*scanrangex/5),GL_UNSIGNED_BYTE,timechar);//x3/5刻度值

        i=takeinteger((double)4*scanrangex/5);
		if(i>3)
		j=0.00;
		glRasterPos3d((double)(3*ihalf/5)/oldwidth-0.04,zvalue,(double)(-jhalf)/oldheight-0.06-j);
        sprintf(timechar,"%d",(int)4*scanrangex/5);
		glCallLists(takeinteger((double)4*scanrangex/5),GL_UNSIGNED_BYTE,timechar);//x4/5刻度值
        
		i=takeinteger((double)scanrangex);
		if(i>3)
		j=0.01;
		glRasterPos3d((double)(ihalf)/oldwidth-0.04,zvalue,(double)(-jhalf)/oldheight-0.05-j);
		sprintf(timechar,"%d",(int)scanrangex);
		glCallLists(takeinteger((double)scanrangex),GL_UNSIGNED_BYTE,timechar);//x最大刻度值
        //glCallLists(1,GL_UNSIGNED_BYTE,"0");//x0刻度值
        
		//************************************************************************//
		//写y向刻度值
        i=takeinteger((double)3*scanrangex/5);
		if(i>1)
		j=0.02;
		glRasterPos3d((double)(-ihalf)/oldwidth-0.13-j,zvalue+0.3,(double)(-3*jhalf/5)/oldheight-0.05);
        sprintf(timechar,"%d",(int)scanrangey/5);
		glCallLists(takeinteger((double)scanrangey/5),GL_UNSIGNED_BYTE,timechar);//y1/5刻度值
        
		i=takeinteger((double)3*scanrangex/5);
		if(i>1)
		j=0.03;
		glRasterPos3d((double)(-ihalf)/oldwidth-0.13-j,zvalue+0.3,(double)(-jhalf/5)/oldheight-0.05);
        sprintf(timechar,"%d",(int)2*scanrangey/5);
		glCallLists(takeinteger((double)2*scanrangey/5),GL_UNSIGNED_BYTE,timechar);//y2/5刻度值
        
        i=takeinteger((double)3*scanrangex/5);
		if(i>3)
		j=0.03;
		glRasterPos3d((double)(-ihalf)/oldwidth-0.13-j,zvalue+0.3,(double)(jhalf/5)/oldheight-0.05);
        sprintf(timechar,"%d",(int)3*scanrangey/5);
		glCallLists(takeinteger((double)3*scanrangey/5),GL_UNSIGNED_BYTE,timechar);//y3/5刻度值
        
        i=takeinteger((double)4*scanrangex/5);
		if(i>3)
		j=0.03;
		glRasterPos3d((double)(-ihalf)/oldwidth-0.13-j,zvalue+0.3,(double)(3*jhalf/5)/oldheight-0.05);
        sprintf(timechar,"%d",(int)4*scanrangey/5);
		glCallLists(takeinteger((double)4*scanrangey/5),GL_UNSIGNED_BYTE,timechar);//y4/5刻度值
        
        i=takeinteger((double)scanrangex);
		if(i>3)
		j=0.03;
		glRasterPos3d((double)(-ihalf)/oldwidth-0.13-j,zvalue+0.3,(double)(jhalf)/oldheight-0.05);
        sprintf(timechar,"%d",(int)scanrangey);
		glCallLists(takeinteger((double)scanrangey),GL_UNSIGNED_BYTE,timechar);//y最大刻度值
        //glCallLists(1,GL_UNSIGNED_BYTE,"0");//y0刻度值

        //*******************************************************************************//
		//写z向刻度值
        sprintf(timechar,"%f",(float)((zmax2-zmin2)*300.0)); // 此值随高度/电压系数改变 // AD值1V等于高压77V，恒力模式时，等于压电陶瓷伸缩77×6nm＝450nm
       
		
		if((zmax2-zmin2)*300.0>100.0) // 此值随高度/电压系数改变 //
		{
		   glRasterPos3d((double)(-ihalf)/oldwidth-0.11,zvaluemax,(double)(jhalf)/oldheight+0.07);
		   glCallLists(5,GL_UNSIGNED_BYTE,timechar);
		}
 
		if((zmax2-zmin2)*300.0<=100.0) // 此值随高度/电压系数改变 //
		{
		   glRasterPos3d((double)(-ihalf)/oldwidth-0.095,zvaluemax,(double)(jhalf)/oldheight+0.06);
		   glCallLists(4,GL_UNSIGNED_BYTE,timechar);
		}

		glRasterPos3d((double)(-ihalf)/oldwidth-0.135,zvaluemax,(double)(jhalf)/oldheight+0.02);
		glCallLists(4,GL_UNSIGNED_BYTE,"  nm");//z向刻度值
		
 } 
        //***************************************************************************//
 
		for (j=jhalf-2;j>-jhalf-1;j--)
		{
			glBegin(GL_QUAD_STRIP);

			for (i=-ihalf;i<ihalf;i++)
			{
				//画第一个顶点
				//对比度调整，设置红色
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbRed-128)*contrast)<0)
					redcolor=0;
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbRed-128)*contrast)>255)
					redcolor=255;
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbRed-128)*contrast)>=0&&(128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbRed-128)*contrast)<=255)
					redcolor=(BYTE)(128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbRed-128)*contrast);
				//亮度调整，设置红色
				if (redcolor*brightness<0)
					redcolor=0;
				if (redcolor*brightness>255)
					redcolor=255;
				if (redcolor*brightness>=0&&redcolor*brightness<=255)
					redcolor=(BYTE)(redcolor*brightness);

				//对比度调整，设置蓝色
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbBlue-128)*contrast)<0)
					bluecolor=0;
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbBlue-128)*contrast)>255)
					bluecolor=255;
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbBlue-128)*contrast)>=0&&(128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbBlue-128)*contrast)<=255)
					bluecolor=(BYTE)(128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbBlue-128)*contrast);
				//亮度调整，设置蓝色
				if (bluecolor*brightness<0)
					bluecolor=0;
				if (bluecolor*brightness>255)
					bluecolor=255;
				if (bluecolor*brightness>=0&&bluecolor*brightness<=255)
					bluecolor=(BYTE)(bluecolor*brightness);

				//对比度调整，设置绿色
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbGreen-128)*contrast)<0)
					greencolor=0;
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbGreen-128)*contrast)>255)
					greencolor=255;
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbGreen-128)*contrast)>=0&&(128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbGreen-128)*contrast)<=255)
					greencolor=(BYTE)(128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+jhalf]].rgbGreen-128)*contrast);
				//亮度调整，设置绿色
				if (greencolor*brightness<0)
					greencolor=0;
				if (greencolor*brightness>255)
					greencolor=255;
				if (greencolor*brightness>=0&&greencolor*brightness<=255)
					greencolor=(BYTE)(greencolor*brightness);
				
//////////////////////// 20140504
                if((redcolor+greencolor+bluecolor)>=384)
				{
				   redcolor1=255.0;
				   greencolor1=(redcolor+greencolor+bluecolor)/3.0;
				   bluecolor1=2*((redcolor+greencolor+bluecolor)/3.0-128)+1;
				}

                if((redcolor+greencolor+bluecolor)<384)
				{
				   redcolor1=2*(redcolor+greencolor+bluecolor)/3.0;
				   greencolor1=(redcolor+greencolor+bluecolor)/3.0;
				   bluecolor1=0;
				}
///////////////////////////////////// 20140504	
				
				//设置第一个顶点的颜色
//    			glColor3d(redcolor/255.0, 0.7*greencolor/255.0, 0.3*bluecolor/255.0);
				glColor3d(redcolor1/255.0, greencolor1/255.0, bluecolor1/255.0);
	
				
				//设置第一个顶点的位置
				zvalue=(float)(bmpoldbits[i+ihalf][j+jhalf]*solid/255.0+position);
		        //画第一个顶点
				glVertex3d((double)i/oldwidth,(double)zvalue,(double)j/oldheight);

				//画第二个顶点
				//对比度调整，设置红色
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbRed-128)*contrast)<0)
					redcolor=0;
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbRed-128)*contrast)>255)
					redcolor=255;
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbRed-128)*contrast)>=0&&(128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbRed-128)*contrast)<=255)
					redcolor=(BYTE)(128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbRed-128)*contrast);
				//亮度调整，设置红色
				if (redcolor*brightness<0)
					redcolor=0;
				if (redcolor*brightness>255)
					redcolor=255;
				if (redcolor*brightness>=0&&redcolor*brightness<=255)
					redcolor=(BYTE)(redcolor*brightness);

				//对比度调整，设置蓝色
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbBlue-128)*contrast)<0)
					bluecolor=0;
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbBlue-128)*contrast)>255)
					bluecolor=255;
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbBlue-128)*contrast)>=0&&(128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbBlue-128)*contrast)<=255)
					bluecolor=(BYTE)(128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbBlue-128)*contrast);
				//亮度调整，设置蓝色
				if (bluecolor*brightness<0)
					bluecolor=0;
				if (bluecolor*brightness>255)
					bluecolor=255;
				if (bluecolor*brightness>=0&&bluecolor*brightness<=255)
					bluecolor=(BYTE)(bluecolor*brightness);

				//对比度调整，设置绿色
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbGreen-128)*contrast)<0)
					greencolor=0;
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbGreen-128)*contrast)>255)
					greencolor=255;
				if ((128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbGreen-128)*contrast)>=0&&(128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbGreen-128)*contrast)<=255)
					greencolor=(BYTE)(128+(bmpoldcolorpalette[bmpoldbits[i+ihalf][j+1+jhalf]].rgbGreen-128)*contrast);
				//亮度调整，设置绿色
				if (greencolor*brightness<0)
					greencolor=0;
				if (greencolor*brightness>255)
					greencolor=255;
				if (greencolor*brightness>=0&&greencolor*brightness<=255)
					greencolor=(BYTE)(greencolor*brightness);
				

//////////////////////// 20140504
                if((redcolor+greencolor+bluecolor)>=384)
				{
				   redcolor1=255.0;
				   greencolor1=(redcolor+greencolor+bluecolor)/3.0;
				   bluecolor1=2*((redcolor+greencolor+bluecolor)/3.0-128)+1;
				}

                if((redcolor+greencolor+bluecolor)<384)
				{
				   redcolor1=2*(redcolor+greencolor+bluecolor)/3.0;
				   greencolor1=(redcolor+greencolor+bluecolor)/3.0;
				   bluecolor1=0;
				}
///////////////////////////////////// 20140504	
				
				//设置第一个顶点的颜色
//    			glColor3d(redcolor/255.0, 0.7*greencolor/255.0, 0.3*bluecolor/255.0);
				glColor3d(redcolor1/255.0, greencolor1/255.0, bluecolor1/255.0);
	
				
				//设置第二个顶点的位置
				zvalue=(float)(bmpoldbits[i+ihalf][j+1+jhalf]*solid/255.0+position);
			    //画第二个顶点
				glVertex3d((double)i/oldwidth,(double)zvalue,(double)(j+1)/oldheight);
			}

			glEnd();
		}
//******************************************************//加图例
if(legendflag==TRUE)
{
        glBegin(GL_LINES);
	    glColor3d(1.0,1.0,0.0); 
		glVertex3d(ihalf/oldwidth,zvalue,1.2*jhalf/oldwidth);
		glVertex3d(ihalf/oldwidth,zvalue,1.2*jhalf/oldwidth+0.1);
	    glEnd();
}
//****************************************************//
       
	glPopMatrix();
	glFinish();
}

void CShow3d::OnRButtonDown(UINT nFlags, CPoint point) 
{

	CMenu *menu, *popup;
	menu = new CMenu();
	// load menu from resource file
	menu->LoadMenu(IDR_POPUP_DIALOG);
	popup = menu->GetSubMenu(0); // item 0 is DUMMY
	
	UINT nEnable;
	//设置对比度调节菜单的能使

	//未加标尺时，去标尺菜单变灰，加标尺菜单使能
	if(axisflag==FALSE)
	{
    nEnable = MF_BYCOMMAND|MF_ENABLED;
	popup->EnableMenuItem(ID_POPUP_ADDAXIS, nEnable);
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_SUBTRACTAXIS, nEnable);
	}

   //加标尺时，加标尺菜单变灰，去标尺菜单使能
    if(axisflag==TRUE)
	{
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_ADDAXIS, nEnable);
    nEnable = MF_BYCOMMAND|MF_ENABLED;
	popup->EnableMenuItem(ID_POPUP_SUBTRACTAXIS, nEnable);
	}

	//当对比度大于等于16.0时，对比度增强菜单变灰
	if(contrast>=(float)(16.0))
	{
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_ADDCONTRAST, nEnable);
	}

	//当对比度小于等于0时，对比度减弱菜单变灰
	if(contrast<=(float)(0.0))
	{
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_SUBTRACTCONTRAST, nEnable);
	}

    //当对比度等于1.0时，对比度复原菜单变灰
	if (contrast==(float)(1.0))
	{
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_RESETCONTRAST, nEnable);
	}
    
	//设置亮度调节菜单的能使
	//当亮度大于等于4.0时，亮度增强菜单变灰
	if(brightness>=(float)(4.0))
	{
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_ADDBRIGHTNESS, nEnable);
	}

	//当亮度小于等于0时，亮度减弱菜单变灰
	if(brightness<=(float)(0.0))
	{
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_SUBTRACTBRIGHTNESS, nEnable);
	}
    //当亮度等于1.0时，亮度复原菜单变灰
	if (brightness==(float)(1.0))
	{
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_RESETBRIGHTNESS, nEnable);
	}

	//设置图像立体度调节菜单的能使
	//当立体度大于等于10.0时，立体度增强菜单变灰
	if(solid>=(float)(10.0))
	{
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_ADDSOLID, nEnable);
	}

	//当立体度小于等于0时，立体度减弱菜单变灰
	if(solid<=(float)(0.0))
	{
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_SUBTRACTSOLID, nEnable);
	}
    //当立体度等于1.0时，立体度复原菜单变灰
	if (solid==(float)(1.0))
	{
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_RESETSOLID, nEnable);
	}

	//设置图像位置调节菜单的能使
	//当位置大于等于0.0时，图像整体上移菜单变灰
	if(position>=(float)0.0)
	{
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_IMAGEUPPER, nEnable);
	}

	//当位置小于等于-solid时，图像整体下移菜单变灰
	if(position<=-solid)
	{
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_IMAGELOWER, nEnable);
	}
    //当位置等于-solid/2.0时，图像位置复原菜单变灰
	if (position==-(float)(solid/2.0))
	{
	nEnable = MF_BYCOMMAND|MF_GRAYED;
	popup->EnableMenuItem(ID_POPUP_IMAGERESET, nEnable);
	}

	//display menu
	ClientToScreen(&point);
	popup->TrackPopupMenu(
	TPM_LEFTALIGN | TPM_RIGHTBUTTON,
	point.x, point.y, this );
	delete menu;

	CDialog::OnRButtonDown(nFlags, point);
}

void CShow3d::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_LeftButtonDown = TRUE;
	m_LeftDownPos = point;
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CShow3d::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_LeftButtonDown = FALSE;
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CShow3d::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_LeftButtonDown)
	{
		m_yRotate += (m_LeftDownPos.x - point.x)/5.0;
		m_xRotate += (m_LeftDownPos.y - point.y)/5.0;
		m_LeftDownPos = point;
		InvalidateRect(NULL,FALSE);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

BOOL CShow3d::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//装载弹出菜单的加速键
	Popup_Accel =LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_POPUP_DIALOG));
	if(Popup_Accel == NULL) 
	{ 
	  MessageBox(_T("三维立体图中各项命令只能用鼠标点击完成"),
				_T("加速键加载失败"), MB_ICONEXCLAMATION | MB_OK);	
	} 

	//把系统设置的颜色赋值给图像的颜色
	CMicroscopeApp* app =(CMicroscopeApp*)AfxGetApp();
	m_FrontColor=app->m_OptionColor;

	//把系统的颜色赋值给调色板
	//得到颜色中的RGB值
	Frontred=GetRValue(m_FrontColor);
	Frontgreen=GetGValue(m_FrontColor);
	Frontblue=GetBValue(m_FrontColor);

	//产生新调色板
	for (i=0;i<=255;i++)
		{
		bmpoldcolorpalette[i].rgbRed=(BYTE)(Frontred+i*(255.0-Frontred)/255.0);
		bmpoldcolorpalette[i].rgbGreen=(BYTE)(Frontgreen+i*(255.0-Frontgreen)/255.0);
		bmpoldcolorpalette[i].rgbBlue=(BYTE)(Frontblue+i*(255.0-Frontblue)/255.0);
		bmpoldcolorpalette[i].rgbReserved=0;
		}


	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
   	LPSTR lpbi;              // pointer to packed-DIB

	//读取原始图像数据
	lpbi = (LPSTR) ::GlobalLock((HGLOBAL) holdDIB);
 
	/* get pointer to BITMAPINFO (Win 3.0) */
    lpbmi = (LPBITMAPINFO)lpbi;

	//得到图像的高度和宽度
    oldwidth=lpbmi->bmiHeader.biWidth;
	oldheight=lpbmi->bmiHeader.biHeight;
    scanrangex=lpbmi->bmiHeader.biSizeImage;
    scanrangey=lpbmi->bmiHeader.biXPelsPerMeter;
	zmin=(float)((lpbmi->bmiHeader.biYPelsPerMeter)/1000.0);
	zmax=(float)((lpbmi->bmiHeader.biClrImportant)/1000.0);
	//计算DIB图像数据宽度实际使用的字节数
	oldfactwidth=WIDTHBYTES(oldwidth*8);

	//动态申请用来临时存储DIB图像数据的数组
	bmpoldbits=new BYTE*[oldfactwidth];
	for (i=0;i<oldfactwidth;i++)
		bmpoldbits[i]=new BYTE[oldheight];

	//复制索引值到二维数组
	for (j=0;j<oldheight;j++)
		for (i=0;i<oldfactwidth;i++)
			::memcpy(&bmpoldbits[i][j],lpbi+1064+i+j*oldfactwidth,1);

	zmax2=bmpoldbits[0][0];
	zmin2=bmpoldbits[0][0];
	for(int i=0;i<oldwidth;i++)
	{
		for(int j=0;j<oldheight;j++)
		{
			if(zmax2<=bmpoldbits[i][j])
				zmax2=bmpoldbits[i][j];
			if(zmin2>=bmpoldbits[i][j])
				zmin2=bmpoldbits[i][j];
		}
	}
	zmax2=(zmax-zmin)*(float)(zmax2/255.0)+zmin;
	zmin2=(zmax-zmin)*(float)(zmin2/255.0)+zmin;

	return TRUE;  
}

void CShow3d::OnPopupBackground() 
{
	//定义一个颜色选择对话框
	CColorDialog dlg(m_BackgroundColor,CC_FULLOPEN);  
 	
	if (dlg.DoModal() != IDOK)
		return;

	m_BackgroundColor = dlg.GetColor();

	this->Invalidate(FALSE);
	
	return;	
}


void CShow3d::OnPopupLine() 
{
	//定义一个颜色选择对话框
	CColorDialog dlg(m_LineColor,CC_FULLOPEN);  
 	
	if (dlg.DoModal() != IDOK)
		return;

	m_LineColor = dlg.GetColor();

	this->Invalidate(FALSE);
	
	return;	
}





void CShow3d::OnPopupFront() 
{
	//定义一个颜色选择对话框
	CColorDialog dlg(m_FrontColor,CC_FULLOPEN);  
 	
	if (dlg.DoModal() != IDOK)
		return;

	m_FrontColor = dlg.GetColor();

	//把系统的颜色赋值给调色板
	//得到颜色中的RGB值
	Frontred=GetRValue(m_FrontColor);
	Frontgreen=GetGValue(m_FrontColor);
	Frontblue=GetBValue(m_FrontColor);

	//产生新调色板
	for (i=0;i<=255;i++)
		{
		bmpoldcolorpalette[i].rgbRed=(BYTE)(Frontred+i*(255.0-Frontred)/255.0);
		bmpoldcolorpalette[i].rgbGreen=(BYTE)(Frontgreen+i*(255.0-Frontgreen)/255.0);
		bmpoldcolorpalette[i].rgbBlue=(BYTE)(Frontblue+i*(255.0-Frontblue)/255.0);
		bmpoldcolorpalette[i].rgbReserved=0;
		}

	this->Invalidate(FALSE);
	
	return;	
	
}



void CShow3d::OnPopupAddcontrast() 
{
	if (contrast<(float)(16.0))
	{
	contrast=(float)(contrast+0.1);
	InvalidateRect(NULL,FALSE);
	}
}

void CShow3d::OnPopupResetcontrast() 
{
	contrast=(float)1.0;
	InvalidateRect(NULL,FALSE);
}

void CShow3d::OnPopupSubtractcontrast() 
{
	if (contrast>(float)(0.0))
	{
	contrast=(float)(contrast-0.1);
	InvalidateRect(NULL,FALSE);
	}
}

void CShow3d::OnPopupAddbrightness() 
{
	if (brightness<(float)(4.0))
	{
	brightness=(float)(brightness+0.1);
	InvalidateRect(NULL,FALSE);	
	}
}

void CShow3d::OnPopupResetbrightness() 
{
	brightness=(float)1.0;
	InvalidateRect(NULL,FALSE);
}

void CShow3d::OnPopupSubtractbrightness() 
{
	if (brightness>(float)(0.0))
	{
	brightness=(float)(brightness-0.1);
	InvalidateRect(NULL,FALSE);
	}
}

void CShow3d::OnPopupAddsolid() 
{
	if (solid<(float)(10.0))
	{
	solid=(float)(solid+0.1);
	position=-(float)(solid/2.0);
	InvalidateRect(NULL,FALSE);	
	}
}

void CShow3d::OnPopupResetsolid() 
{
	solid=(float)1.0;
	position=-(float)(solid/2.0);

	InvalidateRect(NULL,FALSE);
}

void CShow3d::OnPopupSubtractsolid() 
{
	if (solid>(float)(0.0))
	{
	solid=(float)(solid-0.1);
	position=-(float)(solid/2.0);
	InvalidateRect(NULL,FALSE);
	}
	
}

void CShow3d::OnPopupImageupper() 
{
	if (position<(float)(0.0))
	{
	position=(float)(position+solid/10.0);
	InvalidateRect(NULL,FALSE);
	}
}

void CShow3d::OnPopupImagelower() 
{
	if (position>-solid)
	{
	position=(float)(position-solid/10.0);
	InvalidateRect(NULL,FALSE);
	}
}

void CShow3d::OnPopupImagereset() 
{
	position=-(float)(solid/2.0);
	InvalidateRect(NULL,FALSE);
	
}

void CShow3d::OnPopupCaptureimage() 
{
	BeginWaitCursor();

	//捕获用OpenGL在对话框上所画的图像，分两步：
	//1.捕获对话框上用OpenGL所画的图像为一个CBitmap对象，并得到此对象的BITMAP句柄
	//2.把HBITMAP转换成HDIB（DDB to DIB），从而进行显示和以后的保存。由于强制变成24bit的DIB图像，所以不需要加入调色板

	//以下是第一步，得到对话框上图像（即CBitmap对象）的HBITMAP句柄

	CDC memDC;  //内存设备上下文
	CBitmap bitmap; //捕获的BMP图
	CRect	rect;  

	//得到当前活动窗口（即本对话框窗口）的大小
	this->GetActiveWindow()->GetClientRect(rect);
    //为对话框显示设备上下文创建兼容的内存设备上下文
    memDC.CreateCompatibleDC(m_pDC);
	//创建一个与对话框显示设备上下文兼容的位图 
    bitmap.CreateCompatibleBitmap(m_pDC, rect.right,rect.bottom);
    //把新位图选到内存设备上下文中，并返回被替代的原位图的指针
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	//把对话框显示设备描上下文拷贝到内存设备上下文中
    memDC.BitBlt(0, 0, rect.right,rect.bottom, m_pDC, 0, 0, SRCCOPY); 
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

void CShow3d::OnPopupClose() 
{
	CDialog::OnCancel();  	

}

BOOL CShow3d::PreTranslateMessage(MSG* pMsg) 
{
	//响应弹出菜单的加速键
	TranslateAccelerator(this->GetSafeHwnd(),Popup_Accel,pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CShow3d::PrintString(const char *str)
{
    glPushAttrib(GL_LIST_BIT);
    glListBase(m_nFontOffset);
    glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte*)str);
    glPopAttrib();
}


void CShow3d::OnPopupAddaxis() 
{
	// TODO: Add your command handler code here
	axisflag=TRUE;
	InvalidateRect(NULL, FALSE);
}

void CShow3d::OnPopupSubtractaxis() 
{
	// TODO: Add your command handler code here
	axisflag=FALSE;
	InvalidateRect(NULL, FALSE);
}

void CShow3d::OnPopupAddlegend() 
{
	// TODO: Add your command handler code here
	legendflag=TRUE;
    InvalidateRect(NULL, FALSE);
}

void CShow3d::OnPopupDeletelegend() 
{
	// TODO: Add your command handler code here
	legendflag=FALSE;
	InvalidateRect(NULL, FALSE);
}

void CShow3d::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//	设置视口大小
	glViewport(0,0,cx,cy);
	//	设置变换模式为投影变换
	glMatrixMode(GL_PROJECTION);

	//	设置变换模式为模型变换
	glMatrixMode(GL_MODELVIEW);
	
}

void CShow3d::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if(xrotateflag==TRUE)//X向旋转
	{
	m_xRotate -= 2;
	InvalidateRect(NULL,FALSE);
	}
   
	if(yrotateflag==TRUE) //y向旋转
	{
	m_yRotate -= 3;
	InvalidateRect(NULL,FALSE);
	}
    
	if(randomrotateflag==TRUE)//任意向旋转
	{
    m_xRotate -= 2;
	m_yRotate -= 3;
	InvalidateRect(NULL,FALSE);
	}
    
	if(stoprotateflag==TRUE)//停止旋转
	{
    m_xRotate =m_xRotate;
	m_yRotate =m_yRotate;
	InvalidateRect(NULL,FALSE);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CShow3d::OnPopupXrotate() 
{
	// TODO: Add your command handler code here
	xrotateflag=TRUE;
}

void CShow3d::OnPopupYrotate() 
{
	// TODO: Add your command handler code here
	yrotateflag=TRUE;	
}


void CShow3d::OnPopupRandomrotate() 
{
	// TODO: Add your command handler code here
	randomrotateflag=TRUE;
}

void CShow3d::OnPopupStoptotate() 
{
	// TODO: Add your command handler code here
	stoprotateflag=TRUE;
	InvalidateRect(NULL,FALSE);
}

void CShow3d::RasterFont()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    m_nFontOffset = glGenLists(128);
    for (GLuint i = 0; i < 127; i++)
	{
		glNewList(i + m_nFontOffset, GL_COMPILE);
		glBitmap(8, 13, 0.0f, 2.0f, 10.0f, 0.0f, m_RasterFont[i-32]);
		glEndList();
    }
}

void CShow3d::drawstring(unsigned char* str)
{
 GLYPHMETRICSFLOAT pgmf[1];
 HDC hDC=wglGetCurrentDC();
 DWORD dwChar;
 int ListNum;
 for(size_t i=0;i<strlen((char *)str);i++)
   {
     if(IsDBCSLeadByte(str[i]))
       {dwChar=(DWORD)((str[i]<<8)|str[i+1]);i++;}
     else dwChar=str[i];
     ListNum=glGenLists(1);
     wglUseFontOutlines(hDC,dwChar,1,ListNum,0.0,0.1,WGL_FONT_LINES,pgmf);
     glCallList(ListNum);
     glDeleteLists(ListNum,1);
   }
}  


int CShow3d::takeinteger(double z)
{
 int i=0;
 while(floor(z)!=0)
 {
   i++;
   z=z/10;
 }
 return i;
}
