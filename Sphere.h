#pragma once
#include "Vect.h"
#include "Ray.h"
#include "Object.h"

class Sphere : public Object
{
public:
	Sphere(const Vect& C, double R,const Vect& albedo, bool mirror = false, bool transparent = false);
	Vect& getCenter();
	double getRayon();
	Vect& getAlbedo();
	bool getMirrorProperty();
	bool getTransparencyProperty();
	bool intersect(Ray& r,Vect& P, Vect& N,double& racine);

private:
	Vect center;
	double rayon;
	Vect albedo;
	bool isMirror,isTransparent;
};

