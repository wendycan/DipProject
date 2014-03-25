// dspView.h : interface of the CDspView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSPVIEW_H__A325A7DE_D66D_4FC4_92DC_BA75C5B9A80C__INCLUDED_)
#define AFX_DSPVIEW_H__A325A7DE_D66D_4FC4_92DC_BA75C5B9A80C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDspView : public CView
{
protected: // create from serialization only
	CDspView();
	DECLARE_DYNCREATE(CDspView)

// Attributes
public:
	CDspDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDspView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDspView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDspView)
	afx_msg void OnFanbai();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in dspView.cpp
inline CDspDoc* CDspView::GetDocument()
   { return (CDspDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSPVIEW_H__A325A7DE_D66D_4FC4_92DC_BA75C5B9A80C__INCLUDED_)
