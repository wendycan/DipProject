#if !defined(AFX_DLGHISTOGRAM_H__43B3AA71_4B17_448A_A8F7_4E3B5B079892__INCLUDED_)
#define AFX_DLGHISTOGRAM_H__43B3AA71_4B17_448A_A8F7_4E3B5B079892__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHistogram.h : header file
//
class CDib;
/////////////////////////////////////////////////////////////////////////////
// CDlgHistogram dialog

class CDlgHistogram : public CDialog
{
// Construction
public:
	CDlgHistogram(CDib* pDib,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgHigstogram)
	enum { IDD = IDD_DLG_HISTOGRAM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHistogram)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHistogram)
	afx_msg void OnPaint();
//	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	long m_nGreyCount;
	long *m_pGreyScale;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHISTOGRAM_H__43B3AA71_4B17_448A_A8F7_4E3B5B079892__INCLUDED_)
