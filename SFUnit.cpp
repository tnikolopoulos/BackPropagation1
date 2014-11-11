#include "StdAfx.h"
#include "SFUnit.h"

SFUnit::SFUnit(void) : Output(0.f) 
{
}

SFUnit::~SFUnit(void)
{
}

void SFUnit::initialize( UINT32 Width , UINT32 Height )
{
	DeltaWeights.resize( Width * Height);
	Weights.resize(Width * Height);
	for ( UINT32 I = 0; I < Weights.size() ; I++ )
		Weights[I] = (float) rand() / (float)RAND_MAX - 0.5f;
}

float SFUnit::computeOutput( std::vector<float>& Input )
{
	float S = 0 ;
	for ( UINT32 I = 0 ; I < Input.size() ; I++)
		S += Weights[I] * Input[I];
	
	Output = 1.f / ( 1.f + powf( 2.7182f, -S ) ) ;
	return Output;
}