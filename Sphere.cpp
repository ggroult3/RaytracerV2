#include "Sphere.h"

Sphere::Sphere(const Vect& C, double R) {
	center = C;
	rayon = R;
}

Vect& Sphere::getCenter()
{
	// Accesseur du centre de la sphere
	return center;
}

double Sphere::getRayon()
{
	// Accesseur du rayon de la sphere
	return rayon;
}

bool Sphere::intersect(Ray& r)
{
	/* Determine s'il y a intersection en resolvant l'equation a*t^2 + b*t + c = 0
	* avec a = (r.getDirection)^2
	* b = 2 dot(r.getDirection,r.getOrigin - sphere.getCenter)
	* c = ((r.getOrigin - sphere.getCenter).getNorm)^2 - sphere.getRayon^2
	* Dans un premier temps, nous interessons au signe de delta
	*/
	double a = 1; // car la direction du Ray r est normalisee
	double b = 2 * dot(r.getDirection(), r.getOrigin() - getCenter());
	double sqNorm1 = (r.getOrigin() - getCenter()).getSquarredNorm();
	double c = sqNorm1 - getRayon() * getRayon();
	
	double delta = b * b - 4 * a * c;//Discrimant

	if (delta >= 0) { 
		return true;
	}
	

	return false;
}
