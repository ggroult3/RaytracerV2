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

bool Scene::intersect(Ray& r, Vect& intersectionPoint, Vect& intersectionNormal, Vect& albedo,double& racine, bool& isMirror)
{
	/* Determine ou le ray intersecte un element de la scene par le calcul du point d'intersection
	* et la normale a la sphere au point d'intersection.
	* Dans le cas ou le ray intersecte plusieurs elements de la scene,
	* nous nous interessons au point d'intersection le plus proche de la camera
	*/
	racine = 1E10; // Initialisation a tres grande valeur pour obtenir la racine positive la plus proche de zero par la suite
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
			isMirror = objects[i].getMirrorProperty(); // Propriete miroir du ieme objet
		}
	}
	return hasIntersect;
}

Vect& Scene::estimatePixelColor(Ray& ray, double nbRebonds)
{
	Vect intersectionPoint, intersectionNormal; // Point d'intersection et le vecteur normale a la sphere a ce point 
	Vect objectAlbedo; // Albedo de l'objet intersecte dans la scene
	double racine; // Racine obtenue lors de l'intersection
	bool isMirror; // Propriete miroir du materiau
	bool hasIntersect = intersect(ray, intersectionPoint, intersectionNormal, objectAlbedo, racine, isMirror); // Determine si le Ray intersecte la sphere
	// Si c'est le cas, il renvoie le point d'intersection, la normale a la sphere a ce point d'intersection et l'albedo de l'objet intersecte
	
	Vect color(0., 0., 0.); // Par defaut la couleur du pixel est noire
	if (hasIntersect) { //S'il y a intersection sphere-ray, nous avons deux cas possibles : 

		if (isMirror) { // Cas miroir : la couleur resultera de l'intersection ray-objet du Ray reflechi
			Vect incidentDirection = ray.getDirection();
			Vect reflectedDirection = incidentDirection - 2 * dot(incidentDirection, intersectionNormal) * intersectionNormal;
			reflectedDirection = reflectedDirection.normalize();
			Ray reflectedRay(intersectionPoint + 0.001 * intersectionNormal, reflectedDirection); // Ray reflechi. Nous decalons l'origine du ray de la surface de l'objet pour eviter les effets de bord
			return estimatePixelColor(reflectedRay, nbRebonds + 1);// Couleur determinee par l'intersection ray-objet du ray reflechi
		}
		else { // Cas diffus : la couleur du pixel dependra de la distance entre le point d'intersection et la lampe
			Vect intersectionToLamp = lightOrigin - intersectionPoint;
			double distance = intersectionToLamp.getNorm(); // Distance entre le point d'intersection et la lampe

			/* Avant de calculer la couleur du pixel, nous allons vérifier que ce pixel n'appartient pas a l'ombre d'un element de la scene
			* Pour cela, nous envoyons un Ray vers la lampe.
			* S'il y a intersection ray-objet (donc racine positive)
			* et que le point d'intersection obtenue se situe sur le segment [premier point d'intersection - lampe] (donc shadowRacine < distance),
			* alors le pixel restera noir car il appartient à l'ombre d'un objet de la scene.
			*/

			Ray shadowRay(intersectionPoint + 0.0001 * intersectionNormal, intersectionToLamp / distance); // Ray allant du point d'intersection vers la lampe.
			// Nous avons decale l'origine du Ray de la surface de l'objet pour eviter les effets de bord (nommes bruits d'ombre)

			//Nous reutilisons la meme methode d'intersection donc nous calculons le point d'intersection et le vecteur normal a ce point mais ils ne seront pas utilises
			Vect shadowIntersectionPoint, shadowIntersectionNormal, shadowAlbedo;
			double shadowRacine; // racine de la nouvelle intersection
			bool shadowMirror; // propriete miroir non utilisee dans le cas de l'ombre
			bool hasShadowIntersect = intersect(shadowRay, shadowIntersectionPoint, shadowIntersectionNormal, shadowAlbedo, shadowRacine, shadowMirror);

			if (!hasShadowIntersect || shadowRacine >= distance) { // Dans le cas ou aucune des deux conditions pour obtenir de l'ombre n'est verifiee, nous determinons la couleur 
				double pixelIntensity = lightIntensity / (4 * M_PI * distance * distance) * max(0., dot(intersectionNormal, intersectionToLamp / distance)); // terme calcule pour obtenir un materiau considere comme diffus
				color = objectAlbedo / M_PI * pixelIntensity; // Couleur du pixel = Albedo de l'objet intersecte * terme de diffusion de la lumiere
			}
		}
	}

		
	
	return color;
}
