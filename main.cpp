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
#include <algorithm>
#include "Scene.h"



int main() {

	time_t beginTime, endTime; //Timecode de debut et de fin d'execution du code
	time(&beginTime); 
	cout << "Start !" << endl;

	int W = 512; // Largeur de l'image
	int H = 512; // Hauteur de l'image
	
	/* Test de la classe Vect
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
	*/

	Vect cameraOrigin(0, 0, 55); // Coordonnees de la camera

	double fieldOfView = 60 * M_PI / 180; // Champ de vision

	bool useGammaCorrection = true;
	double lightIntensity;

	if (useGammaCorrection) {
		lightIntensity = 5E9; // Intensité lumineuse avec correction gamma
	}
	else {
		lightIntensity = 1E7; // Intensite lumineuse sans correction gamma
	}

	
	Vect lightOrigin(-10, 20, 40); // Coordonnees de la lampe

	//Definition de la scene
	Scene scene;
	scene.setLightIntensity(lightIntensity);
	scene.setLightOrigin(lightOrigin);

	//Declaration des elements de la scene
	Sphere S(Vect(0, 0, 0), 10, Vect(21., 137., 10.), false, true); // Sphere principale transparente
	Sphere sol(Vect(0, -1000, 0), 990, Vect(255., 0., 0.));
	Sphere plafond(Vect(0, 1000, 0), 940, Vect(0., 0., 255.));
	Sphere murGauche(Vect(-1000, 0, 0), 940, Vect(255., 0., 255.));
	Sphere murDroite(Vect(1000, 0, 0), 940, Vect(255., 255., 255.));
	Sphere murAvant(Vect(0, 0, -1000), 940, Vect(255., 255., 0.));
	Sphere murArriere(Vect(0, 0, 1000), 940, Vect(0., 255., 0.));

	//Ajout des elements de la scene
	scene.push(S);
	scene.push(sol);
	scene.push(plafond);
	scene.push(murGauche);
	scene.push(murDroite);
	scene.push(murAvant);
	scene.push(murArriere);


	vector<unsigned char> image(W * H * 3, 0);
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			
			// Calcul du ray envoye par la camera
			Vect direction(j - W / 2, i - H / 2, -W / (2.*tan(fieldOfView / 2)));
			direction = direction.normalize(); // la direction du rayon doit toujours etre normee
			Ray ray(cameraOrigin, direction); // Ray partant de la camera vers un pixel de l'image

			Vect color = scene.estimatePixelColor(ray, 0); // Calcul de la couleur du pixel			

			// Ajustement de la couleur
			// On prend le minimum entre la couleur determinee et 255 afin d'eviter un depassement arithmetique
			// Si on utilise la correction du Gamma, la valeur de la couleur est elevee a la puissance 0.45
			if (useGammaCorrection) { // Cas ou la correction Gamma est appliquee
				color[0] = min(255., pow(color[0],0.45)); // Rouge
				color[1] = min(255., pow(color[1], 0.45)); // Vert
				color[2] = min(255., pow(color[2], 0.45)); // Bleu
			}
			else { // Pas de correction Gamma
				color[0] = min(255.,color[0]); // Rouge
				color[1] = min(255., color[1]); // Vert
				color[2] = min(255., color[2]); // Bleu
			}

			// Ecriture de la couleur du pixel dans la matrice image
			// On inverse l'image en remplacant i * W + j par ((H - i - 1) * W + j)
			image[((H - i - 1) * W + j) * 3 + 0] = color[0]; // Rouge
			image[((H - i - 1) * W + j) * 3 + 1] = color[1]; // Vert
			image[((H - i - 1) * W + j) * 3 + 2] = color[2]; // Bleu
		}
	}

	// Ecriture de l'image sous format PNG
	stbi_write_png("Transparent.png", W, H, 3, &image[0], 0);
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
