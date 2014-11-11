#pragma once

class SFNetwork;
class SFLayer;

class SFNetworkView
{
public:
	SFNetworkView(void);
	~SFNetworkView(void);

	void initialize( HWND Handle);
	void updateView();
	void setNetwork( SFNetwork* Net ) { PNetwork = Net; }
	
	bool getUpdateDisplay() const { return UpdateDisplay; }
	void setUpdateDisplay(const bool& val) { UpdateDisplay = val; }
	void handleResize();

	void handleLeftButtonDown( INT32 X , INT32 Y);


private:
	void displayReconstruction(HDC DC , SFLayer& Layer , UINT64* Output , UINT32 SX , UINT32 SY );
	void displayWeights(HDC DC , SFLayer& Layer , UINT32 SX , UINT32 SY );
	void displayOutput( HDC DC , SFLayer& Layer , UINT32 SX , UINT32 SY );
	bool		UpdateDisplay;
	SFNetwork*	PNetwork;

	HWND		Handle;
	RECT		ClientRect;
	HBITMAP		BackBuffer;
	float		Scale;
	INT32		SelectedLayer;
};
