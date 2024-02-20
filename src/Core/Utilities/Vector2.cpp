#include "Vector2.h"
#include <cmath>

namespace Tapioca {
Vector2::Vector2(float xy) : x(xy), y(xy) { }

Vector2::Vector2(float x, float y) : x(x), y(y) { }

Vector2::Vector2(const Vector2& other) : x(other.x), y(other.y) { }

float Vector2::magnitudeSquared() const { return x * x + y * y; }
float Vector2::magnitude() const { return sqrtf(magnitudeSquared()); }

Vector2 Vector2::getNormalized() const {
	float mag = magnitude();
    return mag == 0 ? Vector2() : Vector2(x / mag, y / mag);
}

float Vector2::normalize() {
	float mag = magnitude();
	if(mag != 0) {
        x /= mag; y /= mag;
	}
	return mag;
}

}