#include "StdAfx.h"
#include "SFLayer.h"
#include "SFUnit.h"
 
SFLayer::SFLayer(void) : Width(0) , Height(0) , SelectedUnit(-1) , LastInput(NULL)
{
}

SFLayer::~SFLayer(void)
{
}

void SFLayer::initialize( UINT32 PWidth , UINT32 PHeight , UINT32 PInputWidth , UINT32 PInputHeight)
{
	InputWidth = PInputWidth;
	InputHeight = PInputHeight;
	Width = PWidth;
	Height = PHeight;
	Units.resize( Width * Height);
	for ( UINT32 I = 0 ; I < Width * Height ; I++ )
		Units[I].initialize( InputWidth,InputHeight);
	Output.resize( Width * Height);
	InputError.resize( InputWidth * InputHeight );
}

void SFLayer::feed(std::vector<float>& Input)
{
	LastInput = &Input;
	// feed forward 	
	for ( UINT32 I = 0 ; I < Units.size() ; I++) 
		Output[I] = Units[I].computeOutput(Input);
	
}

std::vector<float> SFLayer::calculateError(std::vector<float>& Target) 
{
	std::vector<float> Error;
	for ( UINT32 I = 0 ; I < Target.size() ; I++ )
	{
		float Output = Units[I].getOutput();
		Error.push_back(  Target[I] - Output );	
	}	

	return Error;
}

std::vector<float>& SFLayer::propagateError( std::vector<float>& Error ) 
{
	for  ( UINT32 I = 0 ; I < Units.size(); I++ )
		for ( UINT32 J = 0 ; J < Units[I].getDeltaWeights().size() ; J++ )
		{
			float Output = Units[I].getOutput();
			Units[I].getDeltaWeights()[J] += 0.02f * Output * ( 1 - Output) *  Error[I] * LastInput->at(J);
		}

	for ( UINT32 J = 0 ; J < InputWidth * InputHeight ; J++ )
	{
		InputError[J] = 0;	
		for  ( UINT32 I = 0 ; I < Units.size(); I++ )
		{
			float Output = Units[I].getOutput();
			InputError[J] += Units[I].getWeights()[J] * Error[I] * Output * (1 - Output);
		}
	}

	return InputError;
}

void SFLayer::applyWeightChanges()
{
	for ( UINT32 I = 0 ; I < Units.size() ;I++ )
	{
		for ( UINT32 J = 0 ; J < Units[I].getWeights().size() ; J++ )
		{
			Units[I].getWeights()[J] += Units[I].getDeltaWeights()[J];
			Units[I].getDeltaWeights()[J] = 0.f ;
		}
	}

}


