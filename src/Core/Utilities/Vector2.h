#pragma once

namespace Tapioca {
/**
* Clase que implementa un vector geometrico de dos dimensiones.
*/
class Vector2 {
public:
    // Genera un vector que inicializa "x" e "y" al numero dado.
    // Si no se da un numero, este se considera cero.
    Vector2(float xy = 0);
    // Genera un vector que inicializa "x" e "y" a sus valores correspondientes.
    Vector2(float x, float y);
    // Genera una copia del vector dado.
    Vector2(const Vector2& other);

    float x;
    float y;

    // Devuelve la magnitud al cuadrado del vector.
    float magnitudeSquared() const;
    // Devuelve la magnitud del vector.
    float magnitude() const;
    // Devuelve este vector, pero normalizado.
    // Si este vector tiene magnitud cero, devuelve un vector de magnitud cero.
    Vector2 getNormalized() const;
    // Normaliza este vector y devuelve la magnitud que tenia anteriormente.
    // Si este vector tenia magnitud cero, no cambia.
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

    inline bool operator==(const Vector2& other) { return (x == other.x && y == other.y); }
    inline bool operator!=(const Vector2& other) { return !(*this == other); }

    //Para comparar magnitudes
    //inline bool operator<(const Vector2& other) { return magnitude() < other.magnitude(); }
    //inline bool operator>(const Vector2& other) { return other.magnitude() < magnitude(); }
    //inline bool operator<=(const Vector2& other) { return !(magnitude() > other.magnitude()); }
    //inline bool operator>=(const Vector2& other) { return !(magnitude() < other.magnitude()); }
};
}