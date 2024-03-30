#pragma once
#include "defs.h"

namespace Tapioca {
/*
* Clase que implementa un vector geometrico de dos dimensiones.
*/
class TAPIOCA_API Vector2 {
public:
    /*
    * @brief Constructora de la clase Vector2.
    * Genera un vector que inicializa "x" e "y" al numero dado. 
    * Si no se da un numero, este se considera cero.
    * @param xy Valor al que se inicializan "x" e "y"
    */
    Vector2(const float xy = 0);
    /*
    * @brief Constructora de la clase Vector2.
    * Genera un vector que inicializa "x" e "y" a sus valores correspondientes.
    * @param x Valor al que se inicializa "x"
    * @param y Valor al que se inicializa "y"
    */
    Vector2(const float x, const float y);
    /*
    * @brief Constructora de la clase Vector2.
    * Genera una copia del vector dado.
    * @param other Vector que se quiere copiar
    */
    Vector2(const Vector2& other);

    float x;
    float y;

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
    Vector2 getNormalized() const;
    /*
    * @brief Normaliza este vector y devuelve la magnitud que tenia anteriormente.
    * No ocurre nada si su magnitud es 0
    * @return Magnitud del vector antes de ser normalizado
    */
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

    inline bool operator==(const Vector2& other) const { return (x == other.x && y == other.y); }
    inline bool operator!=(const Vector2& other) const { return !(*this == other); }

    //Para comparar magnitudes
    //inline bool operator<(const Vector2& other) { return magnitude() < other.magnitude(); }
    //inline bool operator>(const Vector2& other) { return other.magnitude() < magnitude(); }
    //inline bool operator<=(const Vector2& other) { return !(magnitude() > other.magnitude()); }
    //inline bool operator>=(const Vector2& other) { return !(magnitude() < other.magnitude()); }

    /*
    * @brief Devuelve la interpolacion entre 2 vectores segun un factor de interpolacion
    * @param start Punto desde el que se quiere interpoalr
    * @param other Punto hasta el que se quiere interpolar.
    * @param t factor de interpolacion (debe ser un valor entre 0 y 1. Si no, se redondeara al limite mas cercano)
    * @return Valor interpolado entre los 2 valores con el factor t 
    */
    static Vector2 lerp(Vector2 const& start, Vector2 const& end, float t);
};
}