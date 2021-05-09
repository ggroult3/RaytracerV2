#include "Scene.h"

Scene::Scene()
{

}

void Scene::setLightIntensity(double& I)
{
	// Mutateur de l'intensité lumineuse
	lightIntensity = I;
}

void Scene::setLightOrigin(Vect& L)
{
	// Mutateur de la position de la lampe
	lightOrigin = L;
}

double Scene::getLightIntensity()
{
	// Accesseur de l'intensité lumineuse
	return lightIntensity;
}

Vect& Scene::getLightOrigin()
{
	// Accesseur de la position de la lampe
	return lightOrigin;
}

void Scene::push(Sphere& object)
{
	// Ajoute un object comme element de la scene
	objects.push_back(object);
}

int Scene::getObjectsSize()
{
	// Accesseur de la taille du vecteur contenant les elements de la scene
	return objects.size();
}

bool Scene::intersect(Ray& r, Vect& intersectionPoint, Vect& intersectionNormal, Vect& albedo)
{
	/* Determine ou le ray intersecte un element de la scene par le calcul du point d'intersection
	* et la normale a la sphere au point d'intersection.
	* Dans le cas ou le ray intersecte plusieurs elements de la scene,
	* nous nous interessons au point d'intersection le plus proche de la camera
	*/
	double racine = 1E10; // Initialisation a tres grande valeur pour obtenir la racine positive la plus proche de zero par la suite
	bool hasIntersect = false;

	for (int i = 0; i < getObjectsSize(); i++) { // Nous parcourons les differents elements de la scene pour savoir s'il y a une intersection
		
		Vect localIntersectionPoint, localIntersectionNormal; // Point d'intersection avec le ieme objet et vecteur normale a ce point d'intersection
		double localRacine; // Racine obtenue lors de l'intersection du ray avec le ieme objet
		bool hasLocalIntersect = objects[i].intersect(r, localIntersectionPoint, localIntersectionNormal,localRacine); // Determination de l'intersection ray-objet avec le 
		if (hasLocalIntersect && localRacine < racine) { // S'il y a intersection avec le ieme objet et que la racine obtenue est inferieure a celle obtenue precedemment alors le ieme objet est plus de la camera
			racine = localRacine; // Nous nous conservons la racine obtenue lors cette intersection 
			intersectionPoint = localIntersectionPoint; // ainsi que le point d'intersection determine
			intersectionNormal = localIntersectionNormal; // et la normale a ce point d'intersection
			if (!hasIntersect) { // S'il n'y a pas eu d'intersection avant, on annonce qu'il y en a une
				hasIntersect = true;
			}
			albedo = objects[i].getAlbedo(); // La couleur du pixel sera lie a l'albedo du ieme objet
		}
	}
	return hasIntersect;
}
