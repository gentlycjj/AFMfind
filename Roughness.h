#if !defined(AFX_ROUGHNESS_H__9B2A3FA0_70E4_11D6_862F_0050BABC4913__INCLUDED_)
#define AFX_ROUGHNESS_H__9B2A3FA0_70E4_11D6_862F_0050BABC4913__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Roughness.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRoughness dialog

class CRoughness : public CDialog
{
// Construction
public:
	int imagesizex; //ͼ��X���������
	int imagesizey; //ͼ��Y���������
	int scanrangex;     //ͼ��X����ɨ�跶Χ
	int scanrangey;    //ͼ��Y����ɨ�跶Χ
	float Ra;            //�ֲڶ�Ra
	float Ry;            //�ֲڶ�Ry
	float Rz;            //�ֲڶ�Rz
	char timechar[25];  //��ʱ�ַ�����

	CRoughness(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRoughness)
	enum { IDD = IDD_ROUGHNESS };
	CButton	m_cancel;
	CButton	m_ok;
	CStatic	m_Rz;
	CStatic	m_Ry;
	CStatic	m_scanrangey;
	CStatic	m_scanrangex;
	CStatic	m_Ra;
	CStatic	m_imagesizey;
	CStatic	m_imagesizex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoughness)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRoughness)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROUGHNESS_H__9B2A3FA0_70E4_11D6_862F_0050BABC4913__INCLUDED_)
