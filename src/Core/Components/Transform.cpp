#include "Transform.h"

namespace Tapioca {

Transform::Transform()
    : Component() { }   //PRUEBA

void Transform::initComponent() {   //PRUEBA
    position = Vector3(0);
    rotation = Vector3(0);
    scale = Vector3(1);
}

Vector3 Transform::getPosition() { return position; }
Vector3 Transform::getRotation() { return rotation; }
Vector3 Transform::getScale() { return scale; }

void Transform::setPosition(Vector3 p) { position = p; }
void Transform::setRotation(Vector3 r) { rotation = r; }
void Transform::setScale(Vector3 s) { scale = s; }

void Transform::translate(Vector3 p) { position += p; }
void Transform::rotate(Vector3 r) { rotation += r; }

Vector3 Transform::right() {

    Vector3 v(1, 0, 0);   //X

    v = v.rotateX(rotation.x);
    v = v.rotateY(rotation.y);
    v = v.rotateZ(rotation.z);

    v.normalize();

    return v;
}
Vector3 Transform::up() {

    Vector3 v(0, 1, 0);   //Y

    v = v.rotateX(rotation.x);
    v = v.rotateY(rotation.y);
    v = v.rotateZ(rotation.z);

    v.normalize();

    return v;
}
Vector3 Transform::forward() {

    Vector3 v(0, 0, 1);   //Z

    v = v.rotateX(rotation.x);
    v = v.rotateY(rotation.y);
    v = v.rotateZ(rotation.z);

    v.normalize();

    return v;
}

}
