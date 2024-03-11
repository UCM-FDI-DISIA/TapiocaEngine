#pragma once
#include "defs.h"

namespace Tapioca {
class Vector2;

/**
* Clase que implementa un vector geometrico de tres dimensiones.
*/
class TAPIOCA_API Vector3 {
public:
    /*
    * @brief Constructora de la clase Vector3.
    * Genera un vector que inicializa "x", "y" y "z" al numero dado.
    * Si no se da un numero, este se considera cero.
    * @param xyz Valor al que se inicializan "x", "y" y "z"
    */
    Vector3(const float xyz = 0);
    /*
    * @brief Constructora de la clase Vector3.
    * Genera un vector que inicializa "x", "y" y "z" a sus valores correspondientes.
    * Si no se da un valor a "z", se considera cero.
    * @param x Valor al que se inicializa "x"
    * @param y Valor al que se inicializa "y"
    * @param z Valor al que se inicializa "z"
    */
    Vector3(const float x, const float y, const float z = 0);
    /*
    * @brief Constructora de la clase Vector3.
    * Genera una copia del vector dado.
    * @param other Vector que se quiere copiar
    */
    Vector3(const Vector3& other);
    /*
    * @brief Constructora de la clase Vector3.
    * Genera un vector con "x" e "y" tal como estan en el vector dado.
    * "z" se inicializa segun el numero dado. Si no se da un numero, este se considera cero.
    * @param other Vector2 del que se quiere copiar "x" e "y"
    * @param z Valor al que se inicializa "z"
    */
    Vector3(const Vector2& other, const float z = 0);

    float x;
    float y;
    float z;

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
    Vector3 getNormalized() const;
    /*
    * @brief Normaliza este vector y devuelve la magnitud que tenia anteriormente.
    * No ocurre nada si su magnitud es 0
    * @return Magnitud del vector antes de ser normalizado
    */
    float normalize();
    /*
    * @brief Rota el vector sobre el eje x
    * @param degrees Grado que se quiere rotar el vector
    * @return Resultado de rotar el vector degrees grados sobre el eje x
    */
    Vector3 rotateX(const float degrees);
    /*
    * @brief Rota el vector sobre el eje y
    * @param degrees Grado que se quiere rotar el vector
    * @return Resultado de rotar el vector degrees grados sobre el eje y
    */
    Vector3 rotateY(const float degrees);
    /*
    * @brief Rota el vector sobre el eje z
    * @param degrees Grado que se quiere rotar el vector
    * @return Resultado de rotar el vector degrees grados sobre el eje z
    */
    Vector3 rotateZ(const float degrees);
    /*
    * @brief Producto en cruz, devuelve el vector perpendicular.Sistema diestro
    * @param other Vector por el que se quiere multiplicar
    * @return Vector perpendicular a los vectores utilizados
    */
    Vector3 cross(const Vector3 other);
    /*
    * @brief Producto escalar
    * @return Devuelve el producto escalar entre los vectores
    */
    float dot(const Vector3 other);


    inline Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
    inline Vector3 operator-() const { return Vector3(-x, -y, -z); }
    inline Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    inline Vector3 operator*(const float& other) const { return Vector3(x * other, y * other, z * other); }
    inline Vector3 operator/(const float& other) const { return Vector3(x / other, y / other, z / other); }

    inline Vector3& operator+=(const Vector3& other) { return *this = *this + other; }
    inline Vector3& operator-=(const Vector3& other) { return *this = *this - other; }
    inline Vector3& operator*=(const float& other) { return *this = *this * other; }
    inline Vector3& operator/=(const float& other) { return *this = *this / other; }

    inline bool operator==(const Vector3& other) const { return (x == other.x && y == other.y && z == other.z); }
    inline bool operator!=(const Vector3& other) const { return !(*this == other); }

    //Para comparar magnitudes
    //inline bool operator<(const Vector3& other) { return magnitude() < other.magnitude(); }
    //inline bool operator>(const Vector3& other) { return other.magnitude() < magnitude(); }
    //inline bool operator<=(const Vector3& other) { return !(magnitude() > other.magnitude()); }
    //inline bool operator>=(const Vector3& other) { return !(magnitude() < other.magnitude()); }

};
}