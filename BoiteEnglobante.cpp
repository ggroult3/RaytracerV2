#include "BoiteEnglobante.h"

bool BoiteEnglobante::intersect(Ray& r)
{
    // Determine si le ray intersecte la boite englobante (si c'est le cas alors nous pourrons passage a l'etude de l'intersection ray-maillage
    bool hasIntersect = false;

    /* Pour chacun des plans de la boite englobante (2 sur ex, 2 sur ey, 2 sur ez),
    * nous determinons les points d'intersection ray-plan.
    * Pour cela, nous calculons dot(rayOrigin - pointFromPlan , planNormal) / dot(rayDirection, planNormal)
    * Nous obtenons donc 3 intervalles (un par axe)
    * S'il existe un intervalle ou ces 3 intervalles s'intersectent
    * alors nous avons bien une intersection ray-boite englobante
    */
    Vect rayOrigin = r.getOrigin();
    Vect rayDirection = r.getDirection();

    // Intersection des plans orthogonaux à ex
    double x1 = (borneInf[0] - rayOrigin[0]) / rayDirection[0];
    double x2 = (borneSup[0] - rayOrigin[0]) / rayDirection[0];
    double xInf = min(x1, x2);
    double xSup = max(x1, x2);

    // Intersection des plans orthogonaux à ey
    double y1 = (borneInf[1] - rayOrigin[1]) / rayDirection[1];
    double y2 = (borneSup[1] - rayOrigin[1]) / rayDirection[1];
    double yInf = min(y1, y2);
    double ySup = max(y1, y2);

    // Intersection des plans orthogonaux à ex
    double z1 = (borneInf[2] - rayOrigin[2]) / rayDirection[2];
    double z2 = (borneSup[2] - rayOrigin[2]) / rayDirection[2];
    double zInf = min(z1, z2);
    double zSup = max(z1, z2);

    double minOfBorneSup = min(xSup, min(ySup, zSup)); // la plus petite borne superieure des intervalles d'intersection
    double maxOfBorneInf = max(xInf, max(yInf, zInf)); // la plus grande borne inferieure des intervalles d'intersection

    // S'il existe un intervalle qui intersectent les 3 intervalles d'intersection,
    // alors cette intervalle est [maxOfBorneInf , minOfBorneSup]
    // De plus, minOfBorneSup doit etre possible si l'intersection a lieu derriere l'origine du ray donc non visible
    if (minOfBorneSup < maxOfBorneInf && minOfBorneSup > 0) {
        hasIntersect = true;
    }

    return hasIntersect;
}

Vect& BoiteEnglobante::getBorneSup()
{
    return borneSup;
}

Vect& BoiteEnglobante::getBorneInf()
{
    return borneInf;
}

void BoiteEnglobante::setBorneSup(Vect& u)
{
    borneSup = u;
}

void BoiteEnglobante::setBorneInf(Vect& u)
{
    borneInf = u;
}
