#include "physicsDefs.h"
#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include "Utilities/Vector3.h"
#include "Utilities/Quaternion.h"
#include "checkML.h"

namespace Tapioca {
extern btVector3 toBtVector3(const Vector3 v) { return btVector3(v.x, v.y, v.z); }

extern Vector3 toVector3(const btVector3 v) { return Vector3(v.getX(), v.getY(), v.getZ()); }

extern btQuaternion toBtQuaternion(const Vector3 v) {
    btQuaternion quaternion;
    quaternion.setEuler(v.y * ((float)M_PI / 180), v.x * ((float)M_PI / 180),
                        v.z * ((float)M_PI / 180));   // A radianes
    return quaternion;
}

extern Vector3 toEuler(const btQuaternion q) {
    btScalar x;
    btScalar y;
    btScalar z;
    q.getEulerZYX(z, y, x);
    return Vector3(x * (180 / (float)M_PI), y * (180 / (float)M_PI), z * (180 / (float)M_PI));   // En grados
}
}
