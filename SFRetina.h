#pragma once

class SFRetina
{
public:
	SFRetina(void);
	~SFRetina(void);

	void	initialize( HWND Handle, UINT32 Width );

	HDC startDraw( HDC DC );
	void endDraw();
	void moveTo(int X , int Y );
	void lineTo(int X , int Y );

	UINT32 getWidth() const { return Width; }
	std::vector<float>& getOutput();
	
private:
	HBITMAP Bitmap;
	UINT32 Width;
	RGBQUAD* PBits;
	HDC DrawDC;
	std::vector<float> Output;
};
