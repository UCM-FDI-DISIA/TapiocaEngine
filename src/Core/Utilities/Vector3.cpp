#include "Vector3.h"
#include "Vector2.h"
#include <cmath>
#include <math.h>
#define PI 3.14159265358979323846f

namespace Tapioca {
Vector3::Vector3(const float xyz) : x(xyz), y(xyz), z(xyz) { }

Vector3::Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) { }

Vector3::Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) { }

Vector3::Vector3(const Vector2& other, const float z) : x(other.x), y(other.y), z(z) { }

float Vector3::magnitudeSquared() const { return x * x + y * y + z * z; }

float Vector3::magnitude() const { return sqrtf(magnitudeSquared()); }

Vector3 Vector3::getNormalized() const {
    float mag = magnitude();
    return mag == 0 ? Vector3() : Vector3(x / mag, y / mag, z / mag);
}

float Vector3::normalize() {
    float mag = magnitude();
    if (mag != 0) {
        x /= mag;
        y /= mag;
        z /= mag;
    }
    return mag;
}

Vector3 Vector3::rotateX(const float degrees) const {
    float rad_angle = degrees * PI / 180.0f;   
    return Vector3(x, cos(rad_angle) * y - sin(rad_angle) * z, sin(rad_angle) * y + cos(rad_angle) * z);
}

Vector3 Vector3::rotateY(const float degrees) const {
    float rad_angle = degrees * PI / 180.0f;  
    return Vector3(cos(rad_angle) * x + sin(rad_angle) * z, y, -sin(rad_angle) * x + cos(rad_angle) * z);
}

Vector3 Vector3::rotateZ(const float degrees) const {
    float rad_angle = degrees * PI / 180.0f;  
    return Vector3(cos(rad_angle) * x - sin(rad_angle) * y, sin(rad_angle) * x + cos(rad_angle) * y, z);
}

Vector3 Vector3::cross(const Vector3 other) const {
    return Vector3((y * other.z - z * other.y), (z * other.x - x * other.z), (x * other.y - y * other.x));
}

float Vector3::dot(const Vector3 other) const { return x * other.x + y * other.y + z * other.z; }

Vector3 Vector3::lerp(Vector3 const& end, float t) const {
    t = Vector2::clamp(t, 0, 1);

    float lx = (1 - t) * x + t * end.x;
    float ly = (1 - t) * y + t * end.y;
    float lz = (1 - t) * z + t * end.z;
    return Vector3(lx, ly, lz);
}

float Vector3::distance(Vector3 const& other) const {
    float dx = other.x - x;
    float dy = other.y - y;
    float dz = other.z - z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}
}
