#pragma once

#include "SFRetina.h"
#include "SFShape.h"
#include "SFMNISTLoader.h"

class SFLayer;

class SFNetwork
{
public:
	SFNetwork(void);
	~SFNetwork(void);

	void initialize();
	void run();
	
	std::vector<SFLayer>&	getLayers() { return Layers;}
	SFRetina&	getRetina() { return Retina;}

	SFShape*		getShape() {return PShape;}

	bool getLearn() const { return Learn; }
	void setLearn(const bool& val) { Learn = val; }

	bool getTrain() const { return Train; }
	void setTrain(const bool& val) { Train = val; }
	void changeShape();

private:
	std::vector<SFLayer>	Layers;
	SFMNISTLoader			MNISTLoader;
	SFRetina				Retina;
	SFShape					ShapeX ;
	SFShape					ShapeTri;
	SFShape*				PShape;
	bool					Train;
	bool					Learn;
	UINT32					TrainCount;
	UINT32					MNISTIndex;
};
