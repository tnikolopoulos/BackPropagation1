#include "StdAfx.h"
#include "SFNetwork.h"
#include "SFLayer.h"
 
SFNetwork::SFNetwork(void) : PShape(NULL) , Train(false) ,Learn(false), TrainCount(0) , MNISTIndex(0)
{
}

SFNetwork::~SFNetwork(void)
{
}

void SFNetwork::initialize()
{
	MNISTLoader.load("data/train-images.idx3-ubyte");
	MNISTLoader.loadLabels("data/train-labels.idx1-ubyte");

	Retina.initialize(GetDesktopWindow() , 20);
	Layers.resize(2);

	Layers[0].initialize(6,6,Retina.getWidth() ,Retina.getWidth());
	//Layers[1].initialize(2,1,6,1);
	Layers[1].initialize(20,20,6,6);

	ShapeX.addLine( 0,0,8,8);
	ShapeX.addLine( 8,0,0,8);
	//ShapeX.generateName(8 , Layers[1].getWidth() * Layers[1].getWidth() );
	ShapeX.getName().push_back(1.f);
	ShapeX.getName().push_back(0);
	//ShapeX.getName().push_back(0);

	ShapeTri.addLine( 0,0,8,0);
	ShapeTri.addLine( 8,0,8,8);
	ShapeTri.addLine( 8,8,0,8);
	ShapeTri.addLine( 0,8,0,0);
	ShapeTri.getName().push_back(0.f);
	ShapeTri.getName().push_back(1.f);
	//ShapeTri.getName().push_back(0.f);

	PShape = &ShapeX;
}

void SFNetwork::run()
{
	if ( Train )
	{
		//if ( rand() > RAND_MAX / 2 )
			//PShape = &ShapeX;
		//else
		//	PShape = &ShapeTri;
		float X = 20.f * (float)rand() / (float) RAND_MAX - 5;
		float Y = 20.f * (float)rand() / (float) RAND_MAX - 5 ;
		//PShape->setRotation( (float)rand() / (float) RAND_MAX * 6.28);
		PShape->setPos (X , Y);
		
		while( true ) 
		{
			MNISTIndex++;
			if ( MNISTIndex >= MNISTLoader.getNumImages() )
				MNISTIndex = 0;
			unsigned char C = MNISTLoader.getLabel( MNISTIndex);
			if ( C == 2 )
				break;
		}
	}


	HWND DesktopHandle = GetDesktopWindow();
	HDC DesktopDC = GetDC(DesktopHandle);
	HDC RetinaDC = Retina.startDraw(DesktopDC);
	PatBlt(RetinaDC , 0 , 0, Retina.getWidth() , Retina.getWidth() , WHITENESS);
	PShape->draw(RetinaDC);
	Retina.blit( MNISTLoader.getImageData(MNISTIndex) , MNISTLoader.getWidth() , MNISTLoader.getHeight(),-4,-4);
	std::vector<float>& Input = Retina.getOutput();

	Retina.endDraw();
	ReleaseDC(DesktopHandle,DesktopDC);

	Layers[0].feed(Input);
	Layers[1].feed(Layers[0].getOutput());
	
	if ( !Learn ) 
		return;
	// back propaagate
	//Layers[0].propagateError(  Layers[1].propagateError( Layers[1].calculateError( PShape->getName()) ) ) ;

	Layers[0].propagateError(  Layers[1].propagateError( Layers[1].calculateError( Input) ) ) ;

	/*
	std::vector<float> Test;
	Test.resize( Layers[0].getWidth()  * Layers[0].getHeight() );
	if ( PShape == &ShapeX )
		Test[0] = 1.f;
	else
		Test[1] = 1.f;
	Layers[0].propagateError( Layers[0].calculateError(Test));	
	*/
	
	TrainCount++; 
	if ( TrainCount % 20 == 0 ) // minibatch
	{
		for ( UINT32 I = 0 ; I < Layers.size() ;I++)
			Layers[I].applyWeightChanges();
	}
}

void SFNetwork::changeShape()
{
	if ( PShape == &ShapeX ) 
		PShape = &ShapeTri;
	else 
		PShape = &ShapeX; 
}