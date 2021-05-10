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

Vect generateRandomDirection(const Vect& intersectionNormal)
{
	/* Renvoie une direction generee aleatoirement.
	* Pour cela, nous echantillonnons le facteur en cos(theta_i) de la BRDF a partir de deux nombres aleatoires
	* u1 et u2 suivant la loi uniforme U[0,1].
	* Nous obtenons donc les coordonnees suivantes : 
	* x = cos(2 * pi * u1) * sqrt(1 - u2)
	* y = sin(2 * pi * u1) * sqrt(1 - u2)
	* z = sqrt(u2)
	* Ensuite nous construisons un repere via des vecteurs perpendiculaires au vecteur normale
	* et nous effecturons un changement de repere
	*/

	double u1 = uniform(engine); // echantillon suivant la loi uniforme U[0,1]
	double u2 = uniform(engine); // echantillon suivant la loi uniforme U[0,1]

	// Calcul des coordonnees
	double x = cos(2 * M_PI * u1) * sqrt(1 - u2);
	double y = sin(2 * M_PI * u1) * sqrt(1 - u2);
	double z = sqrt(u2);

	// Construction du repere
	Vect ez = intersectionNormal; // Vecteur normal unitaire

	Vect ex; // Premier vecteur perpendiculaire au vecteur normal
	// Pour cela, nous annulons la plus petite des coordonees du vecteur normal,
	// nous inversons les deux autres coordonnes et nous ajoutons un signe a l'une des deux coordonnees restante
	// Exemple : Soit N(xn, yn, zn) le vecteur normal avec min(xn, yn, zn) = xn
	// Un vecteur perpendiculaire à N est u(0, zn, -yn)
	
	if (ez[0] < ez[1] && ez[0] < ez[2]) { // Le minimum est sur la premiere composante
		ex = Vect(0, ez[2], -ez[1]);
	}
	else {
		if (ez[1] < ez[0] && ez[1] < ez[2]) { // Le minimum est sur la deuxieme composante
			ex = Vect(ez[2], 0, -ez[0]);
		}
		else {  // Le minimum est sur la troisieme composante
			ex = Vect(ez[1], -ez[0], 0);
		}
	}
	ex = ex.normalize(); // ex doit etre unitaire

	Vect ey = cross(ez, ex); // Premier vecteur perpendiculaire au vecteur normal. Ce vecteur est unitaire

	Vect randomDirection = x * ex + y * ey + z * ez; // Changement de repere
	// randomDirection = randomDirection.normalize(); // La direction doit etre unitaire

	return randomDirection;
}

