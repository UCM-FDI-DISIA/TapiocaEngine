#include "Vector3.h"
#include "Vector2.h"
#include "Vector2.h"
#include <cmath>
#include <math.h>
#define PI 3.14159265358979323846 


namespace Tapioca {

Vector3::Vector3(const float xyz) : x(xyz), y(xyz), z(xyz) { }

Vector3::Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) { }

Vector3::Vector3(const Vector3& other) : x(other.x) , y(other.y) , z(other.z) { }

Vector3::Vector3(const Vector2& other, const float z) : x(other.x), y(other.y), z(z) { }

float Vector3::magnitudeSquared() const { return x * x + y * y + z * z; }

float Vector3::magnitude() const { return sqrtf(magnitudeSquared()); }

Vector3 Vector3::getNormalized() const {
    float mag = magnitude();
    return mag == 0 ? Vector3() : Vector3(x / mag, y / mag, z / mag);
}

float Vector3::normalize() {
    float mag = magnitude();
    if (mag != 0) {
        x /= mag;
        y /= mag;
        z /= mag;
    }
    return mag;
}

Vector3 Vector3::rotateX(const float degrees) {
    return Vector3(x, cos(degrees) * y - sin(degrees) * z, sin(degrees) * y + cos(degrees) * z);
}

Vector3 Vector3::rotateY(const float degrees) {
    return Vector3(cos(degrees) * x + sin(degrees) * z, y, -sin(degrees) * x + cos(degrees) * z);
}

Vector3 Vector3::rotateZ(const float degrees) {
    return Vector3(cos(degrees) * x - sin(degrees) * y, sin(degrees) * x + cos(degrees) * y, z);
}

Vector3 Vector3::cross(const Vector3 other) { 
   // float cosalfa = dot(other) / (magnitude() * other.magnitude());
   // float alfa =  acos(cosalfa);//pero esto lo da en radianes
    //alfa *= (180 /PI );
    //return magnitude() * other.magnitude() * sin(alfa); 
    return Vector3((y * other.z - z * other.y), (z * other.x - x * other.z), (x * other.y - y * other.x));

}

float Vector3::dot(const Vector3 other) { 
    return x * other.x + y * other.y + z * other.z; 

}

}
