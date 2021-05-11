#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
#include <string>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include "Vect.h"
#include "TriangleIndices.h"
#include "Object.h"
#include "BoiteEnglobante.h"

using namespace std;

class TriangleMesh : public Object {
public:
	~TriangleMesh();
	TriangleMesh(const Vect albedo, bool mirror = false, bool transparent = false);
	Vect& getAlbedo();
	bool getMirrorProperty();
	bool getTransparencyProperty();
	bool intersect(Ray& r, Vect& P, Vect& N, double& racine);


	void readOBJ(const char* obj); 

	void buildHitBox();
	BoiteEnglobante getHitBox();

	vector<TriangleIndices> indices;
	vector<Vect> vertices;
	vector<Vect> normals;
	vector<Vect> uvs;
	vector<Vect> vertexcolors;

private:
	Vect albedo;
	bool isMirror, isTransparent;
	BoiteEnglobante hitBox;

};
