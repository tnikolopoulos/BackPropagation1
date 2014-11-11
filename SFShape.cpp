#include "StdAfx.h"
#include "SFShape.h"

SFShape::SFShape(void) : X(0) , Y(0), Angle(0.0f)
{
}

SFShape::~SFShape(void)
{
}

void SFShape::draw(HDC dc)
{
	for ( UINT32 I = 0 ; I < Lines.size() ; I++)
	{
		float X1 = Lines[I].left;
		float Y1 = Lines[I].top;
		transform( X1 , Y1);		
		MoveToEx(dc , (DWORD)  X1 , (DWORD) Y1,NULL);
		
		float X2 = Lines[I].right;
		float Y2 = Lines[I].bottom;
		transform( X2 , Y2);
		LineTo( dc , (DWORD) X2 , (DWORD) Y2 );
	}
}

void SFShape::transform( float& PX , float& PY ) 
{
	float RX = PX * cos(Angle) - PY * sin(Angle);
	float RY = PX * sin(Angle) + PY * cos(Angle);
	PX = RX + X;
	PY = RY + Y;
}

