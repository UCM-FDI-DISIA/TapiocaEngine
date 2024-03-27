#include "Plane.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "RenderNode.h"
#include "Utilities/Vector3.h"
#include "checkML.h"

namespace Tapioca {

//Plane(Ogre::Plane(Ogre::Vector3(rkNormal.x, rkNormal.y, rkNormal.z), fConstant))

//Plane::Plane(Ogre::SceneManager* scnMgr, Node* node, Ogre::MeshManager* mshMgr, std::string name, float width,
//             float height, int xSegments, int ySegments, float x, float y, float z, std::string material)
//    : RenderObject(node, scnMgr), name(name) {
//    mPlaneAux = Ogre::Plane();
//    mshMgr->createPlane(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::Plane(), width, height,
//                        xSegments, ySegments);
//
//    mPlane = scnMgr->createEntity(name);
//    if (material != "") mPlane->setMaterialName(material);
//
//    init(mPlane);
//
//    node->setPosition(Vector3(x, y, z));
//}

Plane::Plane(Ogre::SceneManager* const scnMgr, RenderNode* const node, Ogre::MeshManager* const mshMgr,
             const Vector3& rkNormal, const float fConstant, std::string const& name, const float width,
             const float height, const int xSegments, const int ySegments)
    : RenderObject(node, scnMgr) {
    mPlaneAux = Ogre::Plane(Ogre::Vector3(rkNormal.x, rkNormal.y, rkNormal.z), fConstant);
    mshMgr->createPlane(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mPlaneAux, width, height,
                        xSegments, ySegments);

    mPlane = scnMgr->createEntity(name);

    init(mPlane);

    mPlane->setCastShadows(false);
}

Plane::Plane(Ogre::SceneManager* const scnMgr, RenderNode* const node, Ogre::MeshManager* const mshMgr, const float a,
             const float b, const float c, const float _d, std::string const& name, const float width,
             const float height, const int xSegments, const int ySegments)
    : RenderObject(node, scnMgr) {
    mPlaneAux = Ogre::Plane(Ogre::Vector3(a, b, c), _d);
    mshMgr->createPlane(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mPlaneAux, width, height,
                        xSegments, ySegments);

    mPlane = scnMgr->createEntity(name);

    init(mPlane);
}

Vector3 Plane::getNormal() const { return Vector3(mPlaneAux.normal.x, mPlaneAux.normal.y, mPlaneAux.normal.z); }

float Plane::getD() const { return mPlaneAux.d; }

float Plane::getDistance(const Vector3& rkPoint) const {
    return mPlaneAux.getDistance(Ogre::Vector3(rkPoint.x, rkPoint.y, rkPoint.z));
}

void Plane::redefine(const Vector3& p0, const Vector3& p1, const Vector3& p2) {
    Ogre::MeshManager::getSingleton().getByName(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    mPlaneAux.redefine(Ogre::Vector3(p0.x, p0.y, p0.z), Ogre::Vector3(p1.x, p1.y, p1.z),
                       Ogre::Vector3(p2.x, p2.y, p2.z));
}

void Plane::redefine(const Vector3& rkNormal, const Vector3& rkPoint) {
    mPlaneAux.redefine(Ogre::Vector3(rkNormal.x, rkNormal.y, rkNormal.z),
                       Ogre::Vector3(rkPoint.x, rkPoint.y, rkPoint.z));
}

Vector3 Plane::projectVector(const Vector3& v) const {
    Ogre::Vector3 aux = mPlaneAux.projectVector(Ogre::Vector3(v.x, v.y, v.z));
    return Vector3(aux.x, aux.y, aux.z);
}

float Plane::normalise(void) { return mPlaneAux.normalise(); }

void Plane::setMaterial(std::string const& material) { mPlane->setMaterialName(material); }

bool Plane::operator==(const Plane& rhs) const {
    return (rhs.getD() == mPlaneAux.d &&
            rhs.getNormal() == Vector3(mPlaneAux.normal.x, mPlaneAux.normal.y, mPlaneAux.normal.z));
}

bool Plane::operator!=(const Plane& rhs) const { return !(*this == rhs); }

}
