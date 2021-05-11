#pragma once

#include "Vect.h"
#include "Ray.h"
class BoiteEnglobante
{
public:
	bool intersect(Ray& r);
	Vect& getBorneSup();
	Vect& getBorneInf();
	void setBorneSup(Vect& u);
	void setBorneInf(Vect& u);
private:
	Vect borneInf, borneSup;
};

