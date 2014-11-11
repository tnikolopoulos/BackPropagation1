#pragma once

class SFUnit
{
public:
	SFUnit(void);
	~SFUnit(void);

	void	initialize( UINT32 Width , UINT32 Height );
	float	computeOutput( std::vector<float>& Input );
	std::vector<float>&	getWeights() { return Weights;}
	std::vector<float>& getDeltaWeights() { return DeltaWeights;}

	float getOutput() { return Output;}


private:
	std::vector<float>	Weights;
	std::vector<float>	DeltaWeights;
	float Output ; 
};
