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
//	CDialog::OnOK();  //����OnOk()�����������ֹ�������Լ��Ĵ���

	CString txtfilepathname; //�����txt�ļ������֣�����·��

	//�򿪱����ļ��Ի���
	CFileDialog dlg(FALSE, "*.txt","*.txt",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"�ı��ĵ�(*.txt)|*.txt|�����ļ�(*.*)|*.*",NULL);

	if (dlg.DoModal()==IDOK)
		{
			txtfilepathname = dlg.GetPathName(); //�õ������txt�ļ������ּ���·��
			if (txtfilepathname.IsEmpty())
			return;
		}

	CStdioFile filetxt;   
	CFileException fe;
	CString txtfilename;  //�����txt�ļ������֣�������·��

	if (!filetxt.Open(txtfilepathname, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive | CFile::typeText, &fe))
		{
		return ;
		}

	char temp[100];
	int txtnameindex;  //txt�ļ�����������·�����ļ����ַ����е�λ��
	txtnameindex=txtfilepathname.GetLength()-txtfilepathname.ReverseFind('\\')-1;
	txtfilename=txtfilepathname.Right(txtnameindex); //�õ�txt�ļ���

	//дtxt�ļ���
	sprintf(temp,"�ı��ĵ�:  ");
	CString line=temp;
	filetxt.Write(line,line.GetLength());
	filetxt.Write(txtfilename,txtfilename.GetLength());
	sprintf(temp,"\n\n");
	line=temp;
	filetxt.Write(line,line.GetLength());
	//д�ֲڶ�Ra,Ry��Rz
	sprintf(temp,"�ֲڶ�     Ra: %.1f nm ;  Ry: %.1f nm ;  Rz: %.1f nm\n\n",Ra,Ry,Rz);
	line=temp;
	filetxt.Write(line,line.GetLength());	
	//дɨ�跶Χ
	sprintf(temp,"ɨ�跶Χ   X: %d nm ; Y: %d nm\n\n",scanrangex,scanrangey);
	line=temp;
	filetxt.Write(line,line.GetLength());
	//дͼ���С
	sprintf(temp,"ͼ���С   X: %d pixel ; Y: %d pixel\n\n",imagesizex,imagesizey);
	line=temp;
	filetxt.Write(line,line.GetLength());

	filetxt.Close();
        
	return;
}

void CRoughness::OnCancel() 
{
	// TODO: Add extra cleanup here
     CDialog::OnCancel(); //����OnCancel()����
}

BOOL CRoughness::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//��ʾ�ֲڶ�Ra, Ry��Rz
	sprintf(timechar,"%.1f",Ra);
	m_Ra.SetWindowText(timechar);
	sprintf(timechar,"%.1f",Ry);
	m_Ry.SetWindowText(timechar);
	sprintf(timechar,"%.1f",Rz);
	m_Rz.SetWindowText(timechar);

	//��ʾɨ�跶Χ
	sprintf(timechar,"%d ", scanrangex);
	m_scanrangex.SetWindowText(timechar);
	sprintf(timechar,"%d ", scanrangey);
	m_scanrangey.SetWindowText(timechar);

	//��ʾͼ���С
	sprintf(timechar, "%d ", imagesizex);
	m_imagesizex.SetWindowText(timechar);
	sprintf(timechar, "%d ", imagesizey);
	m_imagesizey.SetWindowText(timechar);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
