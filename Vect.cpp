#include "Vect.h"
#include <math.h>

Vect::Vect(double x, double y, double z) {
	coords[0] = x;
	coords[1] = y;
	coords[2] = z;
}

double Vect::operator[](int i) const {
	return coords[i];
}

double& Vect::operator[](int i) {
	return coords[i];
}

double Vect::getSquarredNorm()
{
	return coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2];
}

double Vect::getNorm() {
	return sqrt(getSquarredNorm());
}

Vect Vect::normalize() {
	double norm = this->getNorm();
	return Vect(coords[0] / norm, coords[1] / norm, coords[2] / norm);
}

Vect operator+(const Vect& u, const Vect& v) {
	/* Somme de deux vecteurs u et v : u + v
	*/
	return Vect(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

Vect operator-(const Vect& u, const Vect& v) {
	/* Soustraction de deux vecteurs u et v : u - v
	*/
	return Vect(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

Vect operator-(const Vect& u) {
	/* Renvoie le vecteur opposé a u : -u
	*/
	return Vect(-u[0], -u[1], -u[2]);
}

Vect operator*(double n, const Vect& u) {
	/* Produit du vecteur u par un scalaire n : n * u
	*/
	return Vect(n * u[0], n * u[1], n * u[2]);
}

Vect operator*(const Vect& u, double n) {
	/* Produit du vecteur u par un scalaire n : n * u
	*/
	return Vect(n * u[0], n * u[1], n * u[2]);
}

Vect operator*(const Vect& u, const Vect& v) {
	/* Produit terme à terme de deux vecteurs u et v : u_i * v_i pour tout i
	*/
	return Vect(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

Vect operator/(const Vect& u, double n) {
	/* Division du vecteur u par un scalaire n : u / n
	*/
	return Vect(u[0] / n, u[1] / n, u[2] / n);
}

Vect cross(const Vect& u, const Vect& v) {
	/* Produit vectoriel des vecteurs u et v : u ^ v
	*/
	return Vect(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0]);
}


double dot(const Vect& u, const Vect& v) {
	/* Produit scalaire des vecteurs u et v : u . v
	*/
	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

ostream& operator<< (ostream& sortie, Vect& u) {
	/* Affichage du vecteur u avec la méthode sortie de iostream (en general cout)
	*/
	sortie << "(" << u[0] << "," << u[1] << "," << u[2] << ")";
	return sortie;
}

Vect pow(const Vect& u, double n)
{
	/* Elevation a la puissance n du vecteur u
	*/
	return Vect(pow(u[0],n), pow(u[1], n), pow(u[2], n));
}

