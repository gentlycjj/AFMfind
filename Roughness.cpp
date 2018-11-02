// Roughness.cpp : implementation file
//

#include "stdafx.h"
#include "microscope.h"
#include "Roughness.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRoughness dialog


CRoughness::CRoughness(CWnd* pParent /*=NULL*/)
	: CDialog(CRoughness::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRoughness)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRoughness::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRoughness)
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_Rz, m_Rz);
	DDX_Control(pDX, IDC_Ry, m_Ry);
	DDX_Control(pDX, IDC_Rangey, m_scanrangey);
	DDX_Control(pDX, IDC_Rangex, m_scanrangex);
	DDX_Control(pDX, IDC_Ra, m_Ra);
	DDX_Control(pDX, IDC_Imagesizey, m_imagesizey);
	DDX_Control(pDX, IDC_Imagesizex, m_imagesizex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRoughness, CDialog)
	//{{AFX_MSG_MAP(CRoughness)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoughness message handlers

void CRoughness::OnOK() 
{
	// TODO: Add extra validation here
//	CDialog::OnOK();  //重载OnOk()函数，将其禁止，加入自己的代码

	CString txtfilepathname; //保存的txt文件的名字，带有路径

	//打开保存文件对话框
	CFileDialog dlg(FALSE, "*.txt","*.txt",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"文本文档(*.txt)|*.txt|所有文件(*.*)|*.*",NULL);

	if (dlg.DoModal()==IDOK)
		{
			txtfilepathname = dlg.GetPathName(); //得到保存的txt文件的名字及其路径
			if (txtfilepathname.IsEmpty())
			return;
		}

	CStdioFile filetxt;   
	CFileException fe;
	CString txtfilename;  //保存的txt文件的名字，不带有路径

	if (!filetxt.Open(txtfilepathname, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive | CFile::typeText, &fe))
		{
		return ;
		}

	char temp[100];
	int txtnameindex;  //txt文件名在整个带路径的文件名字符串中的位置
	txtnameindex=txtfilepathname.GetLength()-txtfilepathname.ReverseFind('\\')-1;
	txtfilename=txtfilepathname.Right(txtnameindex); //得到txt文件名

	//写txt文件名
	sprintf(temp,"文本文档:  ");
	CString line=temp;
	filetxt.Write(line,line.GetLength());
	filetxt.Write(txtfilename,txtfilename.GetLength());
	sprintf(temp,"\n\n");
	line=temp;
	filetxt.Write(line,line.GetLength());
	//写粗糙度Ra,Ry和Rz
	sprintf(temp,"粗糙度     Ra: %.1f nm ;  Ry: %.1f nm ;  Rz: %.1f nm\n\n",Ra,Ry,Rz);
	line=temp;
	filetxt.Write(line,line.GetLength());	
	//写扫描范围
	sprintf(temp,"扫描范围   X: %d nm ; Y: %d nm\n\n",scanrangex,scanrangey);
	line=temp;
	filetxt.Write(line,line.GetLength());
	//写图像大小
	sprintf(temp,"图像大小   X: %d pixel ; Y: %d pixel\n\n",imagesizex,imagesizey);
	line=temp;
	filetxt.Write(line,line.GetLength());

	filetxt.Close();
        
	return;
}

void CRoughness::OnCancel() 
{
	// TODO: Add extra cleanup here
     CDialog::OnCancel(); //重载OnCancel()函数
}

BOOL CRoughness::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//显示粗糙度Ra, Ry和Rz
	sprintf(timechar,"%.1f",Ra);
	m_Ra.SetWindowText(timechar);
	sprintf(timechar,"%.1f",Ry);
	m_Ry.SetWindowText(timechar);
	sprintf(timechar,"%.1f",Rz);
	m_Rz.SetWindowText(timechar);

	//显示扫描范围
	sprintf(timechar,"%d ", scanrangex);
	m_scanrangex.SetWindowText(timechar);
	sprintf(timechar,"%d ", scanrangey);
	m_scanrangey.SetWindowText(timechar);

	//显示图像大小
	sprintf(timechar, "%d ", imagesizex);
	m_imagesizex.SetWindowText(timechar);
	sprintf(timechar, "%d ", imagesizey);
	m_imagesizey.SetWindowText(timechar);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
