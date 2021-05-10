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

bool Scene::intersect(Ray& r, Vect& intersectionPoint, Vect& intersectionNormal, Vect& albedo,double& racine, bool& isMirror, bool& isTransparent, int& objectId)
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
			isTransparent = objects[i].getTransparencyProperty(); // Propriete de transparence du ieme objet
			objectId = i; // Indice de l'objet intersecte dans le vector objects
		}
	}
	return hasIntersect;
}

Vect& Scene::estimatePixelColor(Ray& ray, double nbRebonds)
{
	Vect intersectionPoint, intersectionNormal; // Point d'intersection et le vecteur normale a la sphere a ce point 
	Vect objectAlbedo; // Albedo de l'objet intersecte dans la scene
	double racine = 5E10; // Racine obtenue lors de l'intersection
	bool isMirror = false; // Propriete miroir du materiau$
	bool isTransparent = false; //  Propriete de transparence du materiau
	int objectId; // Indice de l'objet intersecte dans le vector objects
	bool hasIntersect = intersect(ray, intersectionPoint, intersectionNormal, objectAlbedo, racine, isMirror, isTransparent, objectId); // Determine si le Ray intersecte la sphere
	// Si c'est le cas, il renvoie le point d'intersection, la normale a la sphere a ce point d'intersection et l'albedo de l'objet intersecte
	
	Vect color(0., 0., 0.); // Par defaut la couleur du pixel est noire
	if (hasIntersect && nbRebonds < 5) { //S'il y a intersection sphere-ray et que nous n'avons pas atteint la fin de la recursion, nous avons quatre cas possibles : 

		if (objectId == 0) { // Cas où le ray intersecte la source de lumiere
			double rayon = objects[0].getRayon();
			color = lightIntensity / (4 * M_PI * M_PI * rayon * rayon) * Vect(1.,1.,1.);
		}
		else {
			if (isMirror) { // Cas miroir : la couleur resultera de l'intersection ray-objet du Ray reflechi
				Vect incidentDirection = ray.getDirection();
				Vect reflectedDirection = incidentDirection - 2 * dot(incidentDirection, intersectionNormal) * intersectionNormal;
				reflectedDirection = reflectedDirection.normalize();
				Ray reflectedRay(intersectionPoint + 0.001 * intersectionNormal, reflectedDirection); // Ray reflechi. Nous decalons l'origine du ray de la surface de l'objet pour eviter les effets de bord
				return estimatePixelColor(reflectedRay, nbRebonds + 1);// Couleur determinee par l'intersection ray-objet du ray reflechi
			}
			else {
				if (isTransparent) { /* Cas transparent : la couleur resultera de l'intersection ray-objet du Ray refracte
					* Nous faisons l'hypothese que la sphere est en verre quand elle est transparente.
					* Le ray refacte est calcule d'apres la 2eme loi de Snell-Descartes :
					* nIncident * sin(incidentAngle) = nRefracted * sin(refractedAngle)
					* Nous calculons d'abord la composante tangentielle (sin(refractedAngle)) a partir de la formule
					* Ensuite nous calculons la composante normale en partant du principe que la direction reflechie est unitaire
					*/
					double nAir = 1.0008; // Indice optique de l'air
					double nVerre = 1.5; // Indice optique du verre
					double nIncident, nRefracted; // Indices optiques des milieux incident et refringeant
					Vect opticalNormal; // vecteur normale optique

					Vect incidentDirection = ray.getDirection();
					if (dot(incidentDirection, intersectionNormal) > 0) {// Cas ou le ray incident sort de l'objet
						nIncident = nVerre; // le milieu incident est le verre
						nRefracted = nAir; // le milieu refringeant est l'air
						opticalNormal = -intersectionNormal; // la normale optique est oppose a la normale au point d'intersection
					}
					else {// Cas ou le ray incident entre dans l'objet
						nIncident = nAir; // le milieu incident est l'air
						nRefracted = nVerre; // le milieu refringeant est le verre
						opticalNormal = intersectionNormal; // la normale optique est la normale au point d'intersection
					}
					double opticalIndexRatio = nIncident / nRefracted; // Rapport des indices optiques
					Vect tangentialComponent = opticalIndexRatio * (incidentDirection - dot(incidentDirection, opticalNormal) * opticalNormal); // Composante tangentielle = sinus de l'angle refracte
					double squarredCosinusRefractedAngle = 1 - opticalIndexRatio * opticalIndexRatio * (1 - dot(incidentDirection, opticalNormal) * dot(incidentDirection, opticalNormal)); // Cosinus carre de l'angle refracte
					Vect normalComponent = sqrt(squarredCosinusRefractedAngle) * (-opticalNormal); // Composante normale = - cosinus de l'angle refracte

					Vect refractedDirection = (tangentialComponent + normalComponent).normalize(); // Direction du ray refracte

					Ray refractedRay(intersectionPoint - 0.0001 * opticalNormal, refractedDirection); // Ray refracte. Nous decalons l'origine pour eviter les effets de bord

					return estimatePixelColor(refractedRay, nbRebonds + 1); // Couleur determinee par l'intersection ray-objet du ray refracte
				}
				else { // Cas diffus : 
					/* L'equation du rendu determine la couleur en affirmant que la lumiere sortante d'un point est la somme des lumieres incidentes
					* mulitipliees par la BRDF (Bidirectionnal Reflectance Distribution Function, fonctions caracterisant le materiau).
					* L'equation du rendu necessitant le calcul d'une integrale, nous l'approximerons par la methode de Monte-Carlo.
					* Nous avons deux type d'eclairage : l'eclairage directe et l'eclairage indirecte
					*/

					// Contribution de l'eclairage directe:
					// la couleur du pixel dependra de la distance entre le point d'intersection et la lampe

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
					bool shadowMirror, shadowTransparent; // proprietes miroir et transparences non utilisees dans le cas de l'ombre
					int shadowObjectId; // Indice de l'objet intersecte
					bool hasShadowIntersect = intersect(shadowRay, shadowIntersectionPoint, shadowIntersectionNormal, shadowAlbedo, shadowRacine, shadowMirror, shadowTransparent, shadowObjectId);


					if (!hasShadowIntersect || shadowRacine >= distance) { // Dans le cas ou aucune des deux conditions pour obtenir de l'ombre n'est verifiee, nous determinons la couleur 
						double pixelIntensity = lightIntensity / (4 * M_PI * distance * distance) * max(0., dot(intersectionNormal, intersectionToLamp / distance)); // terme calcule pour obtenir un materiau considere comme diffus

						color = objectAlbedo / M_PI * pixelIntensity; // Couleur du pixel = Albedo de l'objet intersecte * terme de diffusion de la lumiere

					}


				}
				
				// Contribution de l'eclairage indirecte
				// Nous envoyons un ray depuis le point d'intersection dans une direction determinee aleatoirement dans une hemisphere
				// La couleur indirecte resultera de l'intersection ray-objet de ce ray
				Vect randomDirection = generateRandomDirection(intersectionNormal);
				Ray randomRay(intersectionPoint + 0.001 * intersectionNormal, randomDirection); // origine du ray decale pour eviter les effets de bord
				Vect indirectColor = estimatePixelColor(randomRay, nbRebonds + 1); // Couleur obtenue par eclairage indirecte
				// D'apres l'equation du rendu, la couleur du pixel est la somme des contributions des eclairages directe et indirecte
				Vect prod = (objectAlbedo * indirectColor);
				color = color + objectAlbedo * indirectColor;
			}
		}

		
	}
	return color;
}
