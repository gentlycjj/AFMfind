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
	CRectTrackerEx RectTracker;  //定义一个橡皮区矩形
	BOOL DrawRectTrackerflag;  //是否画橡皮区矩形的标志    
	virtual ~CMicroscopeDoc();
protected:
	HDIB m_hDIB;
	CPalette* m_palDIB;
	CSize m_sizeDoc;
	int year;  //测量时间，年月日，
	int month;
	int day;
	CString measuretimehms;//测量时间，小时，分钟，秒
	BOOL writetxtflag;    //是否写txt信息文件的标示
	int  bmpsize;		//图像大小，小图为0，大图为1
	int  scanrangex;	//扫描范围的x值
	int  scanrangey;	//扫描范围的y值
	int  scantime;		//扫描时间
	float zaverage;	//平均z值
	float zmax;		//最大z值
	float zmin;		//最小z值
	float Ra;      //Ra值
	float Ry;      //Ry值
	float Rz;      //Rz值
	int lthreshold; //图像亮度阈值
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
