// dspDoc.cpp : implementation of the CDspDoc class
//

#include "stdafx.h"
#include "dsp.h"

#include "dspDoc.h"
#include "Dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDspDoc

IMPLEMENT_DYNCREATE(CDspDoc, CDocument)

BEGIN_MESSAGE_MAP(CDspDoc, CDocument)
	//{{AFX_MSG_MAP(CDspDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDspDoc construction/destruction

CDspDoc::CDspDoc()
{
	// TODO: add one-time construction code here
	m_pDib=NULL;

}

CDspDoc::~CDspDoc()
{
	if (m_pDib!=NULL)
	{
		delete m_pDib;
		m_pDib=NULL;
	}
}

BOOL CDspDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

BOOL CDspDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	
	if (m_pDib!=NULL)
	{
		delete m_pDib;
		m_pDib=NULL;
	}
	m_pDib= new CDib;
	if (m_pDib!=NULL)
	{
		m_pDib->LoadFile(lpszPathName);

	//	m_pDib->chuang();

		UpdateAllViews(NULL);
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CDspDoc serialization

void CDspDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDspDoc diagnostics

#ifdef _DEBUG
void CDspDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDspDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDspDoc commands
