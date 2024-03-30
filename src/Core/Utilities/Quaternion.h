#pragma once
#include "Vector3.h"

// q = x + yi + zj +wk
//donde x y z w son numeros reales y ddone i j k son 
//numeros complejos , vectores unitarios ortogonales entre si. i^2 = j^2 = k^2 = -1
namespace Tapioca {
/*
    * @brief Usamos el sistema diestro https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html.
    * Cualquier rotacion en un espacio tridimensional se puede definir
    * con un vector unitario y una cantidad de giro sobre ese vector
*/
class TAPIOCA_API Quaternion {
public:
    float scalar;//(W)

    Vector3 vector;//(X,Y,Z)
    // las componentes individuales seran W X Y Z Q(WXYZ)
    
    float angle;// Angulo en radianes

    //para pasar de  eje + angulo  a quaternion
    // 0 + (x,y,z )
    // q = q0 + q1 + q2 + q3 ,
    // q0 = cos(0/2), q1 = xsin(0/2), q2 = ysin(0/2), q3 = zsin(0/2)

    //para rotar un punto se multiplica por el cuatrenion y lueg por su conjugado pr = q*p*qc
    /*
    * @brief Constructora de la clase Quaternion.
    * Se contruye un a partir de 4 floats que describen el angulo y vector que definen el quaternion
    * @param q0 Cantidad de giro que se aplica sobre el vector
    * @param q1 Valor de la componente x del vector
    * @param q2 Valor de la componente y del vector
    * @param q3 Valor de la componente z del vector
    */
    Quaternion(const float q0, const float q1, const float q2, const float q3);
    /*
    * @brief Constructora de la clase Quaternion. 
    * Se construye a partir de la descripcion la rotacion de alfa grados en el eje vec
    * @params alfa Rotacion del vector en grados sexagesimales
    * @params vec Vector que define el giro
    */
    Quaternion(const float alfa, const Vector3 vec);

    /*
    * @brief Constructora de la clase Quaternion. 
    * Se construye a partir de rotaciones en los 3 ejes globales (x y z) en grados sexagesimales
    * @param euler Rotacion en los 3 ejes globales
    */
    Quaternion(const Vector3 euler);
    /*
    * @brief Devuelve el quaternion inverso (el conjugado entre la magnitud)
    * @return Inverso del quaternion
    */
    Quaternion inverse();
    /*
    * @brief Devuelve el cuaternion conjugado
    * @return Quaternion conjugado
    */
    Quaternion conjugate();
    /*
    * @brief devuelve la magnitud del quaternion 
    * @return Magnitud del quaternion 
    */
    float norm();
    /*
    * @brief Devuelve un vector con la rotacion en angulos sexadecimales  pitch , yaw, roll (x,y,z) en un sistema y up
    * @return Vector con la rotacion en angulos sexadecimales
    */
    Vector3 euler();

    /*
    * @brief Multiplica el cuaternion por rhs.
    * Para rotar un punto se puede hacer q * p * q(conjugado).
    * @param rhs Elemento diestro de la multiplicacion
    * @return Resultado de multiplicar el quaternion por rhs
    */
    Quaternion operator*(const Quaternion rhs);//right hand side

    /*
    * @brief Multiplicacio por un escalar
    * @param s Escalar por el que se multiplican todos los miembros del quaternion
    * @return Resultado de multiplicar el quaternion por s
    */
    Quaternion operator*(const float s);

    /*
    * @brief Divide el cuaternion por un escalar
    * Para rotar un punto se puede hacer q * p * q(conjugado).
    * @param s Escalar por el que se dividen todos los miembros del quaternion
    * @return Resultado de multiplicar el quaternion por rhs
    */
    Quaternion operator/(const float s);

    /*
    * @brief multiplicacion "vectorizada" con la "Formula Rodrigues" 
    * Para rotar un punto se puede hacer q * p * q(conjugado).
    * @param point Punto por el que se multiplica
    * @return Resultado de multiplicar el quaternion por el punto 
    */
    Vector3 rotatePoint(const Vector3 point);
    
    /*
    * @brief Construye un quaternon representando la rotacion desde una direcccion 
    * hacia otra, usando el forward por defecto de la direccion origen
    * @param forward Forward de la direccion a la que se quiere mirar
    * @param upwards Up de la direccion a la que se quiere mirar
    * @return Quaternion que representa la rotacion desde el origen al destino
    */
    static Quaternion lookAt(const Vector3& forward, const Vector3& upwards);
};
}

