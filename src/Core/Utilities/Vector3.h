#pragma once

namespace Tapioca {
class Vector2;

/**
* Clase que implementa un vector geometrico de tres dimensiones.
*/
class Vector3 {
public:
    // Genera un vector que inicializa "x", "y" y "z" al numero dado.
    // Si no se da un numero, este se considera cero.
    Vector3(float xyz = 0);
    // Genera un vector que inicializa "x", "y" y "z" a sus valores correspondientes.
    // Si no se da un valor a "z", se considera cero.
    Vector3(float x, float y, float z = 0);
    // Genera una copia del vector dado.
    Vector3(const Vector3& other);
    // Genera un vector con "x" e "y" tal como estan en el vector dado.
    // "z" se inicializa segun el numero dado. Si no se da un numero, este se considera cero.
    Vector3(const Vector2& other, float z = 0);

    float x;
    float y;
    float z;

    // Devuelve la magnitud al cuadrado del vector.
    float magnitudeSquared() const;
    // Devuelve la magnitud del vector.
    float magnitude() const;
    // Devuelve este vector, pero normalizado.
    // Si este vector tiene magnitud cero, devuelve un vector de magnitud cero.
    Vector3 getNormalized() const;
    // Normaliza este vector y devuelve la magnitud que tenia anteriormente.
    // Si este vector tenia magnitud cero, no cambia.
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