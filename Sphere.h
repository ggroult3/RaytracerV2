#pragma once
#include "Vect.h"
#include "Ray.h"

class Sphere
{
public:
	Sphere(const Vect& C, double R,const Vect& albedo, bool mirror = false);
	Vect& getCenter();
	double getRayon();
	Vect& getAlbedo();
	bool getMirrorProperty();
	bool intersect(Ray& r,Vect& P, Vect& N,double& racine);

private:
	Vect center;
	double rayon;
	Vect albedo;
	bool isMirror;
};

