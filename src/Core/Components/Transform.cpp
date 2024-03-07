#include "Transform.h"
#include "Structure/GameObject.h"
#include "Utilities/INode.h"

namespace Tapioca {
void Transform::getAllChildrenAux(std::vector<Transform*>& allChildren) const {
    for (auto child : children) {
        allChildren.push_back(child);
        child->getAllChildrenAux(allChildren);
    }
}

void Transform::addChild(Transform* const child) {
    if (!children.contains(child)) children.insert(child);
}

void Transform::removeChild(Transform* const child) {
    auto it = children.find(child);
    if (it != children.end()) children.erase(it);
}

void Transform::removeConnections() {
    removeParent();
    // eliminar todos los hijos directos
    for (auto it = children.begin(), itAnt = children.begin(); it != children.end();) {
        ++itAnt;
        (*it)->removeParent();
        it = itAnt;
    }
    children.clear();
}

void Transform::removeParent() {
    if (parent != nullptr) {
        parent->removeChild(this);
    }
    parent = nullptr;
}

Transform::Transform() : Component(), position(Vector3(0)), rotation(Vector3(0)), scale(Vector3(1)), parent(nullptr) { }

Transform::~Transform() {
    for (Transform* child : getAllChildren()) {
        GameObject* childGameObject = child->getObject();
        childGameObject->die();
    }

    removeConnections();
    object->die();
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

    return true;
}

Vector3 Transform::getGlobalPosition() const {
    Vector3 aux = position;
    if (parent != nullptr) {
        aux = aux + parent->getGlobalPosition();
    }
    return aux;
}

Vector3 Transform::getGlobalRotation() const {
    Vector3 aux = rotation;
    if (parent != nullptr) {
        aux = aux + parent->getGlobalRotation();
    }
    return aux;
}

Vector3 Transform::getGlobalScale() const {
    Vector3 aux = scale;
    if (parent != nullptr) {
        Vector3 parentScale = parent->getGlobalScale();
        aux = Vector3(aux.x * parentScale.x, aux.y * parentScale.y, aux.z * parentScale.z);
    }
    return aux;
}

void Transform::setPosition(const Vector3& p) {
    position = p;
    moved();
}
void Transform::setRotation(const Vector3& r) {
    rotation = r;
    rotated();
}
void Transform::setScale(const Vector3& s) {
    scale = s;
    scaled();
}

void Transform::translate(const Vector3& p) {
    position += p;
    moved();
}
void Transform::rotate(const Vector3& r) {
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

void Transform::setParent(Transform* const transform) {
    removeParent();
    parent = transform;
    parent->addChild(this);
}

Transform* Transform::getParent() const { return parent; }

std::vector<Transform*> Transform::getChildren() const {
    std::vector<Transform*> aux(children.size());
    std::copy(children.begin(), children.end(), aux.begin());
    return aux;
}

std::vector<Transform*> Transform::getAllChildren() const {
    std::vector<Transform*> allChildren;
    getAllChildrenAux(allChildren);
    return allChildren;
}
}