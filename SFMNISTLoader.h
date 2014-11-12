#pragma once

class SFMNISTLoader
{
public:
	SFMNISTLoader(void);
	~SFMNISTLoader(void);

	void load(const char* fname );
	void loadLabels(const char* fname);


	UINT32 getWidth() { return Width;}		
	UINT32 getHeight() { return Height;}
	UINT32 getNumImages() { return Images.size();}
	BYTE*	getImageData( UINT32 Index) { return &Images[Index][0];}
	BYTE	getLabel( UINT32 Index) { return Labels[Index];}

private:
	UINT32 Width;
	UINT32 Height;
	std::vector<std::vector<BYTE>>	Images;
	std::vector<BYTE>				Labels;
};


