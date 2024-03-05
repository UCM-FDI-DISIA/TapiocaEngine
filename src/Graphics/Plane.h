#pragma once
#include "OgreMath.h"
#include "RenderObject.h"
#include "OgrePlane.h"
#include <string>
#include "OgreMovablePlane.h"

namespace Ogre {
class SceneManager;
class Plane;
}

namespace Tapioca {
class Node;
class Vector3;

class Plane : public RenderObject {
private:
    Ogre::Entity* mPlane;
    Ogre::Plane mPlaneAux;
    std::string name;

public:
    //Construye un plano con todo inicializado a 0
    //Plane(Ogre::SceneManager* scnMgr, Node* node, Ogre::MeshManager* mshMgr, std::string name, float width, float height,
    //      int xSegments, int ySegments, float x, float y, float z, std::string material);
    //Construye el plano a partir de la normal rkNormal y la mueve una distancia fConstant en la normal
    Plane(Ogre::SceneManager* scnMgr, Node* node, Ogre::MeshManager* mshMgr, const Vector3& rkNormal, float fConstant,
          std::string name, float width, float height, int xSegments, int ySegments, float x, float y, float z,
          std::string material);
    //Construye un plano usando 4 constantes
    Plane(Ogre::SceneManager* scnMgr, Node* node, Ogre::MeshManager* mshMgr, float a, float b, float c, float _d,
          std::string name, float width, float height, int xSegments, int ySegments, float x, float y,
          float z,
          std::string material);

    virtual ~Plane() { };
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
