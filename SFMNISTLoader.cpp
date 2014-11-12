#include "StdAfx.h"
#include "SFMNISTLoader.h"

SFMNISTLoader::SFMNISTLoader(void) : Width(0) , Height(0) 
{
}

SFMNISTLoader::~SFMNISTLoader(void)
{
}

void reverse( UINT32& Num)
{
	BYTE* B = (BYTE*)&Num;
	std::swap(B[0] , B[3]);	
	std::swap(B[1] , B[2]);	
}

void SFMNISTLoader::load(const char* fname ) 
{
	FILE* F = NULL;
	fopen_s(&F, fname , "rb");

	if ( !F)
		return;

	UINT32 Magic = 0;
	fread(&Magic , 4 , 1, F);
	reverse(Magic);

	UINT32 NumImages = 0;
	fread(&NumImages , 4 , 1, F);
	reverse(NumImages);

	fread(&Height , 4 , 1, F );
	reverse(Height);

	fread(&Width , 4 , 1, F);
	reverse(Width);

	Images.resize( NumImages);	
	for ( UINT32 I = 0 ; I < NumImages ; I++ )
	{
		Images[I].resize(Width * Height); 	
		fread( &Images[I][0], Width  * Height, 1, F );
	}

	fclose(F);

}

void SFMNISTLoader::loadLabels(const char* fname) 
{
	FILE* F = NULL;
	fopen_s(&F, fname , "rb");

	if ( !F)
		return;

	UINT32 Magic = 0;
	fread(&Magic , 4 , 1, F);
	reverse(Magic);

	UINT32 NumLabels;
	fread(&NumLabels , 4 , 1, F);
	reverse(NumLabels);

	Labels.resize( NumLabels);	
	fread(&Labels[0] , NumLabels , 1, F);
	fclose(F);
}