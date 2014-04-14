// Dib.h: interface for the CDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIB_H__24EDECEB_990B_4C1D_A402_327C5D84F876__INCLUDED_)
#define AFX_DIB_H__24EDECEB_990B_4C1D_A402_327C5D84F876__INCLUDED_
#include <complex>
using namespace std;
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define Pi 3.14159265
#define Max_Units 1000
#define WIDTHBYTES(i)((i+31)/32*4) //²¹ÉÏ4µÄ±¶Êý

const int SmothKernel[9]={1,1,1,1,1,1,1,1,1};
const int LaplacianKernel[9]={-1,-1,-1,-1,9,-1,-1,-1,-1};

class CDib  
{
public:
	CDib();
	CDib(CDib &);
	virtual ~CDib();

public:
	unsigned char*m_pDib;
	unsigned char *m_pDibData;
	unsigned char*pRawData;
	BITMAPINFOHEADER * m_pBitmapInfoHeader;
	PALETTEENTRY *m_pPaletteEntry;
	unsigned char* m_pDibBits;
	long m_nWidth;
	long m_nHeight;
	long m_nPaletteEntries;
	long m_nDibSize;
	long m_length;

public:
	long m_nWidthBytes;
	double e;
	void DrawCircle(int);
	void CreateDib();
	void DrawRectangle();
	void LaplaceEnhanced();
	void Smooth();
	void HistogramEqu();
	void GrayScale();
	long * m_pGrayScale;
	void RemoveRed();
	void Inverse();
	void LoadFile(const char*);
	void Draw(CDC *);
	BOOL FFT(unsigned char*, long, long, int, BOOL);
	void FFT_2D(complex<double> * , int, int , complex<double> *);
	void FFT_1D(complex<double> * , complex<double> * , int);
	void IFFT_2D(complex<double> * , complex<double> * , int, int);
	void IFFT_1D(complex<double> *, complex<double> *, int);
	BOOL Gaussian(unsigned char*, long, long, int, BOOL);
	BOOL Butterworth(unsigned char*, long, long, int, BOOL);
};




#endif // !defined(AFX_DIB_H__24EDECEB_990B_4C1D_A402_327C5D84F876__INCLUDED_)