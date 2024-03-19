#pragma once
#define PI 3.14159265359f

class btVector3;
class btQuaternion;

namespace Tapioca {
class Vector3;

enum ColliderShape { BOX_SHAPE, SPHERE_SHAPE, PLANE_SHAPE, CAPSULE_SHAPE };
enum MovementType { DYNAMIC_OBJECT, STATIC_OBJECT, KINEMATIC_OBJECT };

//conversiones de vectores
extern btVector3 toBtVector3(const Vector3 v);
extern Vector3 toVector3(const btVector3 v);
extern btQuaternion toBtQuaternion(const Vector3 v);
extern Vector3 toEuler(const btQuaternion q);
}