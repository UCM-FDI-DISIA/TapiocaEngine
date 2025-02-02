#include "Vector2.h"
#include <cmath>
#include "checkML.h"

namespace Tapioca {
Vector2::Vector2(const float xy) : x(xy), y(xy) { }

Vector2::Vector2(const float x, const float y) : x(x), y(y) { }

Vector2::Vector2(const Vector2& other) : x(other.x), y(other.y) { }

float Vector2::magnitudeSquared() const { return x * x + y * y; }
float Vector2::magnitude() const { return sqrtf(magnitudeSquared()); }

Vector2 Vector2::getNormalized() const {
	float mag = magnitude();
    return mag == 0 ? Vector2() : Vector2(x / mag, y / mag);
}

float Vector2::normalize() {
	float mag = magnitude();
	if (mag != 0) {
        x /= mag; 
		y /= mag;
	}
	return mag;
}

Vector2 Vector2::lerp(Vector2 const& end, float t) const {
    t = clamp(t, 0, 1);

    float lx = (1 - t) * x + t * end.x;
    float ly = (1 - t) * y + t * end.y;
    return Vector2(lx, ly);
}
float Vector2::distance(Vector2 const& other) const { 
    float dx = other.x - x;
    float dy = other.y - y;
    return std::sqrt(dx * dx + dy * dy);
}

float Vector2::clamp(float v, float min, float max) {
    if (v < min) return min;
    else if (v > max) return max;
    else return v;
}
}
