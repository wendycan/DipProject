// Radius.cpp : implementation file
//

#include "stdafx.h"
#include "dsp.h"
#include "Radius.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRadius dialog


CRadius::CRadius(CWnd* pParent /*=NULL*/)
	: CDialog(CRadius::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRadius)
	m_radius = 0;
	//}}AFX_DATA_INIT
}


void CRadius::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRadius)
	DDX_Text(pDX, IDC_EDIT1, m_radius);
	DDV_MinMaxInt(pDX, m_radius, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRadius, CDialog)
	//{{AFX_MSG_MAP(CRadius)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRadius message handlers
