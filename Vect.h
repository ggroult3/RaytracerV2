#pragma once
#include <iostream>
#include <math.h>
using namespace std;
class Vect
{
public:
	Vect(double x = 0, double y = 0, double z = 0);
	double operator[](int i) const;
	double& operator[](int i);
	double getSquarredNorm();
	double getNorm();
	Vect normalize();
	friend Vect operator+(const Vect& u, const Vect& v);
	friend Vect operator-(const Vect& u, const Vect& v);
	friend Vect operator-(const Vect& u);
	friend Vect operator*(double n, const Vect& u);
	friend Vect operator*(const Vect& u, double n);
	friend Vect operator*(const Vect& u, const Vect& v);
	friend Vect operator/(const Vect& u, double n);
	friend Vect cross(const Vect& u, const Vect& v);
	friend double dot(const Vect& u, const Vect& v);
	friend ostream& operator<<(ostream& sortie, Vect& u);
	friend Vect pow(const Vect& u, double n);


private :
	double coords[3];

};

