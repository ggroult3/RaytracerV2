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



void estimateCosIntegral(int power = 1) {
	/* Nous cherchons a estimer l'integrale de cos^power sur l'intervalle [-pi/2;pi/2] par la methode de Monte-Carlo
	* La methode consiste a echantillonner aleatoirement la fonction cos^10 ensuite suivant une loi de probabilite p.
	* Ainsi on peut approximer par : Integrale(cos^power(x) dx) = 1/N Somme(cos^power(x_i) / p(x_i), i allant de 0 à N)
	* N etant le nombre d'echantillon
	* Dans notre cas, p suivra une loi normale d'esperance mu = 0 et d'ecart-type sigma = 0.25
	* 
	* Pour obtenir des echantillons x et y qui suivent une loi normale, nous faisons deux tirages u1 et u2 selon la loi uniforme U[0,1]
	* et nous construisons les echantillons à partir de ces tirages via les formules de Box-Muller :
	* x = sigma * sqrt(-2*log(u1)) * cos(2*pi*u2)
	* y = sigma * sqrt(-2*log(u1)) * sin(2*pi*u2)
	*/
	int sampleSize = 100000; // Taille d'echantillon
	double sigma = 0.25; // Ecart-type de la loi normale centree
	double sum = 0; // Somme approximant l'integrale de cos^10(x)
	for (int i = 0; i < sampleSize; i++) {
		double u1 = uniform(engine); // echantillon suivant la loi uniforme U[0,1]
		double u2 = uniform(engine); // echantillon suivant la loi uniforme U[0,1]
		double xi = sigma * sqrt(-2 * log(u1)) * cos(2 * M_PI * u2); // echantillon gaussien détermine par la transformation de Box-Muller
		double p_xi = 1 / (sigma * sqrt(2 * M_PI)) * exp(-xi * xi / (2 * sigma * sigma)); // probabilite de xi
		if (xi > -M_PI / 2 && xi < M_PI / 2) { // Nous verifions que les echantillons sont bien dans le bon intervalle ([-pi/2;pi/2])
			sum += pow(cos(xi), power) / p_xi / sampleSize;
		}

	}
	cout << sum << endl;
}

void estimateCosDoubleIntegral(int power = 1) {
	/* Nous cherchons a estimer l'integrale de cos^power(x*y) sur l'intervalle [-pi/2;pi/2] pour x et y par la methode de Monte-Carlo
	* La methode consiste a echantillonner aleatoirement la fonction cos^power ensuite suivant une loi de probabilite p.
	* Ainsi on peut approximer par : Integrale(cos^power(x+y) dx) = 1/N Somme(f(x_i + y_i) / p(x_i) / p(y_i), i allant de 0 à N)
	* N etant le nombre d'echantillon
	* Dans notre cas, p suivra une loi normale d'esperance mu = 0 et d'ecart-type sigma = 0.25
	*
	* Pour obtenir des echantillons x et y qui suivent une loi normale, nous faisons deux tirages u1 et u2 selon la loi uniforme U[0,1]
	* et nous construisons les echantillons à partir de ces tirages via les formules de Box-Muller :
	* x = sigma * sqrt(-2*log(u1)) * cos(2*pi*u2)
	* y = sigma * sqrt(-2*log(u1)) * sin(2*pi*u2)
	*/
	int sampleSize = 100000; // Taille d'echantillon
	double sigma = 0.25; // Ecart-type de la loi normale centree
	double sum = 0; // Somme approximant l'integrale de cos^10(x)
	for (int i = 0; i < sampleSize; i++) {
		double u1 = uniform(engine); // echantillon suivant la loi uniforme U[0,1]
		double u2 = uniform(engine); // echantillon suivant la loi uniforme U[0,1]
		double xi = sigma * sqrt(-2 * log(u1)) * cos(2 * M_PI * u2); // echantillon gaussien détermine par la transformation de Box-Muller
		double yi = sigma * sqrt(-2 * log(u1)) * sin(2 * M_PI * u2); // echantillon gaussien détermine par la transformation de Box-Muller
		double p_xi = 1 / (sigma * sqrt(2 * M_PI)) * exp(-xi * xi / (2 * sigma * sigma)); // probabilite de xi
		double p_yi = 1 / (sigma * sqrt(2 * M_PI)) * exp(-yi * yi / (2 * sigma * sigma)); // probabilite de xi
		double p = p_xi * p_yi;
		if (xi > -M_PI / 2 && xi < M_PI / 2 && yi > -M_PI / 2 && yi < M_PI / 2) {// Nous verifions que les echantillons sont bien dans le bon intervalle ([-pi/2;pi/2])
			sum += pow(cos(xi + yi), power) / p / sampleSize;
		}
	}
	cout << sum << endl;
}

int main() {

	time_t beginTime, endTime; //Timecode de debut et de fin d'execution du code
	time(&beginTime); 
	cout << "Start !" << endl;

	// Estimation d'integrale par Monte-Carlo
	/*
	estimateCosIntegral(10); // Reponse attendue : 0.773126 / Reponse obtenue : 0.773862
	estimateCosIntegral(2); // Reponse attendue : 1.5708 / Reponse obtenue : 1.56931
	estimateCosDoubleIntegral(2); // Reponse attendue : 4.9348 / Reponse obtenue : 2.47889
	*/


	int W = 512; // Largeur de l'image
	int H = 512; // Hauteur de l'image

	int ratioRayPerPixel = 100; // Nombre de rays envoyes pour chaque pixel (pour l'eclairage indirecte)
	
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

	bool useGammaCorrection = true; // true : Activation de la correction Gamma
	bool useAntiAliasing = false; // true : Activation de l'anti-crenelage
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
	Sphere lumiere(scene.getLightOrigin(), 5, Vect(255., 255., 255.)); // Lampe spherique
	Sphere Sdiffuse(Vect(-20, 0, 0), 10, Vect(21., 137., 10.)); // Sphere principale verte
	Sphere Smiroir(Vect(20, 0, 0), 10, Vect(21., 137., 10.), true); // Sphere principale miroir
	Sphere Stransparente(Vect(0, 0, 0), 10, Vect(21., 137., 10.), false, true); // Sphere principale transparente
	Sphere sol(Vect(0, -1000, 0), 990, Vect(255., 0., 0.));
	Sphere plafond(Vect(0, 1000, 0), 940, Vect(0., 0., 255.));
	Sphere murGauche(Vect(-1000, 0, 0), 940, Vect(255., 0., 255.));
	Sphere murDroite(Vect(1000, 0, 0), 940, Vect(255., 255., 255.));
	Sphere murAvant(Vect(0, 0, -1000), 940, Vect(255., 255., 0.));
	Sphere murArriere(Vect(0, 0, 1000), 940, Vect(0., 255., 0.));

	//Ajout des elements de la scene
	scene.push(lumiere);
	scene.push(Sdiffuse);
	scene.push(Smiroir);
	scene.push(Stransparente);
	scene.push(sol);
	scene.push(plafond);
	scene.push(murGauche);
	scene.push(murDroite);
	scene.push(murAvant);
	scene.push(murArriere);

	

	vector<unsigned char> image(W * H * 3, 0);

	// Parallelisation des operations
#pragma omp parallel for schedule(dynamic,1)
	for (int i = 0; i < H; i++) {
		if (i % 50 == 0) {
			cout << i << endl;
		}
		for (int j = 0; j < W; j++) {

			// Application de l'anticrenelage
			// Pour cela, nous pertubons la direction des rays emis au lieu de les emettre directement au centre du pixel
			// afin de couvrir la totalite de la surface du pixel.
			// Nous echantillons selon la loi uniforme U[0,1]
			// et nous appliquons la transformation de Box-Muller pour obtenir les coordonnees generees aleatoirement selon un loi normale centree
			// d'ecart-type sigma = 0.25 (comme lors de l'estimation d'integrale par Monte-Carlo).

			Vect color(0., 0., 0.); // Par defaut le pixel est noir

			if (useAntiAliasing) { // Application de l'anti-crenelage
				double sigma = 0.25; // Ecart-type de la loi normale
				for (int k = 0; k < ratioRayPerPixel; k++) {
					double u1 = uniform(engine); // echantillon suivant la loi uniforme U[0,1]
					double u2 = uniform(engine); // echantillon suivant la loi uniforme U[0,1]

					// Calcul des coordonnees aleatoires suivant la loi normale centree d'ecart-type sigma
					// via la transformation de Box-Muller
					double x = sigma * cos(2 * M_PI * u1) * sqrt(-2 * log(u2));
					double y = sigma * sin(2 * M_PI * u1) * sqrt(-2 * log(u2));

					// Calcul du ray envoye par la camera
					// La direction du ray est pertubee par l'ajout du vecteur (x, y, 0)
					Vect disturbedDirection(j - W / 2 + x + 0.5, i - H / 2 + y + 0.5, -W / (2. * tan(fieldOfView / 2)));
					disturbedDirection = disturbedDirection.normalize(); // la direction du rayon doit toujours etre normee
					Ray disturbedRay(cameraOrigin, disturbedDirection); // Ray partant de la camera vers une zone voisine au centre du pixel de l'image

					// Calcul de la couleur du pixel
					// Pour observer l'effet de l'eclairage indirecte, nous envoyons un certain nombre de rays pour chaque pixel de l'image
					// et nous calculons la couleur moyenne
					Vect colorContribution = scene.estimatePixelColor(disturbedRay, 0);
					color = color + colorContribution;
				}
			}
			else { // Pas d'anti-crenelage
				// Calcul du ray envoye par la camera
				Vect direction(j - W / 2, i - H / 2, -W / (2. * tan(fieldOfView / 2)));
				direction = direction.normalize(); // la direction du rayon doit toujours etre normee
				Ray ray(cameraOrigin, direction); // Ray partant de la camera vers le centre du pixel de l'image

				// Calcul de la couleur du pixel
				// Pour observer l'effet de l'eclairage indirecte, nous envoyons un certain nombre de rays pour chaque pixel de l'image
				// et nous calculons la couleur moyenne
				for (int k = 0; k < ratioRayPerPixel; k++) {
					Vect colorContribution = scene.estimatePixelColor(ray, 0);
					color = color + colorContribution;
				}
			}
			
			color = color / ratioRayPerPixel; // couleur moyenne

			//Vect color = scene.estimatePixelColor(ray, 0);

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
	stbi_write_png("OmbreDouce.png", W, H, 3, &image[0], 0);
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
