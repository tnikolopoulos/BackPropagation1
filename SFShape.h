#pragma once

class SFShape
{
public:
	SFShape(void);
	~SFShape(void);

	void draw(HDC dc );
	void addLine( UINT32 X1 , UINT32 Y1 , UINT32 X2 , UINT32 Y2 ) { RECT R ={X1,Y1,X2,Y2}; Lines.push_back(R); }
		
	void setPos( float PX , float PY ) {  X = PX ; Y = PY ;}
	void setRotation( float A ) {Angle = A; }
	void transform( float& PX , float& PY );
	std::vector<float>& getName() { return Name;}
	
	float					X,Y;
	float					Angle;
	std::vector<RECT>		Lines;
	std::vector<float>		Name;
};
