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
	m_pGrayScale = NULL;

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
	if (m_pGrayScale != NULL)
	{
	 delete []m_pGrayScale;
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
	long hi = sizeof(BITMAPINFOHEADER);
	m_pPaletteEntry = (PALETTEENTRY *)(m_pDib+sizeof(BITMAPINFOHEADER));
	if (m_pBitmapInfoHeader->biBitCount>8)
	{
		m_nPaletteEntries = 0;
		m_length *= (m_pBitmapInfoHeader->biBitCount)/8;  //To calculate bmp size by multiply by bitcount
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
	for(int i=0;i<m_nPaletteEntries;i++){  //not m_nDibSize
		m_pPaletteEntry[i].peBlue=255-m_pPaletteEntry[i].peBlue;
		m_pPaletteEntry[i].peGreen=255-m_pPaletteEntry[i].peGreen;
		m_pPaletteEntry[i].peRed=255-m_pPaletteEntry[i].peRed;
	//	m_pDibBits[i] = 255 - m_pDibBits[i];// OR use point: *(m_pDibBits+i)=255-*(m_pDibBits+i);
	}
}

void CDib::RemoveRed()
{
	if(m_nDibSize == NULL){
		AfxMessageBox("No object is opened",MB_OK);
		return; 
	}
	for(int i=0;i<m_nPaletteEntries;i++){
		m_pPaletteEntry[i].peBlue=0;
	}
}

void CDib::GrayScale()
{
	if(m_pGrayScale != NULL){
		delete []m_pGrayScale;
		m_pGrayScale = NULL;
	}
	m_pGrayScale = new long [m_nPaletteEntries];
	memset(m_pGrayScale,0,m_nPaletteEntries*sizeof(long));
	for (int i=0;i<m_nHeight;i++)
	{
		for(int j=0;j<m_nWidth;j++)
		{
			m_pGrayScale[*(m_pDibBits+i*m_nWidth+j)]++;
		}
	}
}

void CDib::HistogramEqu()
{
	GrayScale();
	FLOAT L=256;
	long *s= new long[L];			
	for (int i=0;i<L;i++)
	{
		s[i]=0;
	}
	FLOAT s_p=0;
	for (i=0;i<L;i++)
	{		
		s_p=s_p+m_pGrayScale[i];
		s[i]=(L-1)/(m_nHeight*m_nWidth)*s_p;
	}

	for (i=0;i<L;i++)
	{		
		m_pPaletteEntry[i].peBlue=(int)s[i];
		m_pPaletteEntry[i].peRed=(int)s[i];
		m_pPaletteEntry[i].peGreen=(int)s[i];
	}
	if (s!=NULL)
	{
		delete []s;
		s=NULL;
	}

}

void CDib::Smooth()
{
	FLOAT *s= new FLOAT[m_nDibSize];
		for (int i=0;i<m_nDibSize;i++)
	{
		s[i]=0;
	}
	for ( i=1;i<m_nHeight-1;i++)
	{		
		int  i_1=i-1, i_2=i+1;
		for (int j=1;j<m_nWidth-1;j++){
			int j_1=j-1, j_2=j+1;
			s[i*m_nWidth+j]=(m_pDibBits[i_1*m_nWidth+j_1]+m_pDibBits[i_1*m_nWidth+j]+m_pDibBits[i_1*m_nWidth+j_2]
				+m_pDibBits[i*m_nWidth+j_1]+m_pDibBits[i*m_nWidth+j]+m_pDibBits[i*m_nWidth+j_2]+
				m_pDibBits[i_2*m_nWidth+j_1]+m_pDibBits[i_2*m_nWidth+j]+m_pDibBits[i_2*m_nWidth+j_2])/9.0;

		}
	}
	for ( i=1;i<m_nHeight-1;i++)
	{		
		for (int j=1;j<m_nWidth-1;j++){
			if(s[i*m_nWidth+j]>0&&s[i*m_nWidth+j]<255){
				m_pDibBits[i*m_nWidth+j]=(int)s[i*m_nWidth+j];
			}else{s[i*m_nWidth+j]>255?m_pDibBits[i*m_nWidth+j]=255:m_pDibBits[i*m_nWidth+j]=0;}

		}
	}
	if (s!=NULL)
	{
		delete []s;
		s=NULL;
	}
}

void CDib::LaplaceEnhanced()
{
	FLOAT *s= new FLOAT[m_nDibSize];		
	for (int i=0;i<m_nDibSize;i++)
	{
		s[i]=0;
	}
	for ( i=1;i<m_nHeight-1;i++)
	{		
		int  i_1=i-1, i_2=i+1;
		for (int j=1;j<m_nWidth-1;j++){
			int j_1=j-1, j_2=j+1;
			s[i*m_nWidth+j]=(-1*m_pDibBits[i_1*m_nWidth+j_1]-1*m_pDibBits[i_1*m_nWidth+j]-1*m_pDibBits[i_1*m_nWidth+j_2]
				-1*m_pDibBits[i*m_nWidth+j_1]+8*m_pDibBits[i*m_nWidth+j]-1*m_pDibBits[i*m_nWidth+j_2]
				-1*m_pDibBits[i_2*m_nWidth+j_1]-1*m_pDibBits[i_2*m_nWidth+j]-1*m_pDibBits[i_2*m_nWidth+j_2])/16.0;

		}
	}
	for ( i=1;i<m_nHeight-1;i++)
	{		
		for (int j=1;j<m_nWidth-1;j++){
			m_pDibBits[i*m_nWidth+j]=(int)s[i*m_nWidth+j]+m_pDibBits[i*m_nWidth+j];	
			if(m_pDibBits[i*m_nWidth+j]<0||m_pDibBits[i*m_nWidth+j]>255){
				m_pDibBits[i*m_nWidth+j]>255?m_pDibBits[i*m_nWidth+j]=255:m_pDibBits[i*m_nWidth+j]=0;
			}
		}
	}
	if (s!=NULL)
	{
		delete []s;
		s=NULL;
	}
}

void CDib::CreateDib()
{
	m_nDibSize = sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)+512*512*sizeof(unsigned char);
	m_pDib = new unsigned char[m_nDibSize];
	memset(m_pDib,0,m_nDibSize);
	m_pBitmapInfoHeader=(BITMAPINFOHEADER*)m_pDib;
	m_pBitmapInfoHeader->biWidth=512;
	m_pBitmapInfoHeader->biHeight=512;
	m_pBitmapInfoHeader->biSize=sizeof(BITMAPINFOHEADER);
	m_pBitmapInfoHeader->biPlanes=1;
	m_pBitmapInfoHeader->biBitCount=8;
	m_pBitmapInfoHeader->biCompression=BI_RGB;
	m_pBitmapInfoHeader->biSizeImage=512*512*sizeof(unsigned char);
	m_pBitmapInfoHeader->biClrUsed=0;
    m_pBitmapInfoHeader->biClrImportant=0;
    m_nWidth = m_pBitmapInfoHeader->biWidth;
    m_nHeight= m_pBitmapInfoHeader->biHeight;
	if (m_pBitmapInfoHeader->biBitCount >8)
	{
		m_nPaletteEntries = 0;
	}else
	{
		m_nPaletteEntries = 1 << m_pBitmapInfoHeader->biBitCount;
	}
	RGBQUAD * m_pPalette= (RGBQUAD *)m_pDib+sizeof(BITMAPINFOHEADER);
	for (int i=0;i<m_nPaletteEntries;i++)
	{	
		m_pPalette[i].rgbBlue=i;
		m_pPalette[i].rgbRed=i;
		m_pPalette[i].rgbGreen=i;
	}
	
    m_pDibBits = m_pDib + sizeof(BITMAPINFOHEADER) + m_nPaletteEntries*sizeof(RGBQUAD);
	for (int x=0;x<512;x++)
    {
		for(int j=0;j<512;j++)
		{
			m_pDibBits[x*m_nWidth+j]=0;
		}
    }
    
}


void CDib::DrawRectangle()
{
	for ( int x=100;x<400;x++)
    {
		for( int j=100;j<400;j++)
		{
			m_pDibBits[x*m_nWidth+j]=255;
		}
    }
}

void CDib::DrawCircle(int r)
{
	int x = 200, y = 200;
	for ( int i=0;i<512;i++)
    {
		for( int j=0;j<512;j++)
		{
			if ((i-x)*(i-x) + (j-y)*(j-y) <= 10000)
			{
				m_pDibBits[i*m_nWidth+j]=255;
			}
		}
    }
}
