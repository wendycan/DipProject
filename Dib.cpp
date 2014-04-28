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
	unsigned char*	lpSrc;							// 指向源图像的指针
	int y ;										// 循环控制变量
	int x ;										// 循环控制变量
	double dTmpOne ;								//存放临时变量
	double dTmpTwo ;								//存放临时变量
	int nTransWidth ;								// 傅立叶变换的宽度（2的整数次幂）
	int nTransHeight;								// 傅立叶变换的高度（2的整数次幂）
	double unchValue;								// 存贮图像各像素灰度的临时变量
	complex<double> * pCTData ;						// 指向时域数据的指针
	complex<double> * pCFData ;						// 指向频域数据的指针
	// 计算进行傅立叶变换的点数－横向	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)	;
	dTmpTwo = pow(2,dTmpTwo);
	nTransWidth = (int) dTmpTwo;	
	// 计算进行傅立叶变换的点数－纵向 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)	;
	dTmpTwo = pow(2,dTmpTwo);
	nTransHeight = (int) dTmpTwo;
	double dReal;									// 傅立叶变换的实部
	double dImag;									// 傅立叶变换的虚部
	
	pCTData=new complex<double>[nTransWidth * nTransHeight];	// 分配内存
	pCFData=new complex<double>[nTransWidth * nTransHeight];	// 分配内存
	// 图像数据的宽和高不一定是2的整数次幂，所以pCTData有一部分数据需要补0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);		// 补零
		}
	}
	//把图像数据传给pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			// 指向DIB第y行，第x个象素的指针
			lpSrc = (unsigned char*)pDIBBits + nWidth * (nHeight - 1 - y) + x;
			unchValue = (*lpSrc)*pow(-1,x+y);
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	FFT_2D(pCTData, nWidth, nHeight, pCFData) ;					// 傅立叶正变换
	if (m_nRadius == FALSE)
	{
		// Forward FFT
		 for(y=0; y<nHeight; y++)						// 反变换的数据传给lpDIBBits
		 {
		 	for(x=0; x<nWidth; x++)
		 	{
		 		dReal = pCTData[y*nTransWidth + x].real() ;
		 		dImag = pCTData[y*nTransWidth + x].imag() ;
		 		unchValue = sqrt(dImag*dImag+dReal*dReal)/100;
		 		if(unchValue>255)unchValue=255;
		 			
		 		// 指向DIB第y行，第x个象素的指针
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
				// 高通滤波
				if ((low == FALSE) && (Rtemp <= m_nRadius))
				{
					pCFData[k*nTransWidth + l] =complex<double>(0,0);
				}
				// 低通滤波
				else if((low == TRUE) && (Rtemp >= m_nRadius)){
					pCFData[k*nTransWidth + l] =complex<double>(0,0);
				}
			}
		}
		
		IFFT_2D(pCFData, pCTData,nHeight, nWidth); 				// 图象进行反变换
		
		for(y=0; y<nHeight; y++)								// 反变换的数据传给lpDIBBits
		{
			for(x=0; x<nWidth; x++)
			{
				//需要考虑信号的正负问题以及实际所用的图象数据是灰度值还是原始数据
				dReal = pCTData[y*nTransWidth + x].real() ;		// 实部
				dImag = pCTData[y*nTransWidth + x].imag() ;		// 虚部
				unchValue = dReal*pow(-1,x+y);
				// 指向DIB第y行，第x个象素的指针
				lpSrc = (unsigned char*)pDIBBits + nWidth * (nHeight - 1 - y) + x;
				*lpSrc =unchValue ;
			}
		}	
	}
	delete pCTData;										// 释放内存
	delete pCFData;										// 释放内存
	pCTData = NULL;
	pCFData = NULL;	
	return (true);										//返回结果
}

/*************************************************************************
 *
 * \函数名称：
 *   FFT_1D()
 *
 * \输入参数:
 *   complex<double> * pCTData	- 指向时域数据的指针，输入的需要变换的数据
 *   complex<double> * pCFData	- 指向频域数据的指针，输出的经过变换的数据
 *   nLevel						－傅立叶变换蝶形算法的级数，2的幂数，
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   一维快速傅立叶变换。
 *
 *************************************************************************
 */
void CDib::FFT_1D(complex<double> * pCTData, complex<double> * pCFData, int nLevel)
{
		// 循环控制变量
	int		i;
	int     j;
	int     k;

	// 傅立叶变换点数
	int	nCount =0 ;

	// 计算傅立叶变换点数
	nCount =(int)pow(2,nLevel) ;
	
	// 某一级的长度
	int		nBtFlyLen;
	nBtFlyLen = 0 ;
	
	// 变换系数的角度 ＝2 * PI * i / nCount
	double	dAngle;
	
	complex<double> *pCW ;
	
	// 分配内存，存储傅立叶变化需要的系数表
	pCW  = new complex<double>[nCount / 2];

    // 计算傅立叶变换的系数
	for(i = 0; i < nCount / 2; i++)
	{
		dAngle = -2 * PI * i / nCount;
		pCW[i] = complex<double> ( cos(dAngle), sin(dAngle) );
	}

	// 变换需要的工作空间
	complex<double> *pCWork1,*pCWork2; 
	
	// 分配工作空间
	pCWork1 = new complex<double>[nCount];

	pCWork2 = new complex<double>[nCount];

	
	// 临时变量
	complex<double> *pCTmp;
	
	// 初始化，写入数据
	memcpy(pCWork1, pCTData, sizeof(complex<double>) * nCount);

	// 临时变量
	int nInter; 
	nInter = 0;

	// 蝶形算法进行快速傅立叶变换
	for(k = 0; k < nLevel; k++)
	{
		for(j = 0; j < (int)pow(2,k); j++)
		{
			//计算长度
			nBtFlyLen = (int)pow( 2,(nLevel-k) );
			
			//倒序重排，加权计算
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

		// 交换 pCWork1和pCWork2的数据
		pCTmp   = pCWork1	;
		pCWork1 = pCWork2	;
		pCWork2 = pCTmp		;
	}
	
	// 重新排序
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
	
	// 释放内存空间
	delete pCW;
	delete pCWork1;
	delete pCWork2;
	pCW		=	NULL	;
	pCWork1 =	NULL	;
	pCWork2 =	NULL	;

}
/*************************************************************************
 *
 * \函数名称：
 *    IFFT_1D()
 *
 * \输入参数:
 *   complex<double> * pCTData	- 指向时域数据的指针，输入的需要反变换的数据
 *   complex<double> * pCFData	- 指向频域数据的指针，输出的经过反变换的数据
 *   nLevel						－傅立叶变换蝶形算法的级数，2的幂数，
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   一维快速傅立叶反变换。
 *
 ************************************************************************
 */
void CDib::IFFT_1D(complex<double> * pCFData, complex<double> * pCTData, int nLevel)
{
	
	// 循环控制变量
	int		i;

	// 傅立叶反变换点数
	int nCount;

	// 计算傅立叶变换点数
	nCount = (int)pow(2,nLevel) ;
	
	// 变换需要的工作空间
	complex<double> *pCWork;	
	
	// 分配工作空间
	pCWork = new complex<double>[nCount];
	
	// 将需要反变换的数据写入工作空间pCWork
	memcpy(pCWork, pCFData, sizeof(complex<double>) * nCount);
	
	// 为了利用傅立叶正变换,可以把傅立叶频域的数据取共轭
	// 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭
	for(i = 0; i < nCount; i++)
	{
		pCWork[i] = complex<double> (pCWork[i].real(), -pCWork[i].imag());
	}
	
	// 调用快速傅立叶变换实现反变换，结果存储在pCTData中
	FFT_1D(pCWork, pCTData, nLevel);
	
	// 求时域点的共轭，求得最终结果
	// 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据
	// 相差一个系数nCount
	for(i = 0; i < nCount; i++)
	{
		pCTData[i] = 
			complex<double> (pCTData[i].real() / nCount, -pCTData[i].imag() / nCount);
	}
	
	// 释放内存
	delete pCWork;
	pCWork = NULL;
}

/*************************************************************************
 *
 * \函数名称：
 *   FFT_2D()
 *
 * \输入参数:
 *   complex<double> * pCTData	- 图像数据
 *   int    nWidth				- 数据宽度
 *   int    nHeight				- 数据高度
 *   complex<double> * pCFData	- 傅立叶变换后的结果
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   二维傅立叶变换。
 *
 ************************************************************************
 */
void CDib::FFT_2D(complex<double> * pCTData, int nWidth, int nHeight, complex<double> * pCFData)
{
	
	// 循环控制变量
	int	x;
	int	y;
	
	// 临时变量
	double	dTmpOne;
	double  dTmpTwo;
	
	// 进行傅立叶变换的宽度和高度，（2的整数次幂）
	// 图像的宽度和高度不一定为2的整数次幂
	int		nTransWidth;
	int 	nTransHeight;

	// 计算进行傅立叶变换的宽度	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// 计算进行傅立叶变换的高度 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo	   ;	
	
	// x，y（行列）方向上的迭代次数
	int		nXLev;
	int		nYLev;

	// 计算x，y（行列）方向上的迭代次数
	nXLev = (int) ( log(nTransWidth)/log(2) +  0.5 );
	nYLev = (int) ( log(nTransHeight)/log(2) + 0.5 );
	
	for(y = 0; y < nTransHeight; y++)
	{
		// x方向进行快速傅立叶变换
		FFT_1D(&pCTData[nTransWidth * y], &pCFData[nTransWidth * y], nXLev);
	}
	
	// pCFData中目前存储了pCTData经过行变换的结果
	// 为了直接利用FFT_1D，需要把pCFData的二维数据转置，再一次利用FFT_1D进行
	// 傅立叶行变换（实际上相当于对列进行傅立叶变换）
	for(y = 0; y < nTransHeight; y++)
	{
		for(x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransHeight * x + y] = pCFData[nTransWidth * y + x];
		}
	}
	
	for(x = 0; x < nTransWidth; x++)
	{
		// 对x方向进行快速傅立叶变换，实际上相当于对原来的图象数据进行列方向的
		// 傅立叶变换
		FFT_1D(&pCTData[x * nTransHeight], &pCFData[x * nTransHeight], nYLev);
	}

	// pCFData中目前存储了pCTData经过二维傅立叶变换的结果，但是为了方便列方向
	// 的傅立叶变换，对其进行了转置，现在把结果转置回来
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
 * \函数名称：
 *   IFFT_2D()
 *
 * \输入参数:
 *   complex<double> * pCFData	- 频域数据
 *   complex<double> * pCTData	- 时域数据
 *   int    nWidth				- 图像数据宽度
 *   int    nHeight				- 图像数据高度
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   二维傅立叶反变换。
 *
 ************************************************************************
 */
void CDib::IFFT_2D(complex<double> * pCFData, complex<double> * pCTData, int nWidth, int nHeight)
{
	// 循环控制变量
	int	x;
	int	y;
	
	// 临时变量
	double	dTmpOne;
	double  dTmpTwo;
	
	// 进行傅立叶变换的宽度和高度，（2的整数次幂）
	// 图像的宽度和高度不一定为2的整数次幂
	int		nTransWidth;
	int 	nTransHeight;

	// 计算进行傅立叶变换的宽度	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransWidth = (int) dTmpTwo	   ;
	
	// 计算进行傅立叶变换的高度 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
	nTransHeight = (int) dTmpTwo	   ;
	
	// 分配工作需要的内存空间
	complex<double> *pCWork= new complex<double>[nTransWidth * nTransHeight];

	//临时变量
	complex<double> *pCTmp ;
	
	// 为了利用傅立叶正变换,可以把傅立叶频域的数据取共轭
	// 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭
	for(y = 0; y < nTransHeight; y++)
	{
		for(x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCFData[nTransWidth * y + x] ;
			pCWork[nTransWidth * y + x] = complex<double>( pCTmp->real() , -pCTmp->imag() );
		}
	}

	// 调用傅立叶正变换
	FFT_2D(pCWork, nWidth, nHeight, pCTData) ;
	
	// 求时域点的共轭，求得最终结果
	// 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据
	// 相差一个系数
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
	unsigned char*	lpSrc;							// 指向源图像的指针
	int y ;										// 循环控制变量
	int x ;										// 循环控制变量
	double dTmpOne ;								//存放临时变量
	double dTmpTwo ;								//存放临时变量
	int nTransWidth ;								// 傅立叶变换的宽度（2的整数次幂）
	int nTransHeight;								// 傅立叶变换的高度（2的整数次幂）
	double unchValue;								// 存贮图像各像素灰度的临时变量
	complex<double> * pCTData ;						// 指向时域数据的指针
	complex<double> * pCFData ;						// 指向频域数据的指针
	// 计算进行傅立叶变换的点数－横向	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)	;
	dTmpTwo = pow(2,dTmpTwo);
	nTransWidth = (int) dTmpTwo;	
	// 计算进行傅立叶变换的点数－纵向 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)	;
	dTmpTwo = pow(2,dTmpTwo);
	nTransHeight = (int) dTmpTwo;
	double dReal;									// 傅立叶变换的实部
	double dImag;									// 傅立叶变换的虚部
	
	pCTData=new complex<double>[nTransWidth * nTransHeight];	// 分配内存
	pCFData=new complex<double>[nTransWidth * nTransHeight];	// 分配内存
	// 图像数据的宽和高不一定是2的整数次幂，所以pCTData有一部分数据需要补0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);		// 补零
		}
	}
	//把图像数据传给pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			// 指向DIB第y行，第x个象素的指针
			lpSrc = (unsigned char*)pDIBBits + nWidth * (nHeight - 1 - y) + x;
			unchValue = (*lpSrc)*pow(-1,x+y);
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	FFT_2D(pCTData, nWidth, nHeight, pCFData) ;					// 傅立叶正变换

	for (int k=0;k<nTransHeight;k++)
	{
		for (int l=0;l<nTransWidth;l++)
		{
			double Rtemp = (1.0*k- nTransHeight/2.0)*(1.0*k- nTransHeight/2.0) + ( 1.0*l-nTransWidth/2.0)*( 1.0*l-nTransWidth/2.0);
			Rtemp = sqrt(Rtemp);
			int D0 = 10; //截止频率
			// 高通滤波
			if (low == FALSE){
				if (Rtemp <= m_nRadius)
				{
					pCFData[k*nTransWidth + l] =complex<double>(0,0);
				} else {
					pCFData[k*nTransWidth + l] *= exp(-Rtemp*Rtemp/(2*D0*D0));
				}
			}
			// 低通滤波
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
		
	IFFT_2D(pCFData, pCTData,nHeight, nWidth); 				// 图象进行反变换
		
	for(y=0; y<nHeight; y++)								// 反变换的数据传给lpDIBBits
	{
		for(x=0; x<nWidth; x++)
		{
			//需要考虑信号的正负问题以及实际所用的图象数据是灰度值还是原始数据
			dReal = pCTData[y*nTransWidth + x].real() ;		// 实部
			dImag = pCTData[y*nTransWidth + x].imag() ;		// 虚部
			unchValue = dReal*pow(-1,x+y);
			// 指向DIB第y行，第x个象素的指针
			lpSrc = (unsigned char*)pDIBBits + nWidth * (nHeight - 1 - y) + x;
			*lpSrc =unchValue ;
		}
	}	
	
	delete pCTData;										// 释放内存
	delete pCFData;										// 释放内存
	pCTData = NULL;
	pCFData = NULL;	
	return (true);										//返回结果
}

BOOL CDib::Butterworth(unsigned char* pDIBBits, long nWidth, long nHeight, int m_nRadius, BOOL low)
{
	unsigned char*	lpSrc;							// 指向源图像的指针
	int y ;										// 循环控制变量
	int x ;										// 循环控制变量
	double dTmpOne ;								//存放临时变量
	double dTmpTwo ;								//存放临时变量
	int nTransWidth ;								// 傅立叶变换的宽度（2的整数次幂）
	int nTransHeight;								// 傅立叶变换的高度（2的整数次幂）
	double unchValue;								// 存贮图像各像素灰度的临时变量
	complex<double> * pCTData ;						// 指向时域数据的指针
	complex<double> * pCFData ;						// 指向频域数据的指针
	// 计算进行傅立叶变换的点数－横向	（2的整数次幂）
	dTmpOne = log(nWidth)/log(2);
	dTmpTwo = ceil(dTmpOne)	;
	dTmpTwo = pow(2,dTmpTwo);
	nTransWidth = (int) dTmpTwo;	
	// 计算进行傅立叶变换的点数－纵向 （2的整数次幂）
	dTmpOne = log(nHeight)/log(2);
	dTmpTwo = ceil(dTmpOne)	;
	dTmpTwo = pow(2,dTmpTwo);
	nTransHeight = (int) dTmpTwo;
	double dReal;									// 傅立叶变换的实部
	double dImag;									// 傅立叶变换的虚部
	
	pCTData=new complex<double>[nTransWidth * nTransHeight];	// 分配内存
	pCFData=new complex<double>[nTransWidth * nTransHeight];	// 分配内存
	// 图像数据的宽和高不一定是2的整数次幂，所以pCTData有一部分数据需要补0
	for(y=0; y<nTransHeight; y++)
	{
		for(x=0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x]=complex<double>(0,0);		// 补零
		}
	}
	//把图像数据传给pCTData
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			// 指向DIB第y行，第x个象素的指针
			lpSrc = (unsigned char*)pDIBBits + nWidth * (nHeight - 1 - y) + x;
			unchValue = (*lpSrc)*pow(-1,x+y);
			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
		}
	}
	FFT_2D(pCTData, nWidth, nHeight, pCFData) ;					// 傅立叶正变换

	for (int k=0;k<nTransHeight;k++)
	{
		for (int l=0;l<nTransWidth;l++)
		{
			double Rtemp = (1.0*k- nTransHeight/2.0)*(1.0*k- nTransHeight/2.0) + ( 1.0*l-nTransWidth/2.0)*( 1.0*l-nTransWidth/2.0);
			Rtemp = sqrt(Rtemp);
			int n = 2; //滤波阶数
			int D0 = 10; //截止频率
			// 高通滤波
			if (low == FALSE){
				if (Rtemp <= m_nRadius)
				{
					pCFData[k*nTransWidth + l] =complex<double>(0,0);
				} else {
					pCFData[k*nTransWidth + l] *= 1/(1+ pow(Rtemp/D0, 2*n));
				}
			}
			// 低通滤波
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
		
	IFFT_2D(pCFData, pCTData,nHeight, nWidth); 				// 图象进行反变换
		
	for(y=0; y<nHeight; y++)								// 反变换的数据传给lpDIBBits
	{
		for(x=0; x<nWidth; x++)
		{
			//需要考虑信号的正负问题以及实际所用的图象数据是灰度值还是原始数据
			dReal = pCTData[y*nTransWidth + x].real() ;		// 实部
			dImag = pCTData[y*nTransWidth + x].imag() ;		// 虚部
			unchValue = dReal*pow(-1,x+y);
			// 指向DIB第y行，第x个象素的指针
			lpSrc = (unsigned char*)pDIBBits + nWidth * (nHeight - 1 - y) + x;
			*lpSrc =unchValue ;
		}
	}	
	
	delete pCTData;										// 释放内存
	delete pCFData;										// 释放内存
	pCTData = NULL;
	pCFData = NULL;	
	return (true);										//返回结果
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
	// 旋转角度（弧度）
	float	fRotateAngle;
	// 旋转角度的正弦和余弦
	float	fSina, fCosa;
	// 源图四个角的坐标（以图像中心为坐标系原点）
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;	
	// 旋转后四个角的坐标（以图像中心为坐标系原点）
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	fRotateAngle = (float) RADIAN(deg);
	
	if (left == TRUE) //逆时针旋转
	{
		deg += 15;
	} else {  //顺时针旋转
		deg -= 15;
	}
	// 计算旋转角度的正弦
	fSina = (float) sin((double)fRotateAngle);
	
	// 计算旋转角度的余弦
	fCosa = (float) cos((double)fRotateAngle);
	// 计算原图的四个角的坐标（以图像中心为坐标系原点）
	fSrcX1 = 0;
	fSrcY1 = 0;
	fSrcX2 = 0;
	fSrcY2 = m_nHeight;
	fSrcX3 = m_nWidth;
	fSrcY3 = m_nHeight;
	fSrcX4 = m_nWidth;
	fSrcY4 = 0;
	
	// 计算新图四个角的坐标（以图像中心为坐标系原点）
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
