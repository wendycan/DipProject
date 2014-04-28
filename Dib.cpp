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
	level = 1;
	deg = 0;
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
	if (BFH.bfType!='MB')//�ж��ļ�ͷ��ǰ��λ�Ƿ�ΪMB?
	{
		AfxMessageBox("not a bitmap file!",MB_OK);
		return;

	}
	m_nDibSize=file.GetLength()-sizeof(BITMAPFILEHEADER);//ǰ��Ϊ�ܳ��ȣ�����Ϊ�ļ�ͷ�ĳ���
    m_pDib = new unsigned char[m_nDibSize];
    memset(m_pDib,0,m_nDibSize);//����ֵ����0
	file.Read(m_pDib,m_nDibSize);
	m_pBitmapInfoHeader = (BITMAPINFOHEADER*)m_pDib;
	m_nWidth = m_pBitmapInfoHeader->biWidth;
	m_nHeight = m_pBitmapInfoHeader->biHeight;
	m_length = m_nWidth * m_nHeight;
	old_height = m_nHeight;
	old_width = m_nWidth;
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

	m_pDibBits_static = new unsigned char[m_nWidth*m_nHeight];
	memset(m_pDibBits_static, 0, m_nWidth*m_nHeight);
	memcpy(m_pDibBits_static, m_pDibBits, m_nWidth*m_nHeight);
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
			if ((i-x)*(i-x) + (j-y)*(j-y) <= r*r)
			{
				m_pDibBits[i*m_nWidth+j]=255;
			}
		}
    }
}

BOOL CDib::FFT(unsigned char* pDIBBits, long nWidth, long nHeight, int m_nRadius, BOOL low)
{ 
	unsigned char*	lpSrc;							// ָ��Դͼ���ָ��
	int y ;										// ѭ�����Ʊ���
	int x ;										// ѭ�����Ʊ���
	double dTmpOne ;								//�����ʱ����
	double dTmpTwo ;								//�����ʱ����
	int nTransWidth ;								// ����Ҷ�任�Ŀ�ȣ�2���������ݣ�
	int nTransHeight;								// ����Ҷ�任�ĸ߶ȣ�2���������ݣ�
	double unchValue;								// ����ͼ������ػҶȵ���ʱ����
	complex<double> * pCTData ;						// ָ��ʱ�����ݵ�ָ��
	complex<double> * pCFData ;						// ָ��Ƶ�����ݵ�ָ��
	// ������и���Ҷ�任�ĵ���������	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)	;
	dTmpTwo = pow(2,dTmpTwo);
	nTransWidth = (int) dTmpTwo;	
	// ������и���Ҷ�任�ĵ��������� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)	;
	dTmpTwo = pow(2,dTmpTwo);
	nTransHeight = (int) dTmpTwo;
	double dReal;									// ����Ҷ�任��ʵ��
	double dImag;									// ����Ҷ�任���鲿
	
	pCTData=new complex<double>[nTransWidth * nTransHeight];	// �����ڴ�
	pCFData=new complex<double>[nTransWidth * nTransHeight];	// �����ڴ�
	// ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData��һ����������Ҫ��0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);		// ����
		}
	}
	//��ͼ�����ݴ���pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			// ָ��DIB��y�У���x�����ص�ָ��
			lpSrc = (unsigned char*)pDIBBits + nWidth * (nHeight - 1 - y) + x;
			unchValue = (*lpSrc)*pow(-1,x+y);
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	FFT_2D(pCTData, nWidth, nHeight, pCFData) ;					// ����Ҷ���任
	if (m_nRadius == FALSE)
	{
		// Forward FFT
		 for(y=0; y<nHeight; y++)						// ���任�����ݴ���lpDIBBits
		 {
		 	for(x=0; x<nWidth; x++)
		 	{
		 		dReal = pCTData[y*nTransWidth + x].real() ;
		 		dImag = pCTData[y*nTransWidth + x].imag() ;
		 		unchValue = sqrt(dImag*dImag+dReal*dReal)/100;
		 		if(unchValue>255)unchValue=255;
		 			
		 		// ָ��DIB��y�У���x�����ص�ָ��
		 		lpSrc = (unsigned char*)pDIBBits + nWidth *(nHeight-1-y) + x;
		 		*lpSrc =unchValue ;
			}
		}	
	}

	else{	
		for (int k=0;k<nTransHeight;k++)
		{
			for (int l=0;l<nTransWidth;l++)
			{
				double Rtemp = (1.0*k- nTransHeight/2.0)*(1.0*k- nTransHeight/2.0) + ( 1.0*l-nTransWidth/2.0)*( 1.0*l-nTransWidth/2.0);
				Rtemp = sqrt(Rtemp);
				// ��ͨ�˲�
				if ((low == FALSE) && (Rtemp <= m_nRadius))
				{
					pCFData[k*nTransWidth + l] =complex<double>(0,0);
				}
				// ��ͨ�˲�
				else if((low == TRUE) && (Rtemp >= m_nRadius)){
					pCFData[k*nTransWidth + l] =complex<double>(0,0);
				}
			}
		}
		
		IFFT_2D(pCFData, pCTData,nHeight, nWidth); 				// ͼ����з��任
		
		for(y=0; y<nHeight; y++)								// ���任�����ݴ���lpDIBBits
		{
			for(x=0; x<nWidth; x++)
			{
				//��Ҫ�����źŵ����������Լ�ʵ�����õ�ͼ�������ǻҶ�ֵ����ԭʼ����
				dReal = pCTData[y*nTransWidth + x].real() ;		// ʵ��
				dImag = pCTData[y*nTransWidth + x].imag() ;		// �鲿
				unchValue = dReal*pow(-1,x+y);
				// ָ��DIB��y�У���x�����ص�ָ��
				lpSrc = (unsigned char*)pDIBBits + nWidth * (nHeight - 1 - y) + x;
				*lpSrc =unchValue ;
			}
		}	
	}
	delete pCTData;										// �ͷ��ڴ�
	delete pCFData;										// �ͷ��ڴ�
	pCTData = NULL;
	pCFData = NULL;	
	return (true);										//���ؽ��
}

/*************************************************************************
 *
 * \�������ƣ�
 *   FFT_1D()
 *
 * \�������:
 *   complex<double> * pCTData	- ָ��ʱ�����ݵ�ָ�룬�������Ҫ�任������
 *   complex<double> * pCFData	- ָ��Ƶ�����ݵ�ָ�룬����ľ����任������
 *   nLevel						������Ҷ�任�����㷨�ļ�����2��������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   һά���ٸ���Ҷ�任��
 *
 *************************************************************************
 */
void CDib::FFT_1D(complex<double> * pCTData, complex<double> * pCFData, int nLevel)
{
		// ѭ�����Ʊ���
	int		i;
	int     j;
	int     k;

	// ����Ҷ�任����
	int	nCount =0 ;

	// ���㸵��Ҷ�任����
	nCount =(int)pow(2,nLevel) ;
	
	// ĳһ���ĳ���
	int		nBtFlyLen;
	nBtFlyLen = 0 ;
	
	// �任ϵ���ĽǶ� ��2 * PI * i / nCount
	double	dAngle;
	
	complex<double> *pCW ;
	
	// �����ڴ棬�洢����Ҷ�仯��Ҫ��ϵ����
	pCW  = new complex<double>[nCount / 2];

    // ���㸵��Ҷ�任��ϵ��
	for(i = 0; i < nCount / 2; i++)
	{
		dAngle = -2 * PI * i / nCount;
		pCW[i] = complex<double> ( cos(dAngle), sin(dAngle) );
	}

	// �任��Ҫ�Ĺ����ռ�
	complex<double> *pCWork1,*pCWork2; 
	
	// ���乤���ռ�
	pCWork1 = new complex<double>[nCount];

	pCWork2 = new complex<double>[nCount];

	
	// ��ʱ����
	complex<double> *pCTmp;
	
	// ��ʼ����д������
	memcpy(pCWork1, pCTData, sizeof(complex<double>) * nCount);

	// ��ʱ����
	int nInter; 
	nInter = 0;

	// �����㷨���п��ٸ���Ҷ�任
	for(k = 0; k < nLevel; k++)
	{
		for(j = 0; j < (int)pow(2,k); j++)
		{
			//���㳤��
			nBtFlyLen = (int)pow( 2,(nLevel-k) );
			
			//�������ţ���Ȩ����
			for(i = 0; i < nBtFlyLen/2; i++)
			{
				nInter = j * nBtFlyLen;
				pCWork2[i + nInter] = 
					pCWork1[i + nInter] + pCWork1[i + nInter + nBtFlyLen / 2];
				pCWork2[i + nInter + nBtFlyLen / 2] =
					(pCWork1[i + nInter] - pCWork1[i + nInter + nBtFlyLen / 2]) 
					* pCW[(int)(i * pow(2,k))];
			}
		}

		// ���� pCWork1��pCWork2������
		pCTmp   = pCWork1	;
		pCWork1 = pCWork2	;
		pCWork2 = pCTmp		;
	}
	
	// ��������
	for(j = 0; j < nCount; j++)
	{
		nInter = 0;
		for(i = 0; i < nLevel; i++)
		{
			if ( j&(1<<i) )
			{
				nInter += 1<<(nLevel-i-1);
			}
		}
		pCFData[j]=pCWork1[nInter];
	}
	
	// �ͷ��ڴ�ռ�
	delete pCW;
	delete pCWork1;
	delete pCWork2;
	pCW		=	NULL	;
	pCWork1 =	NULL	;
	pCWork2 =	NULL	;

}
/*************************************************************************
 *
 * \�������ƣ�
 *    IFFT_1D()
 *
 * \�������:
 *   complex<double> * pCTData	- ָ��ʱ�����ݵ�ָ�룬�������Ҫ���任������
 *   complex<double> * pCFData	- ָ��Ƶ�����ݵ�ָ�룬����ľ������任������
 *   nLevel						������Ҷ�任�����㷨�ļ�����2��������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   һά���ٸ���Ҷ���任��
 *
 ************************************************************************
 */
void CDib::IFFT_1D(complex<double> * pCFData, complex<double> * pCTData, int nLevel)
{
	
	// ѭ�����Ʊ���
	int		i;

	// ����Ҷ���任����
	int nCount;

	// ���㸵��Ҷ�任����
	nCount = (int)pow(2,nLevel) ;
	
	// �任��Ҫ�Ĺ����ռ�
	complex<double> *pCWork;	
	
	// ���乤���ռ�
	pCWork = new complex<double>[nCount];
	
	// ����Ҫ���任������д�빤���ռ�pCWork
	memcpy(pCWork, pCFData, sizeof(complex<double>) * nCount);
	
	// Ϊ�����ø���Ҷ���任,���԰Ѹ���ҶƵ�������ȡ����
	// Ȼ��ֱ���������任�����������Ǹ���Ҷ���任����Ĺ���
	for(i = 0; i < nCount; i++)
	{
		pCWork[i] = complex<double> (pCWork[i].real(), -pCWork[i].imag());
	}
	
	// ���ÿ��ٸ���Ҷ�任ʵ�ַ��任������洢��pCTData��
	FFT_1D(pCWork, pCTData, nLevel);
	
	// ��ʱ���Ĺ��������ս��
	// ���ݸ���Ҷ�任ԭ�����������ķ�����õĽ����ʵ�ʵ�ʱ������
	// ���һ��ϵ��nCount
	for(i = 0; i < nCount; i++)
	{
		pCTData[i] = 
			complex<double> (pCTData[i].real() / nCount, -pCTData[i].imag() / nCount);
	}
	
	// �ͷ��ڴ�
	delete pCWork;
	pCWork = NULL;
}

/*************************************************************************
 *
 * \�������ƣ�
 *   FFT_2D()
 *
 * \�������:
 *   complex<double> * pCTData	- ͼ������
 *   int    nWidth				- ���ݿ��
 *   int    nHeight				- ���ݸ߶�
 *   complex<double> * pCFData	- ����Ҷ�任��Ľ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ��ά����Ҷ�任��
 *
 ************************************************************************
 */
void CDib::FFT_2D(complex<double> * pCTData, int nWidth, int nHeight, complex<double> * pCFData)
{
	
	// ѭ�����Ʊ���
	int	x;
	int	y;
	
	// ��ʱ����
	double	dTmpOne;
	double  dTmpTwo;
	
	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ���2���������ݣ�
	// ͼ��Ŀ�Ⱥ͸߶Ȳ�һ��Ϊ2����������
	int		nTransWidth;
	int 	nTransHeight;

	// ������и���Ҷ�任�Ŀ��	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo	   ;	
	
	// x��y�����У������ϵĵ�������
	int		nXLev;
	int		nYLev;

	// ����x��y�����У������ϵĵ�������
	nXLev = (int) ( log(nTransWidth)/log(2) +  0.5 );
	nYLev = (int) ( log(nTransHeight)/log(2) + 0.5 );
	
	for(y = 0; y < nTransHeight; y++)
	{
		// x������п��ٸ���Ҷ�任
		FFT_1D(&pCTData[nTransWidth * y], &pCFData[nTransWidth * y], nXLev);
	}
	
	// pCFData��Ŀǰ�洢��pCTData�����б任�Ľ��
	// Ϊ��ֱ������FFT_1D����Ҫ��pCFData�Ķ�ά����ת�ã���һ������FFT_1D����
	// ����Ҷ�б任��ʵ�����൱�ڶ��н��и���Ҷ�任��
	for(y = 0; y < nTransHeight; y++)
	{
		for(x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransHeight * x + y] = pCFData[nTransWidth * y + x];
		}
	}
	
	for(x = 0; x < nTransWidth; x++)
	{
		// ��x������п��ٸ���Ҷ�任��ʵ�����൱�ڶ�ԭ����ͼ�����ݽ����з����
		// ����Ҷ�任
		FFT_1D(&pCTData[x * nTransHeight], &pCFData[x * nTransHeight], nYLev);
	}

	// pCFData��Ŀǰ�洢��pCTData������ά����Ҷ�任�Ľ��������Ϊ�˷����з���
	// �ĸ���Ҷ�任�����������ת�ã����ڰѽ��ת�û���
	for(y = 0; y < nTransHeight; y++)
	{
		for(x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransWidth * y + x] = pCFData[nTransHeight * x + y];
		}
	}

}

/*************************************************************************
 *
 * \�������ƣ�
 *   IFFT_2D()
 *
 * \�������:
 *   complex<double> * pCFData	- Ƶ������
 *   complex<double> * pCTData	- ʱ������
 *   int    nWidth				- ͼ�����ݿ��
 *   int    nHeight				- ͼ�����ݸ߶�
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ��ά����Ҷ���任��
 *
 ************************************************************************
 */
void CDib::IFFT_2D(complex<double> * pCFData, complex<double> * pCTData, int nWidth, int nHeight)
{
	// ѭ�����Ʊ���
	int	x;
	int	y;
	
	// ��ʱ����
	double	dTmpOne;
	double  dTmpTwo;
	
	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ���2���������ݣ�
	// ͼ��Ŀ�Ⱥ͸߶Ȳ�һ��Ϊ2����������
	int		nTransWidth;
	int 	nTransHeight;

	// ������и���Ҷ�任�Ŀ��	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo	   ;
	
	// ���乤����Ҫ���ڴ�ռ�
	complex<double> *pCWork= new complex<double>[nTransWidth * nTransHeight];

	//��ʱ����
	complex<double> *pCTmp ;
	
	// Ϊ�����ø���Ҷ���任,���԰Ѹ���ҶƵ�������ȡ����
	// Ȼ��ֱ���������任�����������Ǹ���Ҷ���任����Ĺ���
	for(y = 0; y < nTransHeight; y++)
	{
		for(x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCFData[nTransWidth * y + x] ;
			pCWork[nTransWidth * y + x] = complex<double>( pCTmp->real() , -pCTmp->imag() );
		}
	}

	// ���ø���Ҷ���任
	FFT_2D(pCWork, nWidth, nHeight, pCTData) ;
	
	// ��ʱ���Ĺ��������ս��
	// ���ݸ���Ҷ�任ԭ�����������ķ�����õĽ����ʵ�ʵ�ʱ������
	// ���һ��ϵ��
	for(y = 0; y < nTransHeight; y++)
	{
		for(x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCTData[nTransWidth * y + x] ;
			pCTData[nTransWidth * y + x] = 
				complex<double>( pCTmp->real()/(nTransWidth*nTransHeight),
								 -pCTmp->imag()/(nTransWidth*nTransHeight) );
		}
	}
	delete pCWork ;
	pCWork = NULL ;
}

BOOL CDib::Gaussian(unsigned char* pDIBBits, long nWidth, long nHeight, int m_nRadius, BOOL low)
{
	unsigned char*	lpSrc;							// ָ��Դͼ���ָ��
	int y ;										// ѭ�����Ʊ���
	int x ;										// ѭ�����Ʊ���
	double dTmpOne ;								//�����ʱ����
	double dTmpTwo ;								//�����ʱ����
	int nTransWidth ;								// ����Ҷ�任�Ŀ�ȣ�2���������ݣ�
	int nTransHeight;								// ����Ҷ�任�ĸ߶ȣ�2���������ݣ�
	double unchValue;								// ����ͼ������ػҶȵ���ʱ����
	complex<double> * pCTData ;						// ָ��ʱ�����ݵ�ָ��
	complex<double> * pCFData ;						// ָ��Ƶ�����ݵ�ָ��
	// ������и���Ҷ�任�ĵ���������	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)	;
	dTmpTwo = pow(2,dTmpTwo);
	nTransWidth = (int) dTmpTwo;	
	// ������и���Ҷ�任�ĵ��������� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)	;
	dTmpTwo = pow(2,dTmpTwo);
	nTransHeight = (int) dTmpTwo;
	double dReal;									// ����Ҷ�任��ʵ��
	double dImag;									// ����Ҷ�任���鲿
	
	pCTData=new complex<double>[nTransWidth * nTransHeight];	// �����ڴ�
	pCFData=new complex<double>[nTransWidth * nTransHeight];	// �����ڴ�
	// ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData��һ����������Ҫ��0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);		// ����
		}
	}
	//��ͼ�����ݴ���pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			// ָ��DIB��y�У���x�����ص�ָ��
			lpSrc = (unsigned char*)pDIBBits + nWidth * (nHeight - 1 - y) + x;
			unchValue = (*lpSrc)*pow(-1,x+y);
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	FFT_2D(pCTData, nWidth, nHeight, pCFData) ;					// ����Ҷ���任

	for (int k=0;k<nTransHeight;k++)
	{
		for (int l=0;l<nTransWidth;l++)
		{
			double Rtemp = (1.0*k- nTransHeight/2.0)*(1.0*k- nTransHeight/2.0) + ( 1.0*l-nTransWidth/2.0)*( 1.0*l-nTransWidth/2.0);
			Rtemp = sqrt(Rtemp);
			int D0 = 10; //��ֹƵ��
			// ��ͨ�˲�
			if (low == FALSE){
				if (Rtemp <= m_nRadius)
				{
					pCFData[k*nTransWidth + l] =complex<double>(0,0);
				} else {
					pCFData[k*nTransWidth + l] *= exp(-Rtemp*Rtemp/(2*D0*D0));
				}
			}
			// ��ͨ�˲�
			else if(low == TRUE){
				if (Rtemp >= m_nRadius)
				{
					pCFData[k*nTransWidth + l] =complex<double>(0,0);
				} else {
					pCFData[k*nTransWidth + l] *= exp(-Rtemp*Rtemp/(2*D0*D0));
				}
			}
		}
	}
		
	IFFT_2D(pCFData, pCTData,nHeight, nWidth); 				// ͼ����з��任
		
	for(y=0; y<nHeight; y++)								// ���任�����ݴ���lpDIBBits
	{
		for(x=0; x<nWidth; x++)
		{
			//��Ҫ�����źŵ����������Լ�ʵ�����õ�ͼ�������ǻҶ�ֵ����ԭʼ����
			dReal = pCTData[y*nTransWidth + x].real() ;		// ʵ��
			dImag = pCTData[y*nTransWidth + x].imag() ;		// �鲿
			unchValue = dReal*pow(-1,x+y);
			// ָ��DIB��y�У���x�����ص�ָ��
			lpSrc = (unsigned char*)pDIBBits + nWidth * (nHeight - 1 - y) + x;
			*lpSrc =unchValue ;
		}
	}	
	
	delete pCTData;										// �ͷ��ڴ�
	delete pCFData;										// �ͷ��ڴ�
	pCTData = NULL;
	pCFData = NULL;	
	return (true);										//���ؽ��
}

BOOL CDib::Butterworth(unsigned char* pDIBBits, long nWidth, long nHeight, int m_nRadius, BOOL low)
{
	unsigned char*	lpSrc;							// ָ��Դͼ���ָ��
	int y ;										// ѭ�����Ʊ���
	int x ;										// ѭ�����Ʊ���
	double dTmpOne ;								//�����ʱ����
	double dTmpTwo ;								//�����ʱ����
	int nTransWidth ;								// ����Ҷ�任�Ŀ�ȣ�2���������ݣ�
	int nTransHeight;								// ����Ҷ�任�ĸ߶ȣ�2���������ݣ�
	double unchValue;								// ����ͼ������ػҶȵ���ʱ����
	complex<double> * pCTData ;						// ָ��ʱ�����ݵ�ָ��
	complex<double> * pCFData ;						// ָ��Ƶ�����ݵ�ָ��
	// ������и���Ҷ�任�ĵ���������	��2���������ݣ�
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)	;
	dTmpTwo = pow(2,dTmpTwo);
	nTransWidth = (int) dTmpTwo;	
	// ������и���Ҷ�任�ĵ��������� ��2���������ݣ�
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)	;
	dTmpTwo = pow(2,dTmpTwo);
	nTransHeight = (int) dTmpTwo;
	double dReal;									// ����Ҷ�任��ʵ��
	double dImag;									// ����Ҷ�任���鲿
	
	pCTData=new complex<double>[nTransWidth * nTransHeight];	// �����ڴ�
	pCFData=new complex<double>[nTransWidth * nTransHeight];	// �����ڴ�
	// ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData��һ����������Ҫ��0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);		// ����
		}
	}
	//��ͼ�����ݴ���pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			// ָ��DIB��y�У���x�����ص�ָ��
			lpSrc = (unsigned char*)pDIBBits + nWidth * (nHeight - 1 - y) + x;
			unchValue = (*lpSrc)*pow(-1,x+y);
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	FFT_2D(pCTData, nWidth, nHeight, pCFData) ;					// ����Ҷ���任

	for (int k=0;k<nTransHeight;k++)
	{
		for (int l=0;l<nTransWidth;l++)
		{
			double Rtemp = (1.0*k- nTransHeight/2.0)*(1.0*k- nTransHeight/2.0) + ( 1.0*l-nTransWidth/2.0)*( 1.0*l-nTransWidth/2.0);
			Rtemp = sqrt(Rtemp);
			int n = 2; //�˲�����
			int D0 = 10; //��ֹƵ��
			// ��ͨ�˲�
			if (low == FALSE){
				if (Rtemp <= m_nRadius)
				{
					pCFData[k*nTransWidth + l] =complex<double>(0,0);
				} else {
					pCFData[k*nTransWidth + l] *= 1/(1+ pow(Rtemp/D0, 2*n));
				}
			}
			// ��ͨ�˲�
			else if(low == TRUE){
				if (Rtemp >= m_nRadius)
				{
					pCFData[k*nTransWidth + l] =complex<double>(0,0);
				} else {
					pCFData[k*nTransWidth + l] *= 1/(1+ pow(Rtemp/D0, 2*n));
				}
			}
		}
	}
		
	IFFT_2D(pCFData, pCTData,nHeight, nWidth); 				// ͼ����з��任
		
	for(y=0; y<nHeight; y++)								// ���任�����ݴ���lpDIBBits
	{
		for(x=0; x<nWidth; x++)
		{
			//��Ҫ�����źŵ����������Լ�ʵ�����õ�ͼ�������ǻҶ�ֵ����ԭʼ����
			dReal = pCTData[y*nTransWidth + x].real() ;		// ʵ��
			dImag = pCTData[y*nTransWidth + x].imag() ;		// �鲿
			unchValue = dReal*pow(-1,x+y);
			// ָ��DIB��y�У���x�����ص�ָ��
			lpSrc = (unsigned char*)pDIBBits + nWidth * (nHeight - 1 - y) + x;
			*lpSrc =unchValue ;
		}
	}	
	
	delete pCTData;										// �ͷ��ڴ�
	delete pCFData;										// �ͷ��ڴ�
	pCTData = NULL;
	pCFData = NULL;	
	return (true);										//���ؽ��
}

void CDib::ChangeSize(BOOL ZoomIn)
{
//	if(m_pDib == NULL )
//	{
//		AfxMessageBox("No object is opened",MB_OK);
//		return;   	
//	}     //no effect
	if (ZoomIn)
	{
		level *= 1.1;  //set zoom in level
	} else {
		level *= 0.9;  //set zoom out level
	}
	int nTransWidth = level * old_width;							
	int nTransHeight = level * old_height;
	m_nWidthBytes = WIDTHBYTES(nTransWidth*m_pBitmapInfoHeader->biBitCount);
	unsigned char *tempBits = new unsigned char[nTransHeight*m_nWidthBytes];
	for (int k=0;k<nTransHeight;k++)
	{
		for (int l=0;l<nTransWidth;l++)
		{
			tempBits[k*m_nWidthBytes + l] = m_pDibBits_static[GetNearPosition(l/level, k/level)];
		}
	}
//	delete[] m_pDibBits;   //?
	m_pDibBits = tempBits;
	m_nHeight = nTransHeight;
	m_nWidth = m_nWidthBytes;
	m_pBitmapInfoHeader->biWidth=m_nWidthBytes;
	m_pBitmapInfoHeader->biHeight=nTransHeight;
}

long CDib::GetNearPosition(float x, float y)
{
	long xn = floor(x);
	long yn = floor(y);
	long xmin, ymin;
	long distance = 10000;
	for (int i = 0; i<2; i++)
	{
		for(int j = 0; j<2; j++){
			xn = xn + i;
			yn = yn + j;
			if((x - xn)*(y - yn) + (x - xn)*(y - yn) < distance){
				xmin = x;
				ymin = y;
				distance = (x - xn)*(y - yn) + (x - xn)*(y - yn);
			}
		}
	}
	if (xmin > old_width || ymin > old_height)
	{
		return (0);
	}
	return (ymin * old_width + xmin);
}

void CDib::Rotate(BOOL left)
{
	int nTransWidth = level * old_width;							
	int nTransHeight = level * old_height;
	// ��ת�Ƕȣ����ȣ�
	float	fRotateAngle;
	// ��ת�Ƕȵ����Һ�����
	float	fSina, fCosa;
	// Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;	
	// ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	fRotateAngle = (float) RADIAN(deg);
	
	if (left == TRUE) //��ʱ����ת
	{
		deg += 15;
	} else {  //˳ʱ����ת
		deg -= 15;
	}
	// ������ת�Ƕȵ�����
	fSina = (float) sin((double)fRotateAngle);
	
	// ������ת�Ƕȵ�����
	fCosa = (float) cos((double)fRotateAngle);
	// ����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fSrcX1 = 0;
	fSrcY1 = 0;
	fSrcX2 = 0;
	fSrcY2 = m_nHeight;
	fSrcX3 = m_nWidth;
	fSrcY3 = m_nHeight;
	fSrcX4 = m_nWidth;
	fSrcY4 = 0;
	
	// ������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fDstX1 =  fCosa * fSrcX1 - fSina * fSrcY1;
	fDstY1 =  fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 - fSina * fSrcY2;
	fDstY2 =  fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 - fSina * fSrcY3;
	fDstY3 =  fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 - fSina * fSrcY4;
	fDstY4 =  fSina * fSrcX4 + fCosa * fSrcY4;
	
	nTransWidth = max(abs(fDstX1 - fDstX3), abs(fDstX2 - fDstX4));
	nTransHeight = max(abs(fDstY1 - fDstY3), abs(fDstY2 - fDstY4));

	m_nWidthBytes = WIDTHBYTES(nTransWidth*m_pBitmapInfoHeader->biBitCount);
	unsigned char *tempBits = new unsigned char[nTransHeight*m_nWidthBytes];
	int x0, y0;
	for (int k=0;k<nTransHeight;k++)
	{
		for (int l=0;l<nTransWidth;l++)
		{
			x0 = (LONG) ((-(float) k) * fSina + ((float) l) * fCosa);
			y0 = (LONG) ( ((float) k) * fCosa + ((float) l) * fSina);
			if ((y0*old_width + x0 > old_width*old_height) || (y0*old_width + x0 < 0))
			{
				tempBits[k*m_nWidthBytes + l] = 255;
			} else {
				tempBits[k*m_nWidthBytes + l] = m_pDibBits_static[y0*old_width + x0];
			}
		}
	}
	m_pDibBits = tempBits;
	m_nHeight = nTransHeight;
	m_nWidth = m_nWidthBytes;
	m_pBitmapInfoHeader->biWidth=m_nWidthBytes;
	m_pBitmapInfoHeader->biHeight=nTransHeight;
}

void CDib::MedianFilter()
{
	for (int i=0;i<m_nHeight;i++)
	{
		for (int j=0;j<m_nWidth;j++)
		{
			m_pDibBits[i*m_nWidth +j] = GetMedianValue(j,i);
		}
	}
}

long CDib::GetMedianValue(int x, int y)
{
	long * result = NearBitsSort(x,y);
	return result[4];
}

void CDib::AdaptivelocalFilter()
{
	long globalVar = 400;
	int i,j;
	for (i=0;i<m_nHeight;i++)
	{
		for (j=0;j<m_nWidth;j++)
		{
			m_pDibBits[i*m_nWidth + j] = GetAdaptivelocalValue(j,i,globalVar);
		}
	}

}

void CDib::AlphaFilter()
{
	for (int i=0;i<m_nHeight;i++)
	{
		for (int j=0;j<m_nWidth;j++)
		{
			m_pDibBits[i*m_nWidth +j] = GetAlphaFixed(j,i,2);
		}
	}
}

long* CDib::NearBitsSort(int x, int y)
{
	int i,j,k=0;
	long grayValue[9];
	for (i=-1;i<2;i++)
	{
		for (j=-1;j<2;j++)
		{
			grayValue[k++] = m_pDibBits[(y+j)*m_nWidth+x+i];
		}
	}
	for (i=0;i<9;i++)
	{
		for (j=i+1;j<9;j++)
		{
			long temp;
			if (grayValue[j] > grayValue[j+1])
			{
				temp = grayValue[j];
				grayValue[j] = grayValue[j+1];
				grayValue[j+1] = temp;
			}
		}
	}
	return grayValue;
}

long CDib::GetAlphaFixed(int x, int y, int d)
{
	long * grayValue = NearBitsSort(x,y);
	int length = 9-d;
	long result[7];
	long sum = 0;
	for (int k=0;k<9-d;k++)
	{
		result[k] = grayValue[k+d/2];
		sum += result[k];
	}
	long value = sum/length;
	return value;
}

long CDib::GetAdaptivelocalValue(int x, int y, long globalVar)
{
	long * nearBits = NearBitsSort(x,y);
	long median = nearBits[4];
	long localVar,localSum = 0, localVarSquSum = 0,localMean;
	int k;
	for (k=0;k<9;k++)
	{
		localSum += nearBits[k];
	}
	localMean = localSum/9;
	for (k=0;k<9;k++)
	{
		localVarSquSum += (nearBits[k] - localMean) * (nearBits[k] - localMean);
	}
	localVar = localVarSquSum/9;
	long value = m_pDibBits[y*m_nWidth + x] - (globalVar/localVar)*(m_pDibBits[y*m_nWidth + x] - median);
	return value;
}
