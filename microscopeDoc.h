// microscopeDoc.h : interface of the CMicroscopeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICROSCOPEDOC_H__14722B0C_A28A_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
#define AFX_MICROSCOPEDOC_H__14722B0C_A28A_11D4_9E5C_0080C8DB8CF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dibapi.h"
#include "RectTrackerEx.h"

class CMicroscopeDoc : public CDocument
{
protected: // create from serialization only
	CMicroscopeDoc();
	DECLARE_DYNCREATE(CMicroscopeDoc)

// Attributes
public:
	HDIB GetHDIB() const
		{ return m_hDIB; }
	CPalette* GetDocPalette() const
		{ return m_palDIB; }
	CSize GetDocSize() const
		{ return m_sizeDoc; }
	void ReplaceData(int syear,int smonth,int sday,CString shms,int ssize,int stime,int sx,int sy,float za,float zma,float zmi,int it,float ra1,float ry1,float rz1);
	void ReplaceFlag(BOOL flag);

// Operations
public:
	void ReplaceHDIB(HDIB hDIB);
	void InitDIBData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMicroscopeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	UINT m_nFromClip;
	//CString m_strOrgPath;
	//BOOL m_bFromClipboard;
	CRectTrackerEx RectTracker;  //����һ����Ƥ������
	BOOL DrawRectTrackerflag;  //�Ƿ���Ƥ�����εı�־    
	virtual ~CMicroscopeDoc();
protected:
	HDIB m_hDIB;
	CPalette* m_palDIB;
	CSize m_sizeDoc;
	int year;  //����ʱ�䣬�����գ�
	int month;
	int day;
	CString measuretimehms;//����ʱ�䣬Сʱ�����ӣ���
	BOOL writetxtflag;    //�Ƿ�дtxt��Ϣ�ļ��ı�ʾ
	int  bmpsize;		//ͼ���С��СͼΪ0����ͼΪ1
	int  scanrangex;	//ɨ�跶Χ��xֵ
	int  scanrangey;	//ɨ�跶Χ��yֵ
	int  scantime;		//ɨ��ʱ��
	float zaverage;	//ƽ��zֵ
	float zmax;		//���zֵ
	float zmin;		//��Сzֵ
	float Ra;      //Raֵ
	float Ry;      //Ryֵ
	float Rz;      //Rzֵ
	int lthreshold; //ͼ��������ֵ
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMicroscopeDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MICROSCOPEDOC_H__14722B0C_A28A_11D4_9E5C_0080C8DB8CF1__INCLUDED_)
