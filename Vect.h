#pragma once
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>

using namespace std;

static default_random_engine engine(7697);
static uniform_real_distribution<double> uniform(0, 1);


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
	friend Vect generateRandomDirection(const Vect& intersectionNormal);


private :
	double coords[3];

};

