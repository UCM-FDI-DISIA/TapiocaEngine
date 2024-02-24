#pragma once
#include "../Core/Utilities/Vector3.h"
#include "../../../dependencies/Bullet/src/src/LinearMath/btVector3.h"
#include "../../../dependencies/Bullet/src/src/LinearMath/btQuaternion.h"
#define PI 3.14159265359
namespace Tapioca {

enum ColliderShape { BOX_SHAPE, SPHERE_SHAPE, PLANE_SHAPE, CAPSULE_SHAPE };
enum MovementType { DYNAMIC_OBJECT, STATIC_OBJECT, KINEMATIC_OBJECT };
//conversiones de vectores
static btVector3 toBtVector3(Vector3 v) { return btVector3(v.x, v.y, v.z); };
static Vector3 toVector3(btVector3 v) { return Vector3(v.getX(), v.getY(), v.getZ()); };
static btQuaternion toBtQuaternion(Vector3 v) {
    btQuaternion quaternion;
    quaternion.setEuler(v.y, v.x, v.z);
    return quaternion;
};
static Vector3 toEuler(btQuaternion q) {
    double roll = atan2(
        2.0 * (q.getY() * q.getW() + q.getX() * q.getZ()), 1.0 - 2.0 * (q.getY() * q.getY() + q.getX() * q.getX()));
    double pitch = asin(2.0 * (q.getW() * q.getX() - q.getY() * q.getZ()));
    double yaw = atan2(
        2.0 * (q.getZ() * q.getW() + q.getX() * q.getY()), 1.0 - 2.0 * (q.getZ() * q.getZ() + q.getX() * q.getX()));

    return Vector3(roll * 180.0 / PI,yaw * 180.0 / PI , pitch * 180.0 / PI);
};
}