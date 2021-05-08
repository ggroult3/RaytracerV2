// main.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
using namespace std;

#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Vect.h"
#include <time.h>
#include "Ray.h"
#include "Sphere.h"



int main() {

	time_t beginTime, endTime; //Timecode de debut et de fin d'execution du code
	time(&beginTime); 
	cout << "Hello world!" << endl;

	int W = 512; // Largeur de l'image
	int H = 512; // Hauteur de l'image

	
	//* Test de la classe Vect
	Vect u(2, 3, 4);
	Vect v(4, 4, 4);
	Vect uPlusV = u + v;
	Vect uMinusV = u - v;
	Vect vNormalized = v.normalize();
	Vect vBy4 = v / 4.;
	Vect uCrossV = cross(u, v);
	cout << "u = " << u << endl;
	cout << "v = " << v << endl;
	cout << "u + v = " << uPlusV << endl;
	cout << "u - v = " << uMinusV << endl;
	cout << "norme de u = " << u.getNorm() << endl;
	cout << "norme de v = " << v.getNorm() << endl;
	cout << "v normalise = " << vNormalized << endl;
	cout << "v / 4 = " << vBy4 << endl;
	cout << "u ^ v = " << uCrossV << endl;
	cout << "u . v = " << dot(u, v) << endl;
	

	Vect CameraOrigin(0, 0, 55); 
	Vect SphereCenter(0, 0, 0);
	double rayon = 10;
	Sphere S(SphereCenter, rayon);

	double fieldOfView = 60 * M_PI / 180;


	vector<unsigned char> image(W * H * 3, 0);
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {

			Vect direction(j - W / 2, i - H / 2, -W / (2.*tan(fieldOfView / 2)));
			direction = direction.normalize(); // la direction du rayon doit toujours etre normee
			Ray r(CameraOrigin, direction); // Ray partant de la camera vers un pixel de l'image

			bool hasIntersect = S.intersect(r); // Determine si le Ray intersecte la sphere
			Vect color(0.,0.,0.); // Par defaut la couleur du pixel est noire
			if (hasIntersect) {
				color = Vect(21., 137., 10.); //S'il y a intersection sphere-ray, le pixel sera vert
			}

			// Affichage de l'intersection
			image[(i * W + j) * 3 + 0] = color[0]; // Rouge
			image[(i * W + j) * 3 + 1] = color[1]; // Vert
			image[(i * W + j) * 3 + 2] = color[2]; // Bleu
		}
	}

	// Ecriture de l'image sous format PNG
	stbi_write_png("Intersect.png", W, H, 3, &image[0], 0);
	time(&endTime);
	cout << "Image enregistree au format PNG au bout de " << difftime(endTime,beginTime) << " seconde(s) !" << endl;
	return 0;
}
// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
