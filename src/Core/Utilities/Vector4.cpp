#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include <cmath>

namespace Tapioca {

Vector4::Vector4(const float xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) { }

Vector4::Vector4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) { }

Vector4::Vector4(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }


Vector4::Vector4(const Vector3& other, const float w) : x(other.x), y(other.y), z(other.z), w(w) { }

Vector4::Vector4(const Vector2& other, const float z, const float w) : x(other.x), y(other.y), z(z), w(w) { }

float Vector4::magnitudeSquared() const { return x * x + y * y + z * z; }

float Vector4::magnitude() const { return sqrtf(magnitudeSquared()); }

Vector4 Vector4::getNormalized() const {
	float mag = magnitude();
    return mag == 0 ? Vector4() : Vector4(x / mag, y / mag, z / mag, w / mag);
}

float Vector4::normalize() {
	float mag = magnitude();
    if (mag != 0) {
        x /= mag;
        y /= mag;
        z /= mag;
        w /= mag;
    }
	return mag;
}

Vector4 Vector4::lerp(Vector4 const& start, Vector4 const& end, float t) {
    t = clamp(t, 0, 1);

    float x = (1 - t) * start.x + t * end.x;
    float y = (1 - t) * start.y + t * end.y;
    float z = (1 - t) * start.z + t * end.z;
    float w = (1 - t) * start.w + t * end.w;
    return Vector4(x, y, z, w);
}
}
