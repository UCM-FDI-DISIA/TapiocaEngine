#pragma once

namespace Tapioca {
/**
* Clase que implementa un vector geometrico de dos dimensiones
*/
class Vector2 {
private:
    float x;
    float y;

public:
    Vector2(float xy = 0);
    Vector2(float x, float y);
    Vector2(const Vector2& other);

    inline float getX() const { return x; }
    inline float getY() const { return y; }

    float magnitudeSquared() const;
    float magnitude() const;
    Vector2 getNormalized() const;
    float normalize();

    inline Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
    inline Vector2 operator-() const { return Vector2(-x, -y); }
    inline Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
    inline Vector2 operator*(const float& other) const { return Vector2(x * other, y * other); }
    inline Vector2 operator/(const float& other) const { return Vector2(x / other, y / other); }

    inline Vector2& operator+=(const Vector2& other) { return *this = *this + other; }
    inline Vector2& operator-=(const Vector2& other) { return *this = *this - other; }
    inline Vector2& operator*=(const float& other) { return *this = *this * other; }
    inline Vector2& operator/=(const float& other) { return *this = *this / other; }
};
}