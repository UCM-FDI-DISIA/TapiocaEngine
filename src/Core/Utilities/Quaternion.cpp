#include "Quaternion.h"
#include "math.h"
#include "Vector3.h"
#define PI 3.14159265358979323846
Tapioca::Quaternion::Quaternion(float q0, float q1, float q2, float q3) { 
scalar = q0;
vector = Vector3(q1, q2, q3);
angle = 2 * acos(q0);
}

Tapioca::Quaternion::Quaternion(float alfa, Vector3 vec) { 
	//primero hay que convertir vec en un vector unitario
	Vector3 uvec = vec / vec.magnitude();
	float alfarad = alfa * (PI / 180);//las funciones de la libreria math cos y sin proporcionan resultados en radianes
    angle = alfarad;
	scalar = cos(alfarad / 2.0); 
	float alfasin = sin(alfarad / 2.0);
    vector = Vector3(alfasin * uvec.x, alfasin * uvec.y, alfasin * uvec.z);
}

Tapioca::Quaternion Tapioca::Quaternion::inverse() { 
	return  conjugate()/norm(); }

Tapioca::Quaternion Tapioca::Quaternion::conjugate() { 
	
	return Tapioca::Quaternion(scalar,-vector.x,-vector.y,-vector.z); 

}

float  Tapioca::Quaternion::norm() {
	return sqrt(scalar*scalar + vector.x*vector.x + vector.y*vector.y + vector.z*vector.z ); 
}

Tapioca::Vector3 Tapioca::Quaternion::euler() { 
//usamos los angulos de los ejes con el vector del cuaternion
// asume un cuaternion normalizado
        float sinx = 2 * (scalar * vector.x + vector.y * vector.z);
        float cosx = 1 - 2 * (vector.x * vector.x + vector.y * vector.y);
        float x = atan2f(sinx,cosx);

		float siny= sqrtf(1 + 2*(scalar*vector.y - vector.x*vector.z));
        float cosy= sqrtf(1 - 2 *(scalar*vector.y - vector.x*vector.z));
		float y = 2 * atan2f(siny,cosy) - PI/2;

		float sinz = 2*(scalar*vector.z + vector.x*vector.y);
        float cosz= 1 - 2*(vector.y * vector.y + vector.z*vector.z);
        float z = atan2f(sinz,cosz);
		//creo que esta bien para un sistema diestro como todo lo demas ,
		// si no puede que haya que devolver -z
        return Vector3(x * (180 / PI), y * (180/PI), z*(180/PI)); 

}

Tapioca::Quaternion Tapioca::Quaternion::operator*(const Quaternion rhs) { 
	float newScalar;
    Vector3 newVector;
    //Vector3 c = vector.cross(rhs.vector);
    //float d = vector.dot(rhs.vector);
	//newVector = vector.cross(rhs.vector) +  rhs.vector*scalar + vector*rhs.scalar;
    //newScalar = scalar * rhs.scalar - vector.dot(rhs.vector);
	//producto de hamilton
    newScalar = scalar * rhs.scalar - vector.x * rhs.vector.x - vector.y * rhs.vector.y - vector.z * rhs.vector.z;
    newVector = Vector3(
        (scalar * rhs.vector.x + vector.x * rhs.scalar + vector.y * rhs.vector.z - vector.z * rhs.vector.y), 
		(scalar*rhs.vector.y - vector.x*rhs.vector.z + vector.y*rhs.scalar + vector.z*rhs.vector.x),
		(scalar*rhs.vector.z + vector.x*rhs.vector.y - vector.y*rhs.vector.x + vector.z*rhs.scalar));
	return Quaternion(newScalar,newVector.x,newVector.y,newVector.z);
}

Tapioca::Quaternion Tapioca::Quaternion::operator/(const float s) { 
	return Quaternion(scalar/s,vector.x/s,vector.y/s,vector.z/s); 
}

Tapioca::Vector3 Tapioca::Quaternion::rotatePoint(Vector3 point) { 
	
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
        Vector3 axis = vector / sin(angle/2);
        return point + axis.cross(axis.cross(point) * (1 - cos(angle))) + axis.cross(point) * sin(angle);
}

