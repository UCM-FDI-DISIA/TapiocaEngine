#pragma once
#include "defs.h"

namespace Tapioca {
class Vector2;

/**
* @brief Clase que implementa un vector geometrico de tres dimensiones
*/
class TAPIOCA_API Vector3 {
public:
    float x;   // Coordenada x del vector
    float y;   // Coordenada y del vector
    float z;   // Coordenada z del vector

    /**
    * @brief Constructor de la clase Vector3.
    * Genera un vector que inicializa "x", "y" y "z" al numero dado.
    * Si no se da un numero, este se considera cero
    * @param xyz Valor al que se inicializan "x", "y" y "z"
    */
    Vector3(const float xyz = 0);
    /**
    * @brief Constructor de la clase Vector3.
    * Genera un vector que inicializa "x", "y" y "z" a sus valores correspondientes.
    * Si no se da un valor a "z", se considera cero
    * @param x Valor al que se inicializa "x"
    * @param y Valor al que se inicializa "y"
    * @param z Valor al que se inicializa "z"
    */
    Vector3(const float x, const float y, const float z = 0);
    /**
    * @brief Constructor de la clase Vector3.
    * Genera una copia del vector dado
    * @param other Vector que se quiere copiar
    */
    Vector3(const Vector3& other);
    /**
    * @brief Constructor de la clase Vector3.
    * Genera un vector con "x" e "y" tal como estan en el vector dado.
    * "z" se inicializa segun el numero dado. Si no se da un numero, este se considera cero
    * @param other Vector2 del que se quiere copiar "x" e "y"
    * @param z Valor al que se inicializa "z"
    */
    Vector3(const Vector2& other, const float z = 0);

    /**
    * @brief Devuelve la magnitud al cuadrado del vector
    * @return Magnitud al cuadrado del vector
    */
    float magnitudeSquared() const;
    /**
    * @brief Devuelve la magnitud del vector
    * @return Magnitud del vector
    */
    float magnitude() const;
    /**
    * @brief Devuelve este vector, pero normalizado.
    * Si este vector tiene magnitud cero, devuelve un vector de magnitud cero
    * @return Vector normalizado
    */
    Vector3 getNormalized() const;
    /**
    * @brief Normaliza este vector y devuelve la magnitud que tenia anteriormente.
    * No ocurre nada si su magnitud es 0
    * @return Magnitud del vector antes de ser normalizado
    */
    float normalize();
    /**
    * @brief Rota el vector sobre el eje x
    * @param degrees Grado que se quiere rotar el vector
    * @return Resultado de rotar el vector degrees grados sobre el eje x
    */
    Vector3 rotateX(const float degrees) const;
    /**
    * @brief Rota el vector sobre el eje y
    * @param degrees Grado que se quiere rotar el vector
    * @return Resultado de rotar el vector degrees grados sobre el eje y
    */
    Vector3 rotateY(const float degrees) const;
    /**
    * @brief Rota el vector sobre el eje z
    * @param degrees Grado que se quiere rotar el vector
    * @return Resultado de rotar el vector degrees grados sobre el eje z
    */
    Vector3 rotateZ(const float degrees) const;
    /**
    * @brief Producto en cruz, devuelve el vector perpendicular.Sistema diestro
    * @param other Vector por el que se quiere multiplicar
    * @return Vector perpendicular a los vectores utilizados
    */
    Vector3 cross(const Vector3 other) const;
    /**
    * @brief Devuelve el producto escalar entre este vector y otro
    * @return Producto escalar entre los vectores entre este vector y otro
    */
    float dot(const Vector3 other) const;

    /**
    * @brief Devuelve el vector resultante de la suma de este vector con otro
    * @param other Vector con el que se quiere sumar
    * @return Vector resultante de la suma
    */
    inline Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
    /**
    * @brief Devuelve el vector resultante de este vector cambiandole el signo
    * @return Vector resultante de este vector cambiandole el signo
    */
    inline Vector3 operator-() const { return Vector3(-x, -y, -z); }
    /**
    * @brief Devuelve el vector resultante de la resta de este vector con otro
    * @param other Vector con el que se quiere restar
    * @return Vector resultante de la resta
    */
    inline Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    /**
    * @brief Devuelve el vector resultante de la multiplicacion de este vector con un escalar
    * @param other Escalar con el que se quiere multiplicar
    * @return Vector resultante de la multiplicacion
    */
    inline Vector3 operator*(const float& other) const { return Vector3(x * other, y * other, z * other); }
    /**
    * @brief Devuelve el vector resultante de la multiplicacion de este vector con otro
    * @param other Vector con el que se quiere multiplicar
    * @return Vector resultante de la multiplicacion
    */
    inline Vector3 operator*(const Vector3& other) const {
        return Vector3((y * other.z) - (z * other.y), -((z * other.x) - (x * other.z)), (x * other.y) - (y * other.x));
    }
    /**
    * @brief Devuelve el vector resultante de la division de este vector con un escalar
    * @param other Escalar con el que se quiere dividir
    * @return Vector resultante de la division
    */
    inline Vector3 operator/(const float& other) const { return Vector3(x / other, y / other, z / other); }

    /**
    * @brief Devuelve el vector resultante de la suma de este vector con otro
    * @param other Vector con el que se quiere sumar
    * @return Vector resultante de la suma
    */
    inline Vector3& operator+=(const Vector3& other) { return *this = *this + other; }
    /**
    * @brief Devuelve el vector resultante de la resta de este vector con otro
    * @param other Vector con el que se quiere restar
    * @return Vector resultante de la resta
    */
    inline Vector3& operator-=(const Vector3& other) { return *this = *this - other; }
    /**
    * @brief Devuelve el vector resultante de la multiplicacion de este vector con un escalar
    * @param other Escalar con el que se quiere multiplicar
    * @return Vector resultante de la multiplicacion
    */
    inline Vector3& operator*=(const float& other) { return *this = *this * other; }
    /**
    * @brief Devuelve el vector resultante de la division de este vector con otro
    * @param other Vector con el que se quiere dividir
    * @return Vector resultante de la division
    */
    inline Vector3& operator*=(const Vector3& other) { return *this = *this * other; }
    /**
    * @brief Devuelve el vector resultante de la division de este vector con un escalar
    * @param other Escalar con el que se quiere dividir
    * @return Vector resultante de la division
    */
    inline Vector3& operator/=(const float& other) { return *this = *this / other; }

    /**
    * @brief Devuelve si este vector es igual a otro
    * @param other Vector con el que se quiere comparar
    * @return true si son iguales, false si no
    */
    inline bool operator==(const Vector3& other) const { return (x == other.x && y == other.y && z == other.z); }
    /**
    * @brief Devuelve si este vector es diferente a otro
    * @param other Vector con el que se quiere comparar
    * @return true si son diferentes, false si no
    */
    inline bool operator!=(const Vector3& other) const { return !(*this == other); }

    /**
    * @brief Devuelve la interpolacion entre este vector y otro segun un factor de interpolacion
    * @param end Punto hasta el que se quiere interpolar.
    * @param t factor de interpolacion (debe ser un valor entre 0 y 1. Si no, se redondeara al limite mas cercano)
    * @return Valor interpolado entre los 2 valores con el factor t 
    */
    Vector3 lerp(Vector3 const& end, float t) const;

    /**
    * @brief Devuelve la distancia entre este vector y otro
    * @param other Vector hasta el que se quiere calcular la distancia.
    * @return Distancia entre los 2 vectores
    */
    float distance(Vector3 const& other) const;

    operator Vector2() const;
};
}
