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
	m_pGreyScale = NULL;
	m_pEquilibration=NULL;
    pRawData=NULL;

}

CDib::~CDib()
{	if (m_pDib!=NULL)
	{	
		delete[] m_pDib;
		m_pDib=NULL;
		m_pDibBits=NULL;
	}
	if (m_pGreyScale!=NULL)
	{
        delete[] m_pGreyScale;
        m_pGreyScale=NULL;
	}
	if (m_pEquilibration!=NULL)
	{
        delete[] m_pEquilibration;
        m_pEquilibration=NULL;
	}
	if (pRawData!=NULL)
	{
        delete[] pRawData;
        pRawData=NULL;
	}
}
void CDib::FileOpen( const char* pFileName)
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
void CDib::LoadFile( const char* FileName)
{
	CFile file;
	file.Open(FileName,CFile::modeRead|CFile::shareDenyRead,NULL);
	BITMAPFILEHEADER BFH;
	file.Read(&BFH,sizeof(BITMAPFILEHEADER));
	if (BFH.bfType!='MB')//判断文件头的前两位是否为MB
	{	
		AfxMessageBox("This is not bmp file");
		return;
	}
	m_nDibSize=file.GetLength()-sizeof(BITMAPFILEHEADER);//前者为总长度，后者为文件头的长度
	m_pDib= new unsigned char[m_nDibSize];
	memset(m_pDib,0,m_nDibSize);//所有值均赋0
	file.Read(m_pDib,m_nDibSize);
	m_pBitmapInfoHeader=(BITMAPINFOHEADER*)m_pDib;
	m_nWidth=m_pBitmapInfoHeader->biWidth;
	m_nHeight=m_pBitmapInfoHeader->biHeight;//调用类的成员函数biHeigth赋给m_nHeight
	if (m_pBitmapInfoHeader->biBitCount>8)
	{
		m_nPaletteEntries=0;
	}else
	{
		m_nPaletteEntries=1<<m_pBitmapInfoHeader->biBitCount;
	}
	m_pDibBits=m_pDib+sizeof(BITMAPINFOHEADER)+m_nPaletteEntries*sizeof(RGBQUAD);//数据区开头的指针=信息头开始的指针+信息头的长度+调色板的长度
	
}



void CDib::Draw( CDC * pDC)
{
	if (m_pDib!=NULL)
	{
		
		::StretchDIBits(pDC->m_hDC,0,0,m_nWidth,m_nHeight,0,0,m_nWidth,m_nHeight,m_pDibBits,(BITMAPINFO*)m_pBitmapInfoHeader,DIB_RGB_COLORS,SRCCOPY);
	}
}

void CDib::SaveFile( const char* FileName )
{
	CFile file;
	if(!file.Open(FileName,CFile::modeCreate|CFile::modeWrite,NULL))
	{
		return;
	}
	BITMAPFILEHEADER BFH;
	BFH.bfType='MB';
	BFH.bfSize=m_nDibSize+sizeof(BITMAPFILEHEADER);
	BFH.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof

(BITMAPINFOHEADER)+m_nPaletteEntries*sizeof(RGBQUAD);
	file.Write(&BFH,sizeof(BITMAPFILEHEADER));
	file.Write(m_pDib,m_nDibSize);
	file.Close();
	return;
}

void CDib::GreyScale()
{
	if (m_nPaletteEntries==0)
	{
		AfxMessageBox("不是灰度图");
	}
	else
	{
		m_pGreyScale=new long[m_nPaletteEntries];
		memset(m_pGreyScale,(long)0,sizeof(long)*m_nPaletteEntries);
		int i;
		for (i=0;i<m_nPaletteEntries;i++)
		{
			m_pGreyScale[i]=0;
		}
		for (i=0;i<m_nHeight;i++)
		{
			for(int j=0;j<m_nWidth;j++)
			{
				m_pGreyScale[*(m_pDibBits+i*m_nWidth+j)]=m_pGreyScale[*(m_pDibBits+i*m_nWidth+j)]+1;
			}
		}
	}

}
void CDib::New()
{
	m_nDibSize = sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD)+512*512*sizeof(unsigned 

char);
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
    for ( x=50;x<461;x++)
    {
		for( int j=50;j<461;j++)
		{
			m_pDibBits[x*m_nWidth+j]=127;
		}
    }
	
}

void CDib::Fanbai()
{
	if(m_nDibSize == NULL )
	{
	AfxMessageBox("No object is opened",MB_OK);
    return;   	
	}
	for(int i=0;i<m_length;i++){
		m_pDibBits[i] = 255 - m_pDibBits[i];// OR use point: *(m_pDibBits+i)=255-*(m_pDibBits+i);
	}

/*	for( int i=0;i<m_nPaletteEntries; i++)  //why m_nPaletteEntries = 0;
	{
	m_pPaletteEntry[i].peBlue=255-m_pPaletteEntry[i].peBlue;
	m_pPaletteEntry[i].peGreen=255-m_pPaletteEntry[i].peGreen;
    m_pPaletteEntry[i].peRed=255-m_pPaletteEntry[i].peRed;
	}*/
/*	for(int i=0;i<m_nHeight;i++)
	{
		for(int j=0;j<m_nWidth;j++)
		{
			m_pDibBits[i*m_nWidth+j]=255-m_pDibBits[i*m_nWidth+j];
		}
	}*/
}
