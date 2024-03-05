#pragma once
#include "defs.h"

namespace Tapioca {
class Vector3;
class Vector2;

/**
* Clase que implementa un vector geometrico de tres dimensiones.
*/
class TAPIOCA_API Vector4 {
public:
    // Genera un vector que inicializa "x", "y", "z" y "w" al numero dado.
    // Si no se da un numero, este se considera cero.
    Vector4(float xyzw = 0);
    // Genera un vector que inicializa "x", "y", "z" y "w" a sus valores correspondientes.
    // Si no se da un valor a "z" o a "w", se considera cero.
    Vector4(float x, float y, float z = 0, float w = 0);
    // Genera una copia del vector dado.
    Vector4(const Vector4& other);
    // Genera un vector con "x", "y" y "z" tal como estan en el vector dado.
    // "w" se inicializa segun el numero dado. Si no se da un numero, este se considera cero.
    Vector4(const Vector3& other, float w = 0);
    // Genera un vector con "x" e "y" tal como estan en el vector dado.
    // "z" y "w" se inicializan segun el numero dado. Si no se da algun numero, este se considera cero.
    Vector4(const Vector2& other, float z = 0, float w = 0);

    float x;
    float y;
    float z;
    float w;

    // Devuelve la magnitud al cuadrado del vector.
    float magnitudeSquared() const;
    // Devuelve la magnitud del vector.
    float magnitude() const;
    // Devuelve este vector, pero normalizado.
    // Si este vector tiene magnitud cero, devuelve un vector de magnitud cero.
    Vector4 getNormalized() const;
    // Normaliza este vector y devuelve la magnitud que tenia anteriormente.
    // Si este vector tenia magnitud cero, no cambia.
    float normalize();

    inline Vector4 operator+(const Vector4& other) const { return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
    inline Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }
    inline Vector4 operator-(const Vector4& other) const { return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
    inline Vector4 operator*(const float& other) const { return Vector4(x * other, y * other, z * other, w * other); }
    inline Vector4 operator/(const float& other) const { return Vector4(x / other, y / other, z / other, w / other); }

    inline Vector4& operator+=(const Vector4& other) { return *this = *this + other; }
    inline Vector4& operator-=(const Vector4& other) { return *this = *this - other; }
    inline Vector4& operator*=(const float& other) { return *this = *this * other; }
    inline Vector4& operator/=(const float& other) { return *this = *this / other; }

    inline bool operator==(const Vector4& other) const {
        return (x == other.x && y == other.y && z == other.z && w == other.w);
    }
    inline bool operator!=(const Vector4& other) const { return !(*this == other); }

    //Para comparar magnitudes
    //inline bool operator<(const Vector4& other) { return magnitude() < other.magnitude(); }
    //inline bool operator>(const Vector4& other) { return other.magnitude() < magnitude(); }
    //inline bool operator<=(const Vector4& other) { return !(magnitude() > other.magnitude()); }
    //inline bool operator>=(const Vector4& other) { return !(magnitude() < other.magnitude()); }

};
}