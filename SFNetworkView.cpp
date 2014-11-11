#include "StdAfx.h"
#include "SFNetworkView.h"
#include "SFNetwork.h"
#include "SFLayer.h"
#include "SFUnit.h"

COLORREF  weightToColor(float W )
{
	/*
	INT32 I = (INT32) ( W * 255.f) ;
	if ( I < 0 ) 
	{
		I *= -1;
		if ( I > 0xFF ) I = 0xFF;
		return I;
	}
	if ( I > 0xFF) I = 0xFF;
	return I << 8;
	*/
	INT32 I = (INT32) ( W * 127.f) + 127 ;

	if  ( I < 0 ) I = 0 ;
	if ( I > 0xFF)  I= 0xFF;
	return I | I << 8 | I << 16;
}

SFNetworkView::SFNetworkView(void) : PNetwork(NULL), UpdateDisplay(true) , BackBuffer(NULL) , Handle(NULL) , Scale(1.f) , SelectedLayer(-1.f) 
{
}

SFNetworkView::~SFNetworkView(void)
{
}

void SFNetworkView::initialize( HWND H)
{
	Handle = H;
	handleResize();
}

void SFNetworkView::handleResize()
{
	GetClientRect( Handle , &ClientRect );

	if ( BackBuffer != NULL ) 
		DeleteObject( BackBuffer);
		
	HDC SDC = GetDC( Handle);
	BackBuffer = CreateCompatibleBitmap( SDC , ClientRect.right , ClientRect.bottom );
	ReleaseDC(Handle,SDC);
}	

void SFNetworkView::updateView()
{
	if ( !PNetwork ) 
		return;
	if ( !UpdateDisplay )
		return;
	
	SFRetina& Retina = PNetwork->getRetina();
	UINT32 RetinaWidth = Retina.getWidth();
	
	HDC DC = GetDC( Handle);

	HDC BackBufferDC = CreateCompatibleDC( DC);
	SelectObject( BackBufferDC , BackBuffer);

	PatBlt( BackBufferDC, 0,0, ClientRect.right , ClientRect.bottom, WHITENESS);

	HDC RetinaDC = Retina.startDraw(DC);
	BitBlt(  BackBufferDC , 0, 0, Retina.getWidth() , Retina.getWidth() , RetinaDC , 0, 0, SRCCOPY);
	Retina.endDraw();
	
	std::vector<SFLayer>& Layers = PNetwork->getLayers();
	
	UINT32 OffsetX = RetinaWidth + 2 ;
	UINT32 OffsetY = RetinaWidth + 2;
	for ( UINT32 I = 0 ; I < Layers.size() ; I++ )
	{
		displayOutput(BackBufferDC , Layers[I] , OffsetX, 0 );
		OffsetX += Layers[I].getWidth() + 2 ;
	}
	OffsetX *= 2;
	if ( SelectedLayer != -1 && Layers[SelectedLayer].getSelectedUnit() != -1 )
	{
		displayWeights( BackBufferDC , Layers[SelectedLayer], 0, OffsetY);
		OffsetY += RetinaWidth + 2; 
	}
	
	/*
	if ( Layers[0].getSelectedUnit() == -1 )
		displayReconstruction(BackBufferDC, Layers[0] , Layers[0].getOutput() , 0 , OffsetY);
	else
		displayWeights( BackBufferDC , Layers[0], 0, OffsetY);
	OffsetY += RetinaWidth + 2; 

	char Buf[256];
	sprintf_s( Buf , 256 , "%d" , PNetwork->getTrainingCount() );
	TextOutA( BackBufferDC , RetinaWidth + 4 , RetinaWidth +  5 , Buf, strlen(Buf) );
	*/

	StretchBlt( DC , 0, 0, ClientRect.right , (OffsetY *  ClientRect.right) / OffsetX , BackBufferDC, 0, 0, OffsetX , OffsetY ,SRCCOPY);
	
	DeleteDC(BackBufferDC);
	ReleaseDC(Handle,DC);
	Scale = (float)ClientRect.right / (float)OffsetX;
}

void SFNetworkView::displayOutput( HDC DC , SFLayer& Layer , UINT32 SX , UINT32 SY )
{
	UINT32 Width = Layer.getWidth();
	UINT32 Height = Layer.getHeight();
	UINT32 Sz = 1;
	Rectangle(DC,SX-1, SY-1, SX + Width * Sz  +1 , SY + Height * Sz + 1);
	for ( UINT32 Y = 0; Y < Height; Y++ )
	{
		for ( UINT32 X = 0; X < Width ; X++ )
		{
			UINT32 Index = Y * Width + X ; 

			float O = Layer.getOutput()[Index]; 
			SetPixel( DC, X + SX , Y + SY , weightToColor(O) );
		}
	}
}


void SFNetworkView::displayWeights(HDC DC , SFLayer& Layer , UINT32 SX , UINT32 SY )
{
	if ( Layer.getSelectedUnit() == -1 )
		return;
	
	
	SFUnit& Unit = Layer.getUnits()[Layer.getSelectedUnit()];
	for ( UINT32 Y = 0 ; Y < Layer.getInputHeight() ; Y++ )
	{
		for ( UINT32 X = 0 ; X < Layer.getInputWidth() ; X++ )
		{
			UINT32 J = Y * Layer.getInputWidth() + X;
			SetPixel( DC , X + SX , Y + SY , weightToColor(Unit.getWeights()[J]) );
		}
	}
}

/*
void SFNetworkView::displayReconstruction(HDC DC , SFLayer& Layer , UINT64* Output , UINT32 SX , UINT32 SY )
{
	UINT32 RetinaWidth = PNetwork->getRetina().getWidth();
	UINT32* PX = new UINT32[RetinaWidth*RetinaWidth];	

	for ( UINT32 I = 0 ; I < RetinaWidth * RetinaWidth ; I++ )	
		PX[I] = 0;

	UINT32 Count = 0 ;
	for ( UINT32 I = 0 ; I < Layer.getWidth() * Layer.getWidth() ; I++ )
	{
		UINT64 Mask = 1ULL << ( I % 64 );
		if (  ( Output[I / 64] & Mask ) == 0)
			continue;

		Count++;
		SFUnit& Unit = Layer.Units[I];
		for ( UINT32 Y = 0 ; Y < RetinaWidth ; Y++ )
		{
			for ( UINT32 X = 0 ; X < RetinaWidth ; X++ )
			{
				UINT32 J = Y * RetinaWidth + X;
				if ( Unit.Connections[J / 64] & (1ULL << (J%64)) )
					PX[J] = PX[J] + 1;
			}
		}
	}
	for ( UINT32 Y = 0;  Y < RetinaWidth ; Y++ )
	{
		for ( UINT32 X = 0 ; X  < RetinaWidth ; X++ )
		{
			UINT32 P = PX[Y * RetinaWidth + X ] * 5 ;
			if ( P > 255 ) 
				P = 255;
			SetPixel( DC , X + SX , Y + SY , P);
		}dfd
	}

	delete[] PX;
}
*/
void SFNetworkView::handleLeftButtonDown(INT32 X , INT32 Y )
{
	float 	MX = X / Scale; 
	float	MY = Y / Scale;
	if ( MX < PNetwork->getRetina().getWidth() ) 
	{
		PNetwork->getShape()->setPos( MX , MY );
	}
	else 
	{
		INT32 LX = (INT32) MX - PNetwork->getRetina().getWidth() - 2;
		INT32 LY = (INT32) MY;
		UINT32 L = 0 ;
		for ( L = 0 ; L < PNetwork->getLayers().size();  L++ )
		{
			if ( LX < PNetwork->getLayers()[L].getWidth() ) 
				break;
			LX = LX - PNetwork->getLayers()[L].getWidth() - 2;
		}
		if ( L == PNetwork->getLayers().size() || LX < 0 || LX >= PNetwork->getLayers()[L].getWidth() )
			return;
		if ( LY < 0 || LY >= PNetwork->getLayers()[L].getHeight() )
			return;

		SelectedLayer = L;
		PNetwork->getLayers()[L].setSelectedUnit( LY * PNetwork->getLayers()[SelectedLayer].getWidth() + LX);
	}
}
