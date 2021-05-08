#include "Ray.h"

Ray::Ray(const Vect& O,const Vect& u) {
	origin = O;
	direction = u;
}

Vect& Ray::getOrigin() {
	// Accesseur de l'origine du rayon
	return origin;
}

Vect& Ray::getDirection() {
	// Accesseur de la direction du rayon
	return direction;
}