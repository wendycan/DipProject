// dspDoc.h : interface of the CDspDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSPDOC_H__0A749F66_C225_489B_949C_8E987D7E400F__INCLUDED_)
#define AFX_DSPDOC_H__0A749F66_C225_489B_949C_8E987D7E400F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDib;

class CDspDoc : public CDocument
{
protected: // create from serialization only
	CDspDoc();
	DECLARE_DYNCREATE(CDspDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDspDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDspDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDspDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CDib * m_pDib;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSPDOC_H__0A749F66_C225_489B_949C_8E987D7E400F__INCLUDED_)
