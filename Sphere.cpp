#include "Sphere.h"

Sphere::Sphere(const Vect& C, double R, const Vect& albedo) {
	center = C;
	rayon = R;
	this->albedo = albedo;
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

Vect& Sphere::getAlbedo()
{
	// Accesseur de l'albedo de la sphere
	return albedo;
}

bool Sphere::intersect(Ray& r,Vect& P,Vect& N)
{
	/* Determine s'il y a intersection en resolvant l'equation a*x^2 + b*x + c = 0
	* avec a = (r.getDirection)^2
	* b = 2 dot(r.getDirection,r.getOrigin - sphere.getCenter)
	* c = ((r.getOrigin - sphere.getCenter).getNorm)^2 - sphere.getRayon^2
	* Dans un premier temps, nous interessons au signe de delta pour determiner s'il y a intersection
	* Si delta >= 0, il y a au moins une intersection (maximum 2)
	* Dans un deuxieme temps, nous nous interessons aux racines pour determiner le point d'intersection
	* le plus proche de la camera.
	* Dans le cas ou les racines sont negatives, ca signifie que les intersection ont lieu derriere la camera
	* donc nous n'avons pas d'intersections visibles (donc on retourne false)
	*/
	bool hasIntersect = false;

	double a = 1; // car la direction du Ray r est normalisee
	double b = 2 * dot(r.getDirection(), r.getOrigin() - getCenter());
	double sqNorm1 = (r.getOrigin() - getCenter()).getSquarredNorm();
	double c = sqNorm1 - getRayon() * getRayon();
	
	double delta = b * b - 4 * a * c;//Discrimant

	if (delta >= 0) {
		double squareRootDelta = sqrt(delta);

		double racine1 = (-b - squareRootDelta) / 2 * a; // premiere racine
		double racine2 = (-b + squareRootDelta) / 2 * a; // seconde racine

		if (racine2 >= 0) { // La plus grande racine est positive ou nulle, donc intersection visible
			hasIntersect = true;

			double racine; // Le point d'intersection le plus proche correspond a la racine positive la plus petite
			if (racine1 > 0) { // racine1 etant la plus petite, si elle est positive c'est celle de notre point d'intersection 
				racine = racine1;
			}
			else { // Sinon c'est racine2;
				racine = racine2;
			}

			P = r.getOrigin() + racine * r.getDirection(); // Point d'intersection le plus proche de la camera
			N = (P - getCenter()).normalize(); // Vecteur normale (normee) a la sphere au point d'intersection P

		}
	}
	return hasIntersect;
}
