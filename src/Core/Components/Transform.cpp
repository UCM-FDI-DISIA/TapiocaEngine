#include "Transform.h"
#include "Structure/GameObject.h"
#include "Utilities/INode.h"

namespace Tapioca {
Transform::Transform() : Component(), position(Vector3(0)), rotation(Vector3(0)), scale(Vector3(1)), parent(nullptr) { }

Transform::~Transform() {
    parent->deleteChild(this);
    object->die();
    for (auto childNode : getAllChildren()) {
        Tapioca::GameObject* childGameObject = childNode->getObject();
        childGameObject->die();
    }
}

bool Transform::initComponent(const CompMap& variables) {
    bool positionSet = setValueFromMap(position.x, "positionX", variables) &&
        setValueFromMap(position.y, "positionY", variables) && setValueFromMap(position.z, "positionZ", variables);
    if (!positionSet) {
#ifdef _DEBUG
        std::cerr << "Error: Transform: no se pudo inicializar la posicion.\n";
#endif
        return false;
    }
    moved();

    bool rotationSet = setValueFromMap(rotation.x, "rotationX", variables) &&
        setValueFromMap(rotation.y, "rotationY", variables) && setValueFromMap(rotation.z, "rotationZ", variables);
    if (!rotationSet) {
#ifdef _DEBUG
        std::cerr << "Error: Transform: no se pudo inicializar la rotacion.\n";
#endif
        return false;
    }
    rotated();

    bool scaleSet = setValueFromMap(scale.x, "scaleX", variables) && setValueFromMap(scale.y, "scaleY", variables) &&
        setValueFromMap(scale.z, "scaleZ", variables);
    if (!scaleSet) {
#ifdef _DEBUG
        std::cerr << "Error: Transform: no se pudo inicializar la escala.\n";
#endif
        return false;
    }
    scaled();
}

void Transform::setPosition(Vector3 p) {
    position = p;
    moved();
}
void Transform::setRotation(Vector3 r) {
    rotation = r;
    rotated();
}
void Transform::setScale(Vector3 s) {
    scale = s;
    scaled();
}

void Transform::translate(Vector3 p) {
    position += p;
    moved();
}
void Transform::rotate(Vector3 r) {
    rotation += r;
    rotated();
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

void Transform::deleteChild(Transform* child) {
    auto it = children.find(child);
    if (it != children.end()) children.erase(it);
}

void Transform::setParent(Transform* transform) {
    if (parent != nullptr) parent->deleteChild(this);
    parent = transform;
}

Transform* Transform::getParent() const { return parent; }

std::vector<Transform*> Transform::getChildren() const {
    std::vector<Transform*> aux;
    std::copy(children.begin(), children.end(), aux.begin());
    return aux;
}

std::vector<Transform*> Transform::getAllChildren() const {
    std::vector<Transform*> allChildren;

    getAllChildrenAux(allChildren);

    return allChildren;
}

void Transform::getAllChildrenAux(std::vector<Transform*>& allChildren) const {
    for (auto child : children) {
        allChildren.push_back(child);
        child->getAllChildrenAux(allChildren);
    }
}

void Transform::fixedUpdate() {

}
}
