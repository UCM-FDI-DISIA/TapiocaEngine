#include "Transform.h"
#include "Structure/GameObject.h"

namespace Tapioca {
void Transform::changed(bool rb) { pushEvent("transformChanged", &rb, false); }

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

Transform::Transform()
    : Component(), position(Vector3(0)), rotation(Vector3(0.0f)), scale(Vector3(1.0f)), parent(nullptr) { }

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

    bool rotationSet = setValueFromMap(rotation.x, "rotationX", variables) &&
        setValueFromMap(rotation.y, "rotationY", variables) && setValueFromMap(rotation.z, "rotationZ", variables);
    if (!rotationSet) {
#ifdef _DEBUG
        std::cerr << "Error: Transform: no se pudo inicializar la rotacion.\n";
#endif
        return false;
    }

    bool scaleSet = setValueFromMap(scale.x, "scaleX", variables) && setValueFromMap(scale.y, "scaleY", variables) &&
        setValueFromMap(scale.z, "scaleZ", variables);
    if (!scaleSet) {
#ifdef _DEBUG
        std::cerr << "Error: Transform: no se pudo inicializar la escala.\n";
#endif
        return false;
    }

    //initialRotation = rotation;

    return true;
}

void Transform::start() { changed(); }

void Transform::handleEvent(std::string const& id, void* info) {
    if (id == "transformChanged") {
        for (Transform* child : children) {
            child->pushEvent("transformChanged", info, false);
        }
    }
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
        aux = parent->getGlobalRotation() + aux;
    }

    return aux;
    /*Quaternion aux = Quaternion(rotation);
    if (parent != nullptr) {
        aux = Quaternion(parent->getGlobalRotation()) * aux;
    }
    return aux.euler();*/
}

Vector3 Transform::getGlobalScale() const {
    Vector3 aux = scale;
    if (parent != nullptr) {
        Vector3 parentScale = parent->getGlobalScale();
        aux = Vector3(aux.x * parentScale.x, aux.y * parentScale.y, aux.z * parentScale.z);
    }
    return aux;
}

void Transform::setPosition(const Vector3& p, bool rb) {
    position = p;
    changed(rb);
}
void Transform::setPositionXY(const Vector2& p, bool rb) {
    position = p;
    changed(rb);
}
void Transform::setRotation(const Vector3& r, bool rb) {
    rotation = r;
    changed(rb);
}
void Transform::setScale(const Vector3& s) {
    scale = s;
    changed();
}
void Transform::setScaleXY(const Vector2& s) {
    scale = s;
    changed();
}

void Transform::translate(const Vector3& p) {
    position += p;
    changed();
}
void Transform::rotate(const Vector3& r) {
    rotation += r;
    changed();
}

Vector3 Transform::right() {

    Vector3 r(1, 0, 0);   //X

    // Crear cuaterniones para las rotaciones en cada eje
    Quaternion q_x(rotation.x, Vector3(1, 0, 0));   // Rotación en el eje X
    Quaternion q_y(rotation.y, Vector3(0, 1, 0));   // Rotación en el eje Y
    Quaternion q_z(rotation.z, Vector3(0, 0, 1));   // Rotación en el eje Z

    // Multiplicar los cuaterniones en orden (Z * Y * X)
    Quaternion combinedRotation = q_z * q_y * q_x;


    // Rotar el vector forward utilizando el cuaternion combinado
    Vector3 v = combinedRotation.rotatePoint(r);

    v.normalize();

    return v;
}
Vector3 Transform::up() {

    Vector3 u(0, -1, 0);   //Y

    // Crear cuaterniones para las rotaciones en cada eje
    Quaternion q_x(rotation.x, Vector3(1, 0, 0));   // Rotación en el eje X
    Quaternion q_y(rotation.y, Vector3(0, 1, 0));   // Rotación en el eje Y
    Quaternion q_z(rotation.z, Vector3(0, 0, 1));   // Rotación en el eje Z

    // Multiplicar los cuaterniones en orden (Z * Y * X)
    Quaternion combinedRotation = q_z * q_y * q_x;


    // Rotar el vector forward utilizando el cuaternion combinado
    Vector3 v = combinedRotation.rotatePoint(u);

    v.normalize();

    return v;
}
Vector3 Transform::forward() {


    //Vector3 f(0, 0, 1);   //Z

    ///*  f.x = cosf(rotation.x) * sin(rotation.y);
    //f.y = -sinf(rotation.x);
    //f.z = cosf(rotation.x) * cos(rotation.y);*/

    //// v.normalize();

    //f.rotateX(rotation.x);
    //f.rotateY(rotation.y);
    //f.rotateZ(rotation.z);

    //return f;
    ///* return v;*/


    //Vector3 u(0, 0, 1);   //Z

    //// Crear cuaterniones para las rotaciones en cada eje
    //Quaternion q_x(rotation.x, Vector3(1, 0, 0));   // Rotación en el eje X
    //Quaternion q_y(rotation.y, Vector3(0, 1, 0));   // Rotación en el eje Y
    //Quaternion q_z(rotation.z, Vector3(0, 0, 1));   // Rotación en el eje Z

    //// Multiplicar los cuaterniones en orden (Z * Y * X)
    //Quaternion combinedRotation = q_z * q_y * q_x;


    //// Rotar el vector forward utilizando el cuaternion combinado
    //Vector3 v = combinedRotation.rotatePoint(u);

    //v.normalize();

    //return v;

    Vector3 r = right();
    Vector3 u = up();
    Vector3 v = u.cross(r);

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