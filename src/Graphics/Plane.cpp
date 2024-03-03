#include "Plane.h"
#include <Ogre.h>
#include "Utilities/Vector3.h"

namespace Tapioca {
Vector3 Plane::getNormal() const { return Vector3(mPlane->normal.x, mPlane->normal.y, mPlane->normal.z); }

float Plane::getD() const { return mPlane->d; }

float Plane::getDistance(const Vector3& rkPoint) const {
    return mPlane->getDistance(Ogre::Vector3(rkPoint.x, rkPoint.y, rkPoint.z));
}

void Plane::redefine(const Vector3& p0, const Vector3&p1, const Vector3& p2) {
    mPlane->redefine(Ogre::Vector3(p0.x, p0.y, p0.z), Ogre::Vector3(p1.x, p1.y, p1.z), Ogre::Vector3(p2.x, p2.y, p2.z));
}

void Plane::redefine(const Vector3& rkNormal, const Vector3& rkPoint) {
    mPlane->redefine(Ogre::Vector3(rkNormal.x, rkNormal.y, rkNormal.z), Ogre::Vector3(rkPoint.x, rkPoint.y, rkPoint.z));
}

Vector3 Plane::projectVector(const Vector3& v) const { 
    Ogre::Vector3 aux = mPlane->projectVector(Ogre::Vector3(v.x, v.y, v.z));
    return Vector3(aux.x, aux.y, aux.z);
}

float Plane::normalise(void) { return mPlane->normalise(); }

Plane Plane::operator-() const {
    return Plane(-(mPlane->normal.x), -(mPlane->normal.y), -(mPlane->normal.z), -mPlane->d);
}

bool Plane::operator==(const Plane& rhs) const {
    return (
        rhs.getD() == mPlane->d && rhs.getNormal() == Vector3(mPlane->normal.x, mPlane->normal.y, mPlane->normal.z));
}

bool Plane::operator!=(const Plane& rhs) const { return !(*this == rhs); }

}
