// dspView.cpp : implementation of the CDspView class
//

#include "stdafx.h"
#include "dsp.h"

#include "dspDoc.h"
#include "dspView.h"
#include "Dib.h"
#include "Radius.h"
#include "DlgHistogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDspView

IMPLEMENT_DYNCREATE(CDspView, CView)

BEGIN_MESSAGE_MAP(CDspView, CView)
	//{{AFX_MSG_MAP(CDspView)
	ON_COMMAND(ID_FANBAI, OnFanbai)
	ON_COMMAND(ID_REMOVERED, OnRemovered)
	ON_COMMAND(ID_MENU_Histogram, OnMENUHistogram)
	ON_COMMAND(ID_HISTOGRAM_EQU, OnHistogramEqu)
	ON_COMMAND(ID_SMOOTH, OnSmooth)
	ON_COMMAND(ID_LAPLACEENHANCED, OnLaplaceEnhanced)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_NEW_RECTANGLE, OnNewRectangle)
	ON_COMMAND(ID_NEW_CIRCLE, OnNewCircle)
	ON_COMMAND(ID_FFT, OnFft)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDspView construction/destruction

CDspView::CDspView()
{
	// TODO: add construction code here

}

CDspView::~CDspView()
{
}

BOOL CDspView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDspView drawing

void CDspView::OnDraw(CDC* pDC)
{
	CDspDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if (pDoc->m_pDib!=NULL)
	{
		pDoc->m_pDib->Draw(pDC);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDspView printing

BOOL CDspView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDspView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDspView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDspView diagnostics

#ifdef _DEBUG
void CDspView::AssertValid() const
{
	CView::AssertValid();
}

void CDspView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDspDoc* CDspView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDspDoc)));
	return (CDspDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDspView message handlers

void CDspView::OnFanbai() 
{
	// TODO: Add your command handler code here
	CDspDoc * pDoc = GetDocument();
	if(pDoc->m_pDib != NULL){
		pDoc->m_pDib->Inverse();
		pDoc->UpdateAllViews(NULL);
	}
	
}

void CDspView::OnRemovered() 
{
	// TODO: Add your command handler code here
	CDspDoc * pDoc = GetDocument();
	if (pDoc->m_pDib != NULL)
	{
		pDoc->m_pDib->RemoveRed();
		pDoc->UpdateAllViews(NULL);
	}
	
}

void CDspView::OnMENUHistogram() 
{
	// TODO: Add your command handler code here
	CDspDoc* pDoc = GetDocument();
	if(pDoc->m_pDib != NULL){
		CDlgHistogram DlgHist(pDoc->m_pDib);
		DlgHist.DoModal();
	}
	
}

void CDspView::OnHistogramEqu() 
{
	// TODO: Add your command handler code here
	CDspDoc* pDoc = GetDocument();
	if	(pDoc->m_pDib->m_pDibData==NULL)
	{
		AfxMessageBox("No picture",MB_OK);
		return;
	}
	pDoc->m_pDib->HistogramEqu();	
	pDoc->UpdateAllViews(NULL);
}

void CDspView::OnSmooth() 
{
	// TODO: Add your command handler code here
	CDspDoc* pDoc = GetDocument();
	if	(pDoc->m_pDib->m_pDibData==NULL)
	{
		AfxMessageBox("No picture",MB_OK);
		return;
	}
	pDoc->m_pDib->Smooth();	
	pDoc->UpdateAllViews(NULL);
}

void CDspView::OnLaplaceEnhanced() 
{
	// TODO: Add your command handler code here
	CDspDoc* pDoc = GetDocument();
	if	(pDoc->m_pDib->m_pDibData==NULL)
	{
		AfxMessageBox("No picture",MB_OK);
		return;
	}
	pDoc->m_pDib->LaplaceEnhanced();	
	pDoc->UpdateAllViews(NULL);
}

void CDspView::OnFileNew() 
{
	// TODO: Add your command handler code here
	CDspDoc* pDoc = GetDocument();
	if (pDoc->m_pDib!=NULL)
	{
		delete pDoc->m_pDib;
		pDoc->m_pDib=NULL;
	}
	pDoc->m_pDib= new CDib;
	if (pDoc->m_pDib!=NULL){
		pDoc->m_pDib->CreateDib();
		pDoc->UpdateAllViews(NULL);
	}
}

void CDspView::OnNewRectangle() 
{
	// TODO: Add your command handler code here
	CDspDoc* pDoc = GetDocument();
	pDoc->m_pDib->DrawRectangle();
	pDoc->UpdateAllViews(NULL);
}

void CDspView::OnNewCircle() 
{
	// TODO: Add your command handler code here
	CDspDoc* pDoc = GetDocument();
	CRadius Radius;
	Radius.DoModal();
	pDoc->m_pDib->DrawCircle(Radius.m_radius);
	pDoc->UpdateAllViews(NULL);
}


void CDspView::OnFft() 
{
	// TODO: Add your command handler code here
	CDspDoc* pDoc = GetDocument();
	CRadius Radius;
	Radius.DoModal();
	pDoc->m_pDib->FFT(pDoc->m_pDib->m_pDibBits, pDoc->m_pDib->m_nWidth, pDoc->m_pDib->m_nHeight, Radius.m_radius);
}
