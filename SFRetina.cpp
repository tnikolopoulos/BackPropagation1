#include "StdAfx.h"
#include "SFRetina.h"

SFRetina::SFRetina(void) : Bitmap(NULL) , PBits(NULL),Output(NULL)
{
}

SFRetina::~SFRetina(void)
{
	if ( Bitmap )
		DeleteObject(Bitmap);
	if ( PBits )
		delete[] PBits;
}


void SFRetina::initialize( HWND Handle, UINT32 PWidth )
{
	Width = PWidth;
	HDC DC = GetDC(Handle);
	Bitmap = CreateCompatibleBitmap(DC,Width,Width);
	ReleaseDC( Handle, DC);
	PBits = new RGBQUAD[Width*Width];
	Output.resize( Width * Width );
}

std::vector<float>& SFRetina::getOutput()
{
	BITMAPINFO bmi = {0}; 
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader); 
	bmi.bmiHeader.biWidth = Width ; 
	bmi.bmiHeader.biHeight = Width ; 
	bmi.bmiHeader.biPlanes = 1; 
	bmi.bmiHeader.biBitCount = 32; 
	bmi.bmiHeader.biCompression = BI_RGB; 

	GetDIBits(
		DrawDC,
		Bitmap, 
		0,  
		Width,  
		PBits,
		&bmi,  
		DIB_RGB_COLORS
		);  

	for ( UINT32 I = 0 ; I < Width * Width; I++ )
	{
		Output[I] = PBits[I].rgbBlue == 0 ? 1.f : 0.f;
	}
	return Output;
}


HDC SFRetina::startDraw(HDC DC )
{
	DrawDC = CreateCompatibleDC(DC);
	SelectObject( DrawDC, Bitmap);
	return DrawDC;
}

void SFRetina::endDraw()
{
	DeleteDC(DrawDC);
	DrawDC = NULL;
}

void SFRetina::moveTo( int X ,int Y )
{
	MoveToEx( DrawDC , X , Y, NULL);
}

void SFRetina::lineTo( int X , int Y)
{
	LineTo( DrawDC , X , Y);
}

