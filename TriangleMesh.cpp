#include "TriangleMesh.h"



TriangleMesh::~TriangleMesh() {}
TriangleMesh::TriangleMesh(const Vect albedo, bool mirror, bool transparent)
{
	this->albedo = albedo;
	isMirror = mirror;
	isTransparent = transparent;
}


Vect& TriangleMesh::getAlbedo()
{
	// Accesseur d'albedo
	return albedo;
}

bool TriangleMesh::getMirrorProperty()
{
	// Accesseur de la propriete miroir
	return isMirror;
}

bool TriangleMesh::getTransparencyProperty()
{
	// Accesseur de la propriete de transparence
	return isTransparent;
}

bool TriangleMesh::intersect(Ray& r, Vect& P, Vect& N, double& racine)
{
	// Determine si la maillage triangulaire est intersecte par le ray
	racine = 1E10;
	bool hasIntersect = false;

	/* Pour savoir si le ray intersecte le maillage, nous allons parcourir chaque triangle du maillage
	* et determiner (localement) s'il y a intersection triangle-ray.
	* Pour cela, nous devenons determiner s'il y a une intersection ray-plan avec le plan contenant le triangle
	* et verifier que le point d'intersection est a l'interieur du triangle grace aux coordonnes barycentriques.
	*/

	for (int i = 0; i < indices.size(); i++) {// Nous parcourons le maillage
		
		// Soient A, B et C les sommets du triangles
		const Vect& A = vertices[indices[i].vtxi];
		const Vect& B = vertices[indices[i].vtxj];
		const Vect& C = vertices[indices[i].vtxk];

		// Soient u et v deux vecteurs directeurs du plan contenant le triangle et w un vecteur normal
		Vect u = C - A; // = Vecteur AC
		Vect v = B - A; // = Vecteur AB
		Vect w = cross(u, v);

		// Soit O l'origine du ray et z son vecteur directeur 
		Vect O = r.getOrigin();
		Vect z = r.getDirection();

		/* Nous notons P le point d'intersection et N le vecteur normal au point P.
		* Nous savons que dot(PA,w) = 0 pour que P puisse appartenir au plan
		* et que P = O + racine * z pour appartenir au ray.
		* Apres developpement, nous avons donc racine = dot(OA,w) / dot(z,w)
		*/
		Vect OA = A - O;
		double localRacine = dot(OA, w) / dot(z, w);
		/* Cependant ça ne suffit pas, il faut que la racine soit positive et que le point d'intersection appartiennent bien au triangle
		* Calculons les coordonnées barycentriques
		* P = alpha * A + beta * B + gamma * C
		* De plus alpha + beta + gamma = 1
		* D'apres Moller-Trumbore, P = A + gamma * u + beta * v
		* Apres developpement, on a :
		* beta = dot(u, cross(OA,z)) / dot(z,w)
		* gamma = - dot(v,cross(OA,z)) / dot(z,w)
		*/
		double beta = dot(u, cross(OA, z)) / dot(z, w);
		double gamma = -dot(v, cross(OA, z)) / dot(z, w);
		double alpha = 1 - beta - gamma;
		if (alpha >= 0 && beta >= 0 && gamma >= 0 && beta <= 1 && gamma <= 1 && localRacine > 0) {
			hasIntersect = true;
			if (localRacine < racine) {
				racine = localRacine;
				P = O + racine * z;
				N = w;
			}
		}
	}


	return hasIntersect;
}

void TriangleMesh::readOBJ(const char* obj) {

	char matfile[255];
	char grp[255];

	FILE* f;
	f = fopen(obj, "r");
	int curGroup = -1;
	while (!feof(f)) {
		char line[255];
		if (!fgets(line, 255, f)) break;

		std::string linetrim(line);
		linetrim.erase(linetrim.find_last_not_of(" \r\t") + 1);
		strcpy(line, linetrim.c_str());

		if (line[0] == 'u' && line[1] == 's') {
			sscanf(line, "usemtl %[^\n]\n", grp);
			curGroup++;
		}

		if (line[0] == 'v' && line[1] == ' ') {
			Vect vec;

			Vect col;
			if (sscanf(line, "v %lf %lf %lf %lf %lf %lf\n", &vec[0], &vec[1], &vec[2], &col[0], &col[1], &col[2]) == 6) {
				col[0] = std::min(1., std::max(0., col[0]));
				col[1] = std::min(1., std::max(0., col[1]));
				col[2] = std::min(1., std::max(0., col[2]));

				vertices.push_back(vec);
				vertexcolors.push_back(col);

			}
			else {
				sscanf(line, "v %lf %lf %lf\n", &vec[0], &vec[1], &vec[2]);
				vertices.push_back(vec);
			}
		}
		if (line[0] == 'v' && line[1] == 'n') {
			Vect vec;
			sscanf(line, "vn %lf %lf %lf\n", &vec[0], &vec[1], &vec[2]);
			normals.push_back(vec);
		}
		if (line[0] == 'v' && line[1] == 't') {
			Vect vec;
			sscanf(line, "vt %lf %lf\n", &vec[0], &vec[1]);
			uvs.push_back(vec);
		}
		if (line[0] == 'f') {
			TriangleIndices t;
			int i0, i1, i2, i3;
			int j0, j1, j2, j3;
			int k0, k1, k2, k3;
			int nn;
			t.group = curGroup;

			char* consumedline = line + 1;
			int offset;

			nn = sscanf(consumedline, "%u/%u/%u %u/%u/%u %u/%u/%u%n", &i0, &j0, &k0, &i1, &j1, &k1, &i2, &j2, &k2, &offset);
			if (nn == 9) {
				if (i0 < 0) t.vtxi = vertices.size() + i0; else	t.vtxi = i0 - 1;
				if (i1 < 0) t.vtxj = vertices.size() + i1; else	t.vtxj = i1 - 1;
				if (i2 < 0) t.vtxk = vertices.size() + i2; else	t.vtxk = i2 - 1;
				if (j0 < 0) t.uvi = uvs.size() + j0; else	t.uvi = j0 - 1;
				if (j1 < 0) t.uvj = uvs.size() + j1; else	t.uvj = j1 - 1;
				if (j2 < 0) t.uvk = uvs.size() + j2; else	t.uvk = j2 - 1;
				if (k0 < 0) t.ni = normals.size() + k0; else	t.ni = k0 - 1;
				if (k1 < 0) t.nj = normals.size() + k1; else	t.nj = k1 - 1;
				if (k2 < 0) t.nk = normals.size() + k2; else	t.nk = k2 - 1;
				indices.push_back(t);
			}
			else {
				nn = sscanf(consumedline, "%u/%u %u/%u %u/%u%n", &i0, &j0, &i1, &j1, &i2, &j2, &offset);
				if (nn == 6) {
					if (i0 < 0) t.vtxi = vertices.size() + i0; else	t.vtxi = i0 - 1;
					if (i1 < 0) t.vtxj = vertices.size() + i1; else	t.vtxj = i1 - 1;
					if (i2 < 0) t.vtxk = vertices.size() + i2; else	t.vtxk = i2 - 1;
					if (j0 < 0) t.uvi = uvs.size() + j0; else	t.uvi = j0 - 1;
					if (j1 < 0) t.uvj = uvs.size() + j1; else	t.uvj = j1 - 1;
					if (j2 < 0) t.uvk = uvs.size() + j2; else	t.uvk = j2 - 1;
					indices.push_back(t);
				}
				else {
					nn = sscanf(consumedline, "%u %u %u%n", &i0, &i1, &i2, &offset);
					if (nn == 3) {
						if (i0 < 0) t.vtxi = vertices.size() + i0; else	t.vtxi = i0 - 1;
						if (i1 < 0) t.vtxj = vertices.size() + i1; else	t.vtxj = i1 - 1;
						if (i2 < 0) t.vtxk = vertices.size() + i2; else	t.vtxk = i2 - 1;
						indices.push_back(t);
					}
					else {
						nn = sscanf(consumedline, "%u//%u %u//%u %u//%u%n", &i0, &k0, &i1, &k1, &i2, &k2, &offset);
						if (i0 < 0) t.vtxi = vertices.size() + i0; else	t.vtxi = i0 - 1;
						if (i1 < 0) t.vtxj = vertices.size() + i1; else	t.vtxj = i1 - 1;
						if (i2 < 0) t.vtxk = vertices.size() + i2; else	t.vtxk = i2 - 1;
						if (k0 < 0) t.ni = normals.size() + k0; else	t.ni = k0 - 1;
						if (k1 < 0) t.nj = normals.size() + k1; else	t.nj = k1 - 1;
						if (k2 < 0) t.nk = normals.size() + k2; else	t.nk = k2 - 1;
						indices.push_back(t);
					}
				}
			}

			consumedline = consumedline + offset;

			while (true) {
				if (consumedline[0] == '\n') break;
				if (consumedline[0] == '\0') break;
				nn = sscanf(consumedline, "%u/%u/%u%n", &i3, &j3, &k3, &offset);
				TriangleIndices t2;
				t2.group = curGroup;
				if (nn == 3) {
					if (i0 < 0) t2.vtxi = vertices.size() + i0; else	t2.vtxi = i0 - 1;
					if (i2 < 0) t2.vtxj = vertices.size() + i2; else	t2.vtxj = i2 - 1;
					if (i3 < 0) t2.vtxk = vertices.size() + i3; else	t2.vtxk = i3 - 1;
					if (j0 < 0) t2.uvi = uvs.size() + j0; else	t2.uvi = j0 - 1;
					if (j2 < 0) t2.uvj = uvs.size() + j2; else	t2.uvj = j2 - 1;
					if (j3 < 0) t2.uvk = uvs.size() + j3; else	t2.uvk = j3 - 1;
					if (k0 < 0) t2.ni = normals.size() + k0; else	t2.ni = k0 - 1;
					if (k2 < 0) t2.nj = normals.size() + k2; else	t2.nj = k2 - 1;
					if (k3 < 0) t2.nk = normals.size() + k3; else	t2.nk = k3 - 1;
					indices.push_back(t2);
					consumedline = consumedline + offset;
					i2 = i3;
					j2 = j3;
					k2 = k3;
				}
				else {
					nn = sscanf(consumedline, "%u/%u%n", &i3, &j3, &offset);
					if (nn == 2) {
						if (i0 < 0) t2.vtxi = vertices.size() + i0; else	t2.vtxi = i0 - 1;
						if (i2 < 0) t2.vtxj = vertices.size() + i2; else	t2.vtxj = i2 - 1;
						if (i3 < 0) t2.vtxk = vertices.size() + i3; else	t2.vtxk = i3 - 1;
						if (j0 < 0) t2.uvi = uvs.size() + j0; else	t2.uvi = j0 - 1;
						if (j2 < 0) t2.uvj = uvs.size() + j2; else	t2.uvj = j2 - 1;
						if (j3 < 0) t2.uvk = uvs.size() + j3; else	t2.uvk = j3 - 1;
						consumedline = consumedline + offset;
						i2 = i3;
						j2 = j3;
						indices.push_back(t2);
					}
					else {
						nn = sscanf(consumedline, "%u//%u%n", &i3, &k3, &offset);
						if (nn == 2) {
							if (i0 < 0) t2.vtxi = vertices.size() + i0; else	t2.vtxi = i0 - 1;
							if (i2 < 0) t2.vtxj = vertices.size() + i2; else	t2.vtxj = i2 - 1;
							if (i3 < 0) t2.vtxk = vertices.size() + i3; else	t2.vtxk = i3 - 1;
							if (k0 < 0) t2.ni = normals.size() + k0; else	t2.ni = k0 - 1;
							if (k2 < 0) t2.nj = normals.size() + k2; else	t2.nj = k2 - 1;
							if (k3 < 0) t2.nk = normals.size() + k3; else	t2.nk = k3 - 1;
							consumedline = consumedline + offset;
							i2 = i3;
							k2 = k3;
							indices.push_back(t2);
						}
						else {
							nn = sscanf(consumedline, "%u%n", &i3, &offset);
							if (nn == 1) {
								if (i0 < 0) t2.vtxi = vertices.size() + i0; else	t2.vtxi = i0 - 1;
								if (i2 < 0) t2.vtxj = vertices.size() + i2; else	t2.vtxj = i2 - 1;
								if (i3 < 0) t2.vtxk = vertices.size() + i3; else	t2.vtxk = i3 - 1;
								consumedline = consumedline + offset;
								i2 = i3;
								indices.push_back(t2);
							}
							else {
								consumedline = consumedline + 1;
							}
						}
					}
				}
			}

		}

	}
	fclose(f);

}
