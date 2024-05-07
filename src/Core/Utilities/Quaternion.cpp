#include "Quaternion.h"
#include <cmath>

namespace Tapioca {
Quaternion::Quaternion(const float q0, const float q1, const float q2, const float q3) {
    scalar = q0;
    vector = Vector3(q1, q2, q3);
    float auxq0 = q0;
    if (q0 > 1 || q0 < -1) auxq0 = std::round(auxq0);
    angle = 2.0f * acosf(auxq0);
}

Quaternion::Quaternion(const float alfa, const Vector3& vec) {
    // Convierte vec en un vector unitario
    Vector3 uvec = vec / vec.magnitude();
    float alfarad = alfa * ((float)M_PI / 180.f);
    angle = alfarad;
    scalar = cosf(alfarad / 2.f);
    float alfasin = sinf(alfarad / 2.f);
    vector = Vector3(alfasin * uvec.x, alfasin * uvec.y, alfasin * uvec.z);
}

Quaternion::Quaternion(const Vector3& euler) {
    // Biblioteca "math" opera en radianes
    float roll = euler.x * ((float)M_PI / 180.f);
    float cosroll = cosf(roll / 2.f);
    float sinroll = sinf(roll / 2.f);

    float yaw = euler.y * ((float)M_PI / 180.f);
    float cosyaw = cosf(yaw / 2.f);
    float sinyaw = sinf(yaw / 2.f);

    float pitch = euler.z * ((float)M_PI / 180.f);
    float cospitch = cosf(pitch / 2.f);
    float sinpitch = sinf(pitch / 2.f);

    scalar = cosroll * cosyaw * cospitch - sinroll * sinyaw * sinpitch;

    vector.x = sinroll * cosyaw * cospitch + cosroll * sinyaw * sinpitch;
    vector.y = cosroll * sinyaw * cospitch + sinroll * cosyaw * sinpitch;
    vector.z = cosroll * cosyaw * sinpitch - sinroll * sinyaw * cospitch;

    angle = 2.f * acosf(scalar);
}

Quaternion Quaternion::inverse() { return conjugate() / magnitude(); }

Quaternion Quaternion::conjugate() { return Quaternion(scalar, -vector.x, -vector.y, -vector.z); }

float Quaternion::magnitude() {
    return sqrtf(scalar * scalar + vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

Vector3 Quaternion::toEuler() {
    normalize();
    float x, y, z;

    float sinr_cosp = 2 * (scalar * vector.x+ vector.y * scalar);
    float cosr_cosp = 1 - 2 * (vector.x * vector.x + vector.y * vector.y);
    x = std::atan2(sinr_cosp, cosr_cosp);

    float sinp = 2 * (scalar * vector.y - vector.z * vector.x);
    if (std::abs(sinp) >= 1) y = std::copysign((float)M_PI / 2, sinp);
    else
        y = std::asin(sinp);

    float siny_cosp = 2 * (scalar * vector.z + vector.x * vector.y);
    float cosy_cosp = 1 - 2 * (vector.y * vector.y + vector.z * vector.z);
    z = std::atan2(siny_cosp, cosy_cosp);

    return Vector3(x * (180.0f / (float)M_PI), y * (180.0f / (float)M_PI), z * (180.0f / (float)M_PI));
}

Quaternion Quaternion::operator*(const Quaternion& rhs) {
    float newScalar = scalar * rhs.scalar - vector.x * rhs.vector.x - vector.y * rhs.vector.y - vector.z * rhs.vector.z;
    Vector3 newVector =
        Vector3((scalar * rhs.vector.x + vector.x * rhs.scalar + vector.y * rhs.vector.z - vector.z * rhs.vector.y),
                (scalar * rhs.vector.y - vector.x * rhs.vector.z + vector.y * rhs.scalar + vector.z * rhs.vector.x),
                (scalar * rhs.vector.z + vector.x * rhs.vector.y - vector.y * rhs.vector.x + vector.z * rhs.scalar));
    return Quaternion(newScalar, newVector.x, newVector.y, newVector.z);
}

Quaternion Quaternion::operator*(const float s) {
    return Quaternion(scalar * s, vector.x * s, vector.y * s, vector.z * s);
}

Quaternion Quaternion::operator/(const float s) {
    return Quaternion(scalar / s, vector.x / s, vector.y / s, vector.z / s);
}

Vector3 Quaternion::rotatePoint(const Vector3& point) {
    // En este caso el punto no rota
    if (!(vector.x || vector.y || vector.z) || !angle) return point;

    Vector3 axis = vector / sinf(angle / 2);
    return point + axis.cross(axis.cross(point) * (1.f - cosf(angle))) + axis.cross(point) * sinf(angle);
}

Quaternion Quaternion::normalized() { return *this / magnitude(); }

void Quaternion::normalize() { (*this) = *this / magnitude(); }
}
