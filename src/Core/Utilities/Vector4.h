#pragma once
#include "defs.h"

namespace Tapioca {
class Vector2;
class Vector3;

/**
* Clase que implementa un vector geometrico de cuatro dimensiones
*/
class TAPIOCA_API Vector4 {
public:
    float x; // Coordenada x del vector
    float y; // Coordenada y del vector
    float z; // Coordenada z del vector
    float w; // Coordenada w del vector

    /**
    * @brief Constructor de la clase Vector4.
    * Genera un vector que inicializa "x", "y", "z" y "w" al numero dado.
    * Si no se da un numero, este se considera cero
    * @param xyz Valor al que se inicializan "x", "y", "z" y "w"
    */
    Vector4(const float xyzw = 0);
    /**
    * @brief Constructor de la clase Vector4.
    * Genera un vector que inicializa "x", "y", "z" y "w" a sus valores correspondientes.
    * Si no se da un numero, este se considera cero
    * @param x Valor al que se inicializa "x"
    * @param y Valor al que se inicializa "y"
    * @param z Valor al que se inicializa "z"
    * @param w Valor al que se inicializa "w"
    */
    Vector4(const float x, const float y, const float z = 0, const float w = 0);
    /**
    * @brief Constructor de la clase Vector4.
    * Genera una copia del vector dado
    * @param other Vector que se quiere copiar
    */
    Vector4(const Vector4& other);
    /**
    * @brief Genera un vector con "x", "y" y "z" tal como estan en el vector dado.
    * "w" se inicializa segun el numero dado. Si no se da un numero, este se considera cero
    * @param other Vector3 del que se quiere copiar "x", "y" y "z"
    */
    Vector4(const Vector3& other, const float w = 0);
    /**
    * @brief Genera un vector con "x" e "y" tal como estan en el vector dado.
    * "z" y "w" se inicializan segun el numero dado. Si no se da algun numero, este se considera cero
    * @param other Vector2 del que se quiere copiar "x" e "y"
    * @param z Valor al que se inicializa "z"
    * @param w Valor al que se inicializa "w"
    */
    Vector4(const Vector2& other, const float z = 0, const float w = 0);

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
    Vector4 getNormalized() const;
    /**
    * @brief Normaliza este vector y devuelve la magnitud que tenia anteriormente.
    * No ocurre nada si su magnitud es 0
    * @return Magnitud del vector antes de ser normalizado
    */
    float normalize();

    /**
    * @brief Devuelve el vector resultante de la suma de este vector con otro
    * @param other Vector con el que se quiere sumar
    * @return Vector resultante de la suma
    */
    inline Vector4 operator+(const Vector4& other) const { return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
    /**
    * @brief Devuelve el vector resultante de este vector cambiandole el signo
    * @return Vector resultante de este vector cambiandole el signo
    */
    inline Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }
    /**
    * @brief Devuelve el vector resultante de la resta de este vector con otro
    * @param other Vector con el que se quiere restar
    * @return Vector resultante de la resta
    */
    inline Vector4 operator-(const Vector4& other) const { return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
    /**
    * @brief Devuelve el vector resultante de la multiplicacion de este vector por un escalar
    * @param other Escalar por el que se quiere multiplicar
    * @return Vector resultante de la multiplicacion
    */
    inline Vector4 operator*(const float& other) const { return Vector4(x * other, y * other, z * other, w * other); }
    /**
    * @brief Devuelve el vector resultante de la division de este vector por un escalar
    * @param other Escalar por el que se quiere dividir
    * @return Vector resultante de la division
    */
    inline Vector4 operator/(const float& other) const { return Vector4(x / other, y / other, z / other, w / other); }

    /**
    * @brief Devuelve el vector resultante de la suma de este vector con otro
    * @param other Vector con el que se quiere sumar
    * @return Vector resultante de la suma
    */
    inline Vector4& operator+=(const Vector4& other) { return *this = *this + other; }
    /**
    * @brief Devuelve el vector resultante de la resta de este vector con otro
    * @param other Vector con el que se quiere restar
    * @return Vector resultante de la resta
    */
    inline Vector4& operator-=(const Vector4& other) { return *this = *this - other; }
    /**
    * @brief Devuelve el vector resultante de la multiplicacion de este vector por un escalar
    * @param other Escalar por el que se quiere multiplicar
    * @return Vector resultante de la multiplicacion
    */
    inline Vector4& operator*=(const float& other) { return *this = *this * other; }
    /**
    * @brief Devuelve el vector resultante de la division de este vector por un escalar
    * @param other Escalar por el que se quiere dividir
    * @return Vector resultante de la division
    */
    inline Vector4& operator/=(const float& other) { return *this = *this / other; }

    /**
    * @brief Devuelve si este vector es igual a otro
    * @param other Vector con el que se quiere comparar
    * @return true si son iguales, false si no
    */
    inline bool operator==(const Vector4& other) const {
        return (x == other.x && y == other.y && z == other.z && w == other.w);
    }
    /**
    * @brief Devuelve si este vector es diferente a otro
    * @param other Vector con el que se quiere comparar
    * @return true si son diferentes, false si no
    */
    inline bool operator!=(const Vector4& other) const { return !(*this == other); }

    /**
    * @brief Devuelve la interpolacion entre este vector y otro segun un factor de interpolacion
    * @param end Punto hasta el que se quiere interpolar.
    * @param t factor de interpolacion (debe ser un valor entre 0 y 1. Si no, se redondeara al limite mas cercano)
    * @return Valor interpolado entre los 2 valores con el factor t 
    */
    Vector4 lerp(Vector4 const& end, float t) const;

    /**
    * @brief Devuelve la distancia entre este vector y otro
    * @param other Vector hasta el que se quiere calcular la distancia.
    * @return Distancia entre los 2 vectores
    */
    float distance(Vector4 const& other) const;
};
}
