#include "Transform.h"
#include "Structure/GameObject.h"
#include "Utilities/INode.h"

namespace Tapioca {

Transform::Transform() : Component(), position(Vector3(0)), rotation(Vector3(0)), scale(Vector3(1)), node(nullptr) { }

Transform::~Transform() {
    object->die();
    for (auto childNode : node->getAllChildren()) {
        Tapioca::GameObject* childGameObject = childNode->getTransform()->getObject();
        childGameObject->die();
    }
    delete node;
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
    node->setPosition(position);

    bool scaleSet = setValueFromMap(scale.x, "scaleX", variables) && setValueFromMap(scale.y, "scaleY", variables) &&
        setValueFromMap(scale.z, "scaleZ", variables);
    if (!scaleSet) {
#ifdef _DEBUG
        std::cerr << "Error: Transform: no se pudo inicializar la escala.\n";
#endif
        return false;
    }
    node->setScale(scale);

    bool rotationSet = setValueFromMap(rotation.x, "rotationX", variables) &&
        setValueFromMap(rotation.y, "rotationY", variables) && setValueFromMap(rotation.z, "rotationZ", variables);
    if (!rotationSet) {
#ifdef _DEBUG
        std::cerr << "Error: Transform: no se pudo inicializar la rotacion.\n";
#endif
        return false;
    }
    node->setRotation(rotation);
}

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

void Transform::setParent(Transform* tranform) { node->setParent(tranform->getNode()); }

Transform* Transform::getParent() const { return node->getParent()->getTransform(); }

std::vector<Transform*> Transform::getChildren() const {
    std::vector<INode*> childrenNodes = node->getChildren();
    std::vector<Transform*> children;
    children.reserve(childrenNodes.size());

    for (INode* node : childrenNodes)
        children.push_back(node->getTransform());

    return children;
}

std::vector<Transform*> Transform::getAllChildren() const {
    std::vector<INode*> childrenNodes = node->getAllChildren();
    std::vector<Transform*> children;
    children.reserve(childrenNodes.size());

    for (INode* node : childrenNodes)
        children.push_back(node->getTransform());

    return children;
}
void Transform::fixedUpdate() {
    //TODO quitar esto que era para probar un cosa
    node->yaw(1);
}
}
