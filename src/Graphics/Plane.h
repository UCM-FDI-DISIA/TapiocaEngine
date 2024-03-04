#pragma once
#include "OgreMath.h"
#include "RenderObject.h"

namespace Ogre {
class SceneManager;
class Plane;
}

namespace Tapioca {
class Node;
class Vector3;

class Plane : public RenderObject{
protected:
    Ogre::Plane* mPlane;

    //Construye un plano con todo inicializado a 0
    Plane(Ogre::SceneManager* scnMgr, Node* node);
    //Construye el plano a partir de la normal rkNormal y la mueve una distancia fConstant en la normal 
    Plane(Ogre::SceneManager* scnMgr, Node* node, const Vector3& rkNormal, float fConstant);
    //Construye un plano usando 4 constantes
    Plane(Ogre::SceneManager* scnMgr, Node* node, float a, float b, float c, float _d);

public:
    virtual ~Plane() { if(mPlane != nullptr) delete mPlane; };
    //Devuelve la normal que forma el plano
    Vector3 getNormal() const;
    //Devuelve la distancia desde el plano al origen
    float getD() const;
    //Devuelve distancia desde un punto al plano
    float getDistance(const Vector3& rkPoint) const;
    //Redefine el plano a partir de 3 puntos
    void redefine(const Vector3& p0, const Vector3& p1, const Vector3& p2);
    //Redefine el plano a partir de una normal y un punto
    void redefine(const Vector3& rkNormal, const Vector3& rkPoint);
    //Proyecta un vector sobre el plano
    Vector3 projectVector(const Vector3& v) const;
    //Normaliza la normal y la distancia d del plano. Devuelve la longitud de la normal antes de ser normalizada
    float normalise(void);

    //Comparadores del plano
    bool operator==(const Plane& rhs) const;
    bool operator!=(const Plane& rhs) const;

};

}

