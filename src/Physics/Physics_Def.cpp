#include "Physics_Def.h"
#include "Utilities/Vector3.h"
#include "Utilities/Quaternion.h"
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>

namespace Tapioca {

extern btVector3 toBtVector3(const Vector3 v) { return btVector3(v.x, v.y, v.z); }

extern Vector3 toVector3(const btVector3 v) { return Vector3(v.getX(), v.getY(), v.getZ()); }

extern btQuaternion toBtQuaternion(const Vector3 v) {
    btQuaternion quaternion;
    quaternion.setEuler(v.y, v.x, v.z);
    return quaternion;
}

extern Vector3 toEuler(const btQuaternion q) {
    btScalar x;
    btScalar y;
    btScalar z;
    q.getEulerZYX(z, y, x);
    return Vector3(x, y, z);
}


}
