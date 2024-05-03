#pragma once
#include "defs.h"

namespace Tapioca {
/**
* Clase que implementa un vector geometrico de dos dimensiones
*/
class TAPIOCA_API Vector2 {
public:
    float x; // Coordenada x del vector
    float y; // Coordenada y del vector

    /**
    * @brief Constructor de la clase Vector2.
    * Genera un vector que inicializa "x" e "y" al numero dado. 
    * Si no se da un numero, este se considera cero
    * @param xy Valor al que se inicializan "x" e "y"
    */
    Vector2(const float xy = 0);
    /**
    * @brief Constructor de la clase Vector2.
    * Genera un vector que inicializa "x" e "y" a sus valores correspondientes
    * @param x Valor al que se inicializa "x"
    * @param y Valor al que se inicializa "y"
    */
    Vector2(const float x, const float y);
    /**
    * @brief Constructor de la clase Vector2.
    * Genera una copia del vector dado
    * @param other Vector que se quiere copiar
    */
    Vector2(const Vector2& other);

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
    Vector2 getNormalized() const;
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
    inline Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
    /**
    * @brief Devuelve el vector resultante de este vector cambiandole el signo
    * @return Vector resultante de este vector cambiandole el signo
    */
    inline Vector2 operator-() const { return Vector2(-x, -y); }
    /**
    * @brief Devuelve el vector resultante de la resta de este vector con otro
    * @param other Vector con el que se quiere restar
    * @return Vector resultante de la resta
    */
    inline Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
    /**
    * @brief Devuelve el vector resultante de la multiplicacion de este vector por un escalar
    * @param other Escalar por el que se quiere multiplicar
    * @return Vector resultante de la multiplicacion
    */
    inline Vector2 operator*(const float& other) const { return Vector2(x * other, y * other); }
    /**
    * @brief Devuelve el vector resultante de la division de este vector por un escalar
    * @param other Escalar por el que se quiere dividir
    * @return Vector resultante de la division
    */
    inline Vector2 operator/(const float& other) const { return Vector2(x / other, y / other); }

    /**
    * @brief Devuelve el vector resultante de la suma de este vector con otro
    * @param other Vector con el que se quiere sumar
    * @return Vector resultante de la suma
    */
    inline Vector2& operator+=(const Vector2& other) { return *this = *this + other; }
    /**
    * @brief Devuelve el vector resultante de la resta de este vector con otro
    * @param other Vector con el que se quiere restar
    * @return Vector resultante de la resta
    */
    inline Vector2& operator-=(const Vector2& other) { return *this = *this - other; }
    /**
    * @brief Devuelve el vector resultante de la multiplicacion de este vector por un escalar
    * @param other Escalar por el que se quiere multiplicar
    * @return Vector resultante de la multiplicacion
    */
    inline Vector2& operator*=(const float& other) { return *this = *this * other; }
    /**
    * @brief Devuelve el vector resultante de la division de este vector por un escalar
    * @param other Escalar por el que se quiere dividir
    * @return Vector resultante de la division
    */
    inline Vector2& operator/=(const float& other) { return *this = *this / other; }

    /**
    * @brief Devuelve si este vector es igual a otro
    * @param other Vector con el que se quiere comparar
    * @return true si son iguales, false si no
    */
    inline bool operator==(const Vector2& other) const { return (x == other.x && y == other.y); }
    /**
    * @brief Devuelve si este vector es distinto a otro
    * @param other Vector con el que se quiere comparar
    * @return true si son distintos, false si no
    */
    inline bool operator!=(const Vector2& other) const { return !(*this == other); }

    /**
    * @brief Devuelve la interpolacion entre este vector y otro segun un factor de interpolacion
    * @param end Punto hasta el que se quiere interpolar.
    * @param t factor de interpolacion (debe ser un valor entre 0 y 1. Si no, se redondeara al limite mas cercano)
    * @return Valor interpolado entre los 2 valores con el factor t 
    */
    Vector2 lerp(Vector2 const& end, float t) const;

    /**
    * @brief Devuelve la distancia entre este vector y otro
    * @param other Vector hasta el que se quiere calcular la distancia
    * @return Distancia entre este vector y otro
    */
    float distance(Vector2 const& other) const;

    /**
    * @brief Limita un valor entre un minimo y un maximo
    * @param v Valor que se quiere limitar
    * @param min Valor minimo al que se puede limitar
    * @param max Valor maximo al que se puede limitar
    * @return Valor limitado entre min y max
    */
    static float clamp(float v, float min, float max);
};
}