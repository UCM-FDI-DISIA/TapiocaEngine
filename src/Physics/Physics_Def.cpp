#include "physicsDefs.h"
#include "Utilities/Vector3.h"
#include "Utilities/Quaternion.h"
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>

namespace Tapioca {

extern btVector3 toBtVector3(const Vector3 v) { return btVector3(v.x, v.y, v.z); }

extern Vector3 toVector3(const btVector3 v) { return Vector3(v.getX(), v.getY(), v.getZ()); }

extern btQuaternion toBtQuaternion(const Vector3 v) {
    btQuaternion quaternion;
    quaternion.setEuler(v.y * (PI / 180), v.x * (PI / 180), v.z * (PI / 180)); //a rad
    return quaternion;
}

extern Vector3 toEuler(const btQuaternion q) {
    btScalar x;
    btScalar y;
    btScalar z;
    q.getEulerZYX(z, y, x);
    return Vector3(x * (180 / PI), y * (180 / PI), z * (180 / PI)); //en grados
}


}
