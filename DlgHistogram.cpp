// DlgHistogram.cpp : implementation file
//

#include "stdafx.h"
#include "Dib.h"
#include "dsp.h"
/*#include "Test.h"*/
#include "DlgHistogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHistogram dialog


CDlgHistogram::CDlgHistogram(CDib* pDib,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHistogram::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHistogram)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	if (pDib->m_pGrayScale == NULL)
	{
		pDib->GrayScale();
	}
	m_pGreyScale = pDib->m_pGrayScale;
	m_nGreyCount = pDib->m_nPaletteEntries;
// 	if ((m_nGreyCount == 0)||(m_pGreyScale != NULL))
// 	{
// 		m_nGreyCount = 255;
// 	}

}


void CDlgHistogram::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHistogram)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHistogram, CDialog)
	//{{AFX_MSG_MAP(CDlgHistogram)
	ON_WM_PAINT()
//	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHistogram message handlers

void CDlgHistogram::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if (m_pGreyScale == NULL)
	{
		return;
	}
	CRect rect;
	
	CWnd *pWnd = GetDlgItem(IDC_STATIC_HISTOGRAM);
// 	this->GetWindowRect(&rect);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	CDC *pDC = &dc;


 	pWnd->UpdateWindow();	
// 	this->UpdateWindow();
	pDC->Rectangle(rect);
	

	CPen *pRedPen = new CPen;
	pRedPen->CreatePen(PS_SOLID,2,RGB(255,0,0));//画坐标轴的笔，红色
	CPen *pOldPen = pDC->SelectObject(pRedPen);//选择新画笔，返回并保存原值
	
	CPen *pBluePen = new CPen;
	pBluePen->CreatePen(PS_SOLID,2,RGB(0,0,255));
	
	rect.DeflateRect(20, 20);
	
	/*	pDC->Rectangle(&rect);*/
	
	pDC->MoveTo(rect.left,rect.bottom);//从当前点移动到参数点
	pDC->LineTo(rect.left,rect.top);//画纵坐标
	pDC->LineTo(rect.left-5,rect.top + 5);//画箭头
	pDC->MoveTo(rect.left,rect.top);
	pDC->LineTo(rect.left+5,rect.top + 5);
	
	pDC->MoveTo(rect.left,rect.bottom);//画横坐标
	pDC->LineTo(rect.right,rect.bottom);
	pDC->LineTo(rect.right-5,rect.bottom - 5);
	pDC->MoveTo(rect.right,rect.bottom);
	pDC->LineTo(rect.right-5,rect.bottom + 5);
	
	
	float DeltaCount = float(rect.Width()-10)/m_nGreyCount;
	
	pDC->SelectObject(pBluePen);//选择蓝色画笔，开始画直方图
	int GreyNumberMax = 0;
	
	for (int i=0; i< m_nGreyCount; i++)
	{
		if (GreyNumberMax < m_pGreyScale[i])
		{
			GreyNumberMax = m_pGreyScale[i];//直方图中灰度次数最多的值
		}
	}
	for (i=0; i< m_nGreyCount; i++)
	{
		if (GreyNumberMax != 0)
		{
			double Ratio = (( double )m_pGreyScale[i])/GreyNumberMax;
			pDC->MoveTo(rect.left +(int)(i* DeltaCount)+2, rect.bottom - 2);
			pDC->LineTo(rect.left +(int)(i* DeltaCount)+2, rect.bottom - 2 - (int)(Ratio * (rect.Height()-10)));
		}
	}
	for (i=0; i< m_nGreyCount; i++)
	{
		if (i % 10 == 0)
		{
			pDC->MoveTo(rect.left +(int)(i* DeltaCount)+2, rect.bottom - 2);
			pDC->LineTo(rect.left +(int)(i* DeltaCount)+2, rect.bottom - 2 + 4);
		}
		if (i % 50 == 0)
		{
			pDC->MoveTo(rect.left +(int)(i* DeltaCount)+2, rect.bottom - 2);
			pDC->LineTo(rect.left +(int)(i* DeltaCount)+2, rect.bottom - 2 + 10);
		}
	}
	
	
	pDC->SelectObject(pOldPen);

	delete pRedPen;
	delete pBluePen;
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgHistogram::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
