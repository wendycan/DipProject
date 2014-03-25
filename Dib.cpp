// Dib.cpp: implementation of the CDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dsp.h"
#include <math.h>
#include "Dib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDib::CDib()
{
	m_pDib=NULL;
	m_pDibBits=NULL;
    pRawData=NULL;

}

CDib::~CDib()
{	if (m_pDib!=NULL)
	{	
		delete[] m_pDib;
		m_pDib=NULL;
		m_pDibBits=NULL;
	}
	if (pRawData!=NULL)
	{
        delete[] pRawData;
        pRawData=NULL;
	}
}
void CDib::LoadFile( const char* pFileName)
{
	CFile file;
	if (!file.Open(pFileName, CFile::modeRead|CFile::shareDenyRead,NULL))
	{
		AfxMessageBox("file not be opened!",MB_OK);
		return;
	}
	BITMAPFILEHEADER BFH;
	file.Read(&BFH, sizeof(BITMAPFILEHEADER));
	if (BFH.bfType!='MB')//判断文件头的前两位是否为MB?
	{
		AfxMessageBox("not a bitmap file!",MB_OK);
		return;

	}
	m_nDibSize=file.GetLength()-sizeof(BITMAPFILEHEADER);//前者为总长度，后者为文件头的长度
    m_pDib = new unsigned char[m_nDibSize];
    memset(m_pDib,0,m_nDibSize);//所有值均赋0
	file.Read(m_pDib,m_nDibSize);
	m_pBitmapInfoHeader = (BITMAPINFOHEADER*)m_pDib;
	m_nWidth = m_pBitmapInfoHeader->biWidth;
	m_nHeight = m_pBitmapInfoHeader->biHeight;
	m_length = m_nWidth * m_nHeight;
	m_pPaletteEntry = (PALETTEENTRY *)(m_pDib+sizeof(BITMAPINFOHEADER));
	if (m_pBitmapInfoHeader->biBitCount>8)
	{
		m_nPaletteEntries = 0;
		m_length *= (m_pBitmapInfoHeader->biBitCount)/8;

	}else
	{
	m_nPaletteEntries = 1<<m_pBitmapInfoHeader->biBitCount;

	}
	m_pDibBits = m_pDib + sizeof(BITMAPINFOHEADER) + m_nPaletteEntries*sizeof(RGBQUAD);
}


void CDib::Draw( CDC * pDC)
{
	if (m_pDib!=NULL)
	{
		
		::StretchDIBits(pDC->m_hDC,0,0,m_nWidth,m_nHeight,0,0,m_nWidth,m_nHeight,m_pDibBits,(BITMAPINFO*)m_pBitmapInfoHeader,DIB_RGB_COLORS,SRCCOPY);
	}
}

void CDib::Inverse()
{
	if(m_nDibSize == NULL )
	{
	AfxMessageBox("No object is opened",MB_OK);
    return;   	
	}
	for(int i=0;i<m_length;i++){
		m_pDibBits[i] = 255 - m_pDibBits[i];// OR use point: *(m_pDibBits+i)=255-*(m_pDibBits+i);
	}
}
