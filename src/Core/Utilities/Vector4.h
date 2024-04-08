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
    /*
    * @brief Constructora de la clase Vector4.
    * Genera un vector que inicializa "x", "y", "z" y "w" al numero dado.
    * Si no se da un numero, este se considera cero.
    * @param xyz Valor al que se inicializan "x", "y", "z" y "w"
    */
    Vector4(const float xyzw = 0);
    /*
    * @brief Constructora de la clase Vector4.
    * Genera un vector que inicializa "x", "y", "z" y "w" a sus valores correspondientes.
    * Si no se da un numero, este se considera cero.
    * @param x Valor al que se inicializa "x"
    * @param y Valor al que se inicializa "y"
    * @param z Valor al que se inicializa "z"
    * @param w Valor al que se inicializa "w"
    */
    Vector4(const float x, const float y, const float z = 0, const float w = 0);
    /*
    * @brief Constructora de la clase Vector4.
    * Genera una copia del vector dado.
    * @param other Vector que se quiere copiar
    */
    Vector4(const Vector4& other);
    /*
    * @brief Genera un vector con "x", "y" y "z" tal como estan en el vector dado.
    * "w" se inicializa segun el numero dado. Si no se da un numero, este se considera cero.
    * @param other Vector3 del que se quiere copiar "x", "y" y "z"
    */
    Vector4(const Vector3& other, const float w = 0);
    /*
    * @brief Genera un vector con "x" e "y" tal como estan en el vector dado.
    * "z" y "w" se inicializan segun el numero dado. Si no se da algun numero, este se considera cero.
    * @param other Vector2 del que se quiere copiar "x" e "y"
    * @param z Valor al que se inicializa "z"
    * @param w Valor al que se inicializa "w"
    */
    Vector4(const Vector2& other, const float z = 0, const float w = 0);

    float x;
    float y;
    float z;
    float w;

    /*
    * @brief Devuelve la magnitud al cuadrado del vector
    * @return Magnitud al cuadrado del vector
    */
    float magnitudeSquared() const;
    /*
    * @brief Devuelve la magnitud del vector
    * @return Magnitud del vector
    */
    float magnitude() const;
    /*
    * @brief Devuelve este vector, pero normalizado.
    * Si este vector tiene magnitud cero, devuelve un vector de magnitud cero.
    * @return Vector normalizado
    */
    Vector4 getNormalized() const;
    /*
    * @brief Normaliza este vector y devuelve la magnitud que tenia anteriormente.
    * No ocurre nada si su magnitud es 0
    * @return Magnitud del vector antes de ser normalizado
    */
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

    /*
    * @brief Devuelve la interpolacion entre este vector y otro segun un factor de interpolacion
    * @param end Punto hasta el que se quiere interpolar.
    * @param t factor de interpolacion (debe ser un valor entre 0 y 1. Si no, se redondeara al limite mas cercano)
    * @return Valor interpolado entre los 2 valores con el factor t 
    */
    Vector4 lerp(Vector4 const& end, float t);

    /*
    * @brief Devuelve la distancia entre este vector y otro
    * @param other Vector hasta el que se quiere calcular la distancia.
    * @return Distancia entre los 2 vectores
    */
    float distance(Vector4 const& other);
};
}