#pragma once
#include "Vector3.h"

// q = x + yi + zj +wk
//donde x y z w son numeros reales y ddone i j k son 
//numeros complejos , vectores unitarios ortogonales entre si. i^2 = j^2 = k^2 = -1
/// usamos el sistema diestro https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html
namespace Tapioca {
class TAPIOCA_API Quaternion {
public:
    //cualquier rotacion en un espacio tridimensional se puede definir
    // con un vector unitario y una cantidad de giro sobre ese vector
    float scalar;//(W)

    Vector3 vector;//(X,Y,Z)
    // las componentes individuales seran W X Y Z Q(WXYZ)
    
    float angle;// se almacenara en radianes

    //para pasar de  eje + angulo  a quaternion
    // 0 + (x,y,z )
    // q = q0 + q1 + q2 + q3 ,
    // q0 = cos(0/2), q1 = xsin(0/2), q2 = ysin(0/2), q3 = zsin(0/2)

    //para rotar un punto se multiplica por el cuatrenion y lueg por su conjugado pr = q*p*qc

    Quaternion(float q0, float q1, float q2, float q3);
    /*
    * @brief  Quaternion que describe la rotacion de alfa grados en el eje vec
    *@params alfa en grados sexagesimales!!!! 
    */
    Quaternion(float alfa, Vector3 vec);

    /*
    * @brief construye un cuaternion a partir de rotaciones en los 3 ejes globales (x y z) en grados SEXAGESIMALES
    */
    Quaternion( Vector3 euler);
    /*
    * @brief devuelve el quaternion inverso que es el conjugado entre la magnitud 
    */
    Quaternion inverse();
    /*
    * @brief devuelve el cuaternion conjugado que es el mismo con la parte compleja negada
    */
    Quaternion conjugate();
    /*
    * @brief devuelve la magnitud del quaternion 
    */
    float norm();
    /*
    * @brief devuelve un vector con la rotacion en angulos sexadecimales  pitch , yaw, roll (x,y,z) en unsistema y up
    */
    Vector3 euler();

    /*La multiplicacion NO esc onmutativa, multiplcar por la derecha o por la izquierda no es lo mismo
    * @brief  multiplica el cuaternion por rhs;Para rotar un punto se puede hacer q * p * q(conjugado).
    * @param rhs elemento diestro de la multiplicacion
    */
    Quaternion operator*(const Quaternion rhs);//right hand side

    /*
    * @brief multiplicacio por un escalar
    * @param s escalar por el que se multiplican todos los miembros del quaternion
    */
    Quaternion operator*(const float s);

    /*
    * @divide todos los componentes del quaternion por el escalar s
    */
    
    Quaternion operator/(const float s);

    /*
    * @brief multiplicacion "vectorizada"  "Formula Rodrigues" 
    */
    Vector3 rotatePoint(Vector3 point);
};
}

