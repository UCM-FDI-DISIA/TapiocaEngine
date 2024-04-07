#include "Quaternion.h"

#include <cmath>
#define PI 3.14159265358979323846f

namespace Tapioca {

Quaternion::Quaternion(const float q0, const float q1, const float q2, const float q3) {
    scalar = q0;
    vector = Vector3(q1, q2, q3);
    angle = 2 * acosf(q0);
}

Quaternion::Quaternion(const float alfa, const Vector3 vec) {
    //primero hay que convertir vec en un vector unitario
    Vector3 uvec = vec / vec.magnitude();
    float alfarad =
        alfa * (PI / 180);   //las funciones de la libreria math cos y sin proporcionan resultados en radianes
    angle = alfarad;
    scalar = cosf(alfarad / 2);
    float alfasin = sinf(alfarad / 2);
    vector = Vector3(alfasin * uvec.x, alfasin * uvec.y, alfasin * uvec.z);
}

Quaternion::Quaternion(const Vector3 euler) {
    //la libreria math opera en radianes
    float roll = euler.x * (PI / 180);
    float cosroll = cosf(roll / 2);
    float sinroll = sinf(roll / 2);

    float yaw = euler.y * (PI / 180);
    float cosyaw = cosf(yaw / 2);
    float sinyaw = sinf(yaw / 2);

    float pitch = euler.z * (PI / 180);
    float cospitch = cosf(pitch / 2);
    float sinpitch = sinf(pitch / 2);

    scalar = cosroll * cosyaw * cospitch + sinroll * sinyaw * sinpitch;

    vector.x = sinroll * cosyaw * cospitch - cosroll * sinyaw * sinpitch;

    vector.y = cosroll * sinyaw * cospitch + sinroll * cosyaw * sinpitch;

    vector.z = cosroll * cosyaw * sinpitch - sinroll * sinyaw * cospitch;

    angle = 2 * acosf(scalar);
}

Quaternion Quaternion::inverse() { return conjugate() / magnitude(); }

Quaternion Quaternion::conjugate() { return Quaternion(scalar, -vector.x, -vector.y, -vector.z); }

float Quaternion::magnitude() {
    return sqrtf(scalar * scalar + vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

Vector3 Quaternion::euler() {
    // normalize();

    // usamos los angulos de los ejes con el vector del cuaternion
    // asume un cuaternion normalizado
    float sinx = 2.0f * (scalar * vector.x + vector.y * vector.z);
    float cosx = 1.0f - 2.0f * (vector.x * vector.x + vector.y * vector.y);
    float x = atan2f(sinx, cosx);

    float siny = sqrtf(1.0f + 2.0f * (scalar * vector.y - vector.x * vector.z));
    float cosy = sqrtf(1.0f - 2.0f * (scalar * vector.y - vector.x * vector.z));

    float y = 2.0f * atan2f(siny, cosy) - PI / 2.0f;

    float sinz = 2.0f * (scalar * vector.z + vector.x * vector.y);
    float cosz = 1.0f - 2.0f * (vector.y * vector.y + vector.z * vector.z);
    float z = atan2f(sinz, cosz);

    // creo que esta bien para un sistema diestro como todo lo demas ,
    // si no puede que haya que devolver -z
    return Vector3(x * (180.0f / PI), y * (180.0f / PI), z * (180.0f / PI));
}

Quaternion Quaternion::operator*(const Quaternion rhs) {
    float newScalar;
    Vector3 newVector;
    // Vector3 c = vector.cross(rhs.vector);
    // float d = vector.dot(rhs.vector);
    // newVector = vector.cross(rhs.vector) +  rhs.vector*scalar + vector*rhs.scalar;
    // newScalar = scalar * rhs.scalar - vector.dot(rhs.vector);
    // producto de hamilton (un man irlandes >:t)
    newScalar = scalar * rhs.scalar - vector.x * rhs.vector.x - vector.y * rhs.vector.y - vector.z * rhs.vector.z;
    newVector =
        Vector3((scalar * rhs.vector.x + vector.x * rhs.scalar + vector.y * rhs.vector.z - vector.z * rhs.vector.y),
                (scalar * rhs.vector.y - vector.x * rhs.vector.z + vector.y * rhs.scalar + vector.z * rhs.vector.x),
                (scalar * rhs.vector.z + vector.x * rhs.vector.y - vector.y * rhs.vector.x + vector.z * rhs.scalar));
    return Quaternion(newScalar, newVector.x, newVector.y, newVector.z);
}

Quaternion Quaternion::operator*(const float s) {
    return Quaternion(scalar * s, vector.x * s, vector.y * s, vector.z * s);
}

Quaternion Quaternion::operator/(const float s) {
    return Quaternion(scalar / s, vector.x / s, vector.y / s, vector.z / s);
}

Vector3 Quaternion::rotatePoint(const Vector3 point) {

    //Vector3 v2 = vector * 2.0;
    //Vector3 vs = vector * scalar;
    //Vector3 v3 = v+ v2.cross(vs + vector.cross(v) )/norm();
    // return v + (vector.cross(v))*2 * scalar +  vector.cross(vector.cross(v))*2;

    //el vector debe ser unitario
    // axis = axis/axis.magnitude();
    //math usa radianes
    //degrees = degrees * (PI / 180);
    // return( point * cos(degrees)) + (axis.cross(point)) * sin(degrees) + axis * ((axis.dot(point)*(1-cos(degrees))));
    //return point + axis.cross(axis.cross(point)) * (1 - cos(degrees)) + axis.cross(point) * sin(degrees);

    //El elemento neutro  es 1 0i 0k 0j
    //En este caso el punto no rota
    if (!(vector.x || vector.y || vector.z) || !angle) {
        return point;
    }

    /* angle = 1;
     float s = sinf(angle / 2); 
   double x = vector.x / sinf(angle / 2);
    double y = vector.y / sinf(angle / 2);
   double z = vector.z/ sinf(angle / 2);
   */
    Vector3 axis = vector / sinf(angle / 2);

    return point + axis.cross(axis.cross(point) * (1 - cosf(angle))) + axis.cross(point) * sinf(angle);
}

Quaternion Quaternion::normalized() { return *this / magnitude(); }

void Quaternion::normalize() { (*this) = *this / magnitude(); }

}
