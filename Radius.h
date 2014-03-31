#if !defined(AFX_RADIUS_H__396CD147_1544_4C2F_A1B0_60033F907160__INCLUDED_)
#define AFX_RADIUS_H__396CD147_1544_4C2F_A1B0_60033F907160__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Radius.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRadius dialog

class CRadius : public CDialog
{
// Construction
public:
	CRadius(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRadius)
	enum { IDD = IDD_DIALOG1 };
	int		m_radius;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRadius)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRadius)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RADIUS_H__396CD147_1544_4C2F_A1B0_60033F907160__INCLUDED_)
