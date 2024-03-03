#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include <cmath>

namespace Tapioca {

Vector4::Vector4(float xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) { }

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }

Vector4::Vector4(const Vector4& other) : x(other.x), y(other.y), z(other.z), w(other.w) { }


Vector4::Vector4(const Vector3& other, float w) : x(other.x), y(other.y), z(other.z), w(w) { }

Vector4::Vector4(const Vector2& other, float z, float w) : x(other.x), y(other.y), z(z), w(w) { }

float Vector4::magnitudeSquared() const { return x * x + y * y + z * z; }

float Vector4::magnitude() const { return sqrtf(magnitudeSquared()); }

Vector4 Vector4::getNormalized() const {
	float mag = magnitude();
    return mag == 0 ? Vector4() : Vector4(x / mag, y / mag, z / mag, w / mag);
}

float Vector4::normalize() {
	float mag = magnitude();
	if (mag != 0) x /= mag; y /= mag; z /= mag; w /= mag;
	return mag;
}

}
