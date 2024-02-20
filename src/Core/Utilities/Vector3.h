#pragma once

#define __TAPIOCA_VECTOR3

namespace Tapioca {
class Vector2;

/**
* Clase que implementa un vector geometrico de tres dimensiones
*/
class Vector3 {
private:
    float x;
    float y;
    float z;

public:
    Vector3(float xyz = 0);
    Vector3(float x, float y, float z = 0);
    Vector3(const Vector3& other);
    Vector3(const Vector2& other);

    inline float getX() const { return x; }
    inline float getY() const { return y; }
    inline float getZ() const { return z; }

    float magnitudeSquared() const;
    float magnitude() const;
    Vector3 getNormalized() const;
    float normalize();

    inline Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
    inline Vector3 operator-() const { return Vector3(-x, -y, -z); }
    inline Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    inline Vector3 operator*(const float& other) const { return Vector3(x * other, y * other, z * other); }
    inline Vector3 operator/(const float& other) const { return Vector3(x / other, y / other, z / other); }

    inline Vector3& operator+=(const Vector3& other) { return *this = *this + other; }
    inline Vector3& operator-=(const Vector3& other) { return *this = *this - other; }
    inline Vector3& operator*=(const float& other) { return *this = *this * other; }
    inline Vector3& operator/=(const float& other) { return *this = *this / other; }

};
}