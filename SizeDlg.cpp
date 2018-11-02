// SizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "microscope.h"
#include "SizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSizeDlg dialog


CSizeDlg::CSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSizeDlg)
	m_iSmall = 1;
	m_dwX = 4000;
	m_dwY = 4000;
	//}}AFX_DATA_INIT
}


void CSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSizeDlg)
	DDX_Control(pDX, IDC_SMALL, m_smallBut);
	DDX_Control(pDX, IDC_SPINY, m_spiny);
	DDX_Control(pDX, IDC_SPINX, m_spinx);
	DDX_Text(pDX, IDC_SCANX, m_dwX);
	DDX_Text(pDX, IDC_SCANY, m_dwY);
	DDX_Radio(pDX, IDC_SMALL, m_iSmall);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSizeDlg, CDialog)
	//{{AFX_MSG_MAP(CSizeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSizeDlg message handlers

BOOL CSizeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_spinx.SetRange(40,4000);
	m_spiny.SetRange(40,4000);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
