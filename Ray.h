#pragma once
#include "Vect.h"
#include <math.h>

class Ray{
public:
	Ray(const Vect& O,const Vect& u);
	Vect& getOrigin();
	Vect& getDirection();
	void setOrigin(Vect& O);
	void setDirection(Vect& u);
private:
	Vect origin;
	Vect direction;
};

