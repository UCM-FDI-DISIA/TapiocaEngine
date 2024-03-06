#include "Physics_Def.h"
#include "Utilities/Vector3.h"
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
    double roll = atan2(2.0 * (q.getY() * q.getW() + q.getX() * q.getZ()), 1.0 - 2.0 * (q.getY() * q.getY() + q.getX() * q.getX()));
    double pitch = asin(2.0 * (q.getW() * q.getX() - q.getY() * q.getZ()));
    double yaw = atan2(2.0 * (q.getZ() * q.getW() + q.getX() * q.getY()), 1.0 - 2.0 * (q.getZ() * q.getZ() + q.getX() * q.getX()));

    return Vector3(float(roll * 180.0 / PI), float(yaw * 180.0 / PI), float(pitch * 180.0 / PI));
}


}
