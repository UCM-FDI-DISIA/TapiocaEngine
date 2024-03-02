#include "Transform.h"
#include "Utilities/INode.h"

namespace Tapioca {

Transform::Transform() : Component(), position(Vector3(0)), rotation(Vector3(0)), scale(Vector3(1)) { }   //PRUEBA

void Transform::initComponent(const CompMap& variables) {
    setValueFromMap(position.x, "positionX", variables);
    setValueFromMap(position.y, "positionY", variables);
    setValueFromMap(position.z, "positionZ", variables);
    node->setPosition(position);
    setValueFromMap(scale.x, "scaleX", variables);
    setValueFromMap(scale.y, "scaleY", variables);
    setValueFromMap(scale.z, "scaleZ", variables);
    node->setScale(scale);
    setValueFromMap(rotation.x, "rotationX", variables);
    setValueFromMap(rotation.y, "rotationY", variables);
    setValueFromMap(rotation.z, "rotationZ", variables);
    node->setRotation(rotation);
}

Vector3 Transform::getPosition() { return position; }
Vector3 Transform::getRotation() { return rotation; }
Vector3 Transform::getScale() { return scale; }

void Transform::setPosition(Vector3 p) {
    position = p;
    node->setPosition(p);
}
void Transform::setRotation(Vector3 r) {
    rotation = r;
    node->setRotation(r);
}
void Transform::setScale(Vector3 s) {
    scale = s;
    node->setScale(s);
}

void Transform::translate(Vector3 p) {
    position += p;
    node->translate(p);
}
void Transform::rotate(Vector3 r) {
    rotation += r;
    node->setRotation(rotation);
}

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
