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

    Plane();
    Plane(const Ogre::Vector3& rkNormal, Ogre::Real fConstant);
    Plane(Ogre::Real a, Ogre::Real b, Ogre::Real c, Ogre::Real _d);

public:
    Vector3 getNormal() const;
    float getD() const;

    float getDistance(const Vector3& rkPoint) const;
    void redefine(const Vector3& p0, const Vector3& p1, const Vector3& p2);
    void redefine(const Vector3& rkNormal, const Vector3& rkPoint);
    Vector3 projectVector(const Vector3& v) const;
    float normalise(void);
    Plane operator-() const;
    bool operator==(const Plane& rhs) const;
    bool operator!=(const Plane& rhs) const;

};

}

