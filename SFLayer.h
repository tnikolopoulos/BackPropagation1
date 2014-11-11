#pragma once

#include "SFUnit.h"

class SFLayer
{
public:
	SFLayer(void);
	~SFLayer(void);

	void					initialize( UINT32 PWidth , UINT32 PHeight , UINT32 InputWidth , UINT32 InputHeight);
	void					feed(std::vector<float>& Input);
	std::vector<float>&		getOutput() { return Output;}

	UINT32					getWidth() { return Width;}
	UINT32					getHeight() { return Height;}

	INT32 getSelectedUnit() const { return SelectedUnit; }
	void setSelectedUnit(const INT32& val) { SelectedUnit = val; }

	std::vector<SFUnit>&	getUnits() { return Units;}
	
	UINT32					getInputWidth() { return InputWidth;}
	UINT32					getInputHeight() { return InputHeight;}
	std::vector<float>		calculateError(std::vector<float>& Target);
	std::vector<float>&		propagateError( std::vector<float>& Error );
	void					applyWeightChanges();
private:
	INT32				SelectedUnit;
	std::vector<SFUnit>	Units;
	std::vector<float> Output;
	std::vector<float>* LastInput;
	std::vector<float>	InputError;
	UINT32 Width , Height;
	UINT32 InputWidth , InputHeight;
};
