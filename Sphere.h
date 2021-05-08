#pragma once
#include "Vect.h"
#include "Ray.h"

class Sphere
{
public:
	Sphere(const Vect& C, double R);
	Vect& getCenter();
	double getRayon();
	bool intersect(Ray& r);

private:
	Vect center;
	double rayon;
};

