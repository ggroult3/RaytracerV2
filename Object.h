#pragma once

#include "Vect.h"
#include "Ray.h"

class Object
{
public:
	Object();
	virtual Vect& getAlbedo() = 0;
	virtual bool getMirrorProperty() = 0;
	virtual bool getTransparencyProperty() = 0;
	virtual bool intersect(Ray& r, Vect& P, Vect& N, double& racine) = 0;

private:
	Vect albedo;
	bool isMirror, isTransparent;
};

