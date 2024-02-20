#include "Vector3.h"
#include "Vector2.h"
#include <cmath>

namespace Tapioca {
Vector3::Vector3(float xyz) : x(xyz), y(xyz), z(xyz) { }

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) { }

Vector3::Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) { }

Vector3::Vector3(const Vector2& other, float z) : x(other.x), y(other.y), z(z) { }

float Vector3::magnitudeSquared() const { return x * x + y * y + z * z; }

float Vector3::magnitude() const { return sqrtf(magnitudeSquared()); }

Vector3 Vector3::getNormalized() const {
	float mag = magnitude();
    return mag == 0 ? Vector3() : Vector3(x / mag, y / mag, z / mag);
}

float Vector3::normalize() {
	float mag = magnitude();
	if(mag != 0) {
		x /= mag; y /= mag; z /= mag;
	}
	return mag;
}

}
