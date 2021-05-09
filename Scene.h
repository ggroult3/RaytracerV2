#pragma once
#include "Vect.h"
#include "Sphere.h"
#include <vector>
#include "Ray.h"
using namespace std;

class Scene
{
public:
	Scene();
	void setLightIntensity(double& I);
	void setLightOrigin(Vect& L);
	double getLightIntensity();
	Vect& getLightOrigin();
	void push(Sphere& object);
	int getObjectsSize();
	bool intersect(Ray& r, Vect& intersectionPoint, Vect& intersectionNormal, Vect& albedo,double& racine);
private:
	double lightIntensity;// Intensite lumineuse
	Vect lightOrigin; // Coordonnees de la lampe
	vector<Sphere> objects; // Contient les différents elements de la scene
};

