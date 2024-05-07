#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include <cmath>
#include "checkML.h"

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

Vector4 Vector4::lerp(Vector4 const& end, float t) const {
    t = Vector2::clamp(t, 0, 1);

    float lx = (1 - t) * x + t * end.x;
    float ly = (1 - t) * y + t * end.y;
    float lz = (1 - t) * z + t * end.z;
    float lw = (1 - t) * w + t * end.w;
    return Vector4(lx, ly, lz, lw);
}
float Vector4::distance(Vector4 const& other) const {
    float dx = other.x - x;
    float dy = other.y - y;
    float dz = other.z - z;
    float dw = other.w - w;
    return std::sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}
}
