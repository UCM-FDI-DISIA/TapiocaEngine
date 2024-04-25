#include "Transform.h"
#include "Structure/GameObject.h"

namespace Tapioca {
Transform::Transform()
    : Component(), position(Vector3(0)), rotation(Vector3(0.0f)), scale(Vector3(1.0f)), parent(nullptr) { }

Transform::~Transform() {
    for (Transform* child : getAllChildren()) {
        GameObject* childGameObject = child->getObject();
        childGameObject->die();
    }

    removeConnections();
    if (object != nullptr) object->die();
}

bool Transform::initComponent(const CompMap& variables) {
    if (!setValueFromMap(position.x, "positionX", variables)) {
        logInfo(("Transform: No se encontro el valor de positionX. Se inicializo al valor predefinido: \"" +
                 std::to_string(position.x) + "\".")
                    .c_str());
    }
    if (!setValueFromMap(position.y, "positionY", variables)) {
        logInfo(("Transform: No se encontro el valor de positionY. Se inicializo al valor predefinido: \"" +
                 std::to_string(position.y) + "\".")
                    .c_str());
    }
    if (!setValueFromMap(position.z, "positionZ", variables)) {
        logInfo(("Transform: No se encontro el valor de positionZ. Se inicializo al valor predefinido: \"" +
                 std::to_string(position.z) + "\".")
                    .c_str());
    }

    Vector3 rotationVec;
    if (!setValueFromMap(rotationVec.x, "rotationX", variables)) {
        logInfo(("Transform: No se encontro el valor de rotationX. Se inicializo al valor predefinido: \"" +
                 std::to_string(rotationVec.x) + "\".")
                    .c_str());
    }
    if (!setValueFromMap(rotationVec.y, "rotationY", variables)) {
        logInfo(("Transform: No se encontro el valor de rotationY. Se inicializo al valor predefinido: \"" +
                 std::to_string(rotationVec.y) + "\".")
                    .c_str());
    }
    if (!setValueFromMap(rotationVec.z, "rotationZ", variables)) {
        logInfo(("Transform: No se encontro el valor de rotationZ. Se inicializo al valor predefinido: \"" +
                 std::to_string(rotationVec.z) + "\".")
                    .c_str());
    }
    rotation = Quaternion(rotationVec);

    if (!setValueFromMap(scale.x, "scaleX", variables)) {
        logInfo(("Transform: No se encontro el valor de scaleX. Se inicializo al valor predefinido: \"" +
                 std::to_string(scale.x) + "\".")
                    .c_str());
    }
    if (!setValueFromMap(scale.y, "scaleY", variables)) {
        logInfo(("Transform: No se encontro el valor de scaleY. Se inicializo al valor predefinido: \"" +
                 std::to_string(scale.y) + "\".")
                    .c_str());
    }
    if (!setValueFromMap(scale.z, "scaleZ", variables)) {
        logInfo(("Transform: No se encontro el valor de scaleZ. Se inicializo al valor predefinido: \"" +
                 std::to_string(scale.z) + "\".")
                    .c_str());
    }
    return true;
}

void Transform::start() {
    posChanged();
    rotChanged();
    scaleChanged();
}

void Transform::handleEvent(std::string const& id, void* info) {
    if (id == "posChanged") {
        for (Transform* child : children)
            child->posChanged();
    }
    else if (id == "rotChanged") {
        for (Transform* child : children)
            child->rotChanged();
    }
    else if (id == "scaleChanged") {
        for (Transform* child : children)
            child->scaleChanged();
    }
}

void Transform::posChanged(bool rb) { pushEvent("posChanged", &rb, false); }

void Transform::rotChanged(bool rb) { pushEvent("rotChanged", &rb, false); }

void Transform::scaleChanged() { pushEvent("scaleChanged", nullptr, false); }

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

    // Elimina todos los hijos directos
    for (auto it = children.begin(), itAnt = children.begin(); it != children.end();) {
        ++itAnt;
        (*it)->removeParent();
        it = itAnt;
    }
    children.clear();
}

void Transform::removeParent() {
    if (parent != nullptr) parent->removeChild(this);
    parent = nullptr;
}

Vector3 Transform::localRight() {
    Vector3 r(-1, 0, 0);   // X
    Vector3 v = rotation.rotatePoint(r);
    v.normalize();
    return v;
}

Vector3 Transform::localUp() {
    Vector3 u(0, 1, 0);   // Y
    Vector3 v = rotation.rotatePoint(u);
    v.normalize();
    return v;
}

Vector3 Transform::localForward() {
    Vector3 r = localRight();
    Vector3 u = localUp();
    Vector3 v = u.cross(r);
    v.normalize();
    return v;
}

Vector3 Transform::getGlobalPositionWithoutRotationAux(Vector3 point) const {
    if (parent == nullptr) return point;

    Vector3 parentScale = parent->getScale();
    point = Vector3(point.x * parentScale.x, point.y * parentScale.y, point.z * parentScale.z);

    // Se convierte al sistema de coordenadas del padre
    point = point + parent->position;

    return parent->getGlobalPositionWithoutRotationAux(point);
}

Vector3 Transform::getGlobalPositionAux(Vector3 point) const {
    if (parent == nullptr) return point;

    Vector3 xAxis = -parent->localRight();
    Vector3 yAxis = parent->localUp();
    Vector3 zAxis = parent->localForward();

    Vector3 parentScale = parent->getScale();
    point = Vector3(point.x * parentScale.x, point.y * parentScale.y, point.z * parentScale.z);

    Vector3 pos = xAxis * point.x + yAxis * point.y + zAxis * point.z;
   /* pos.x = point.x * xAxis.x + point.y * yAxis.x + point.z * zAxis.x;
    pos.y = point.x * xAxis.y + point.y * yAxis.y + point.z * zAxis.y;
    pos.z = point.x * xAxis.z + point.y * yAxis.z + point.z * zAxis.z;*/

    // Se convierte al sistema de coordenadas del padre
    pos = pos + parent->position;

    return parent->getGlobalPositionAux(pos);
}

Vector3 Transform::getLocalPosition(Vector3 point) const {
    if (parent == nullptr) return point-position;
    point = parent->getLocalPosition(point);
    
    Vector3 xAxis = -parent->localRight();
    Vector3 yAxis = parent->localUp();
    Vector3 zAxis = parent->localForward();

    Vector3 localPos;
    localPos.x = point.dot(xAxis);
    localPos.y = point.dot(yAxis);
    localPos.z = point.dot(zAxis);

    Vector3 parentScale = parent->getScale();
    //localPos = localPos/parentScale;
    localPos = Vector3(localPos.x / parentScale.x, localPos.y / parentScale.y, localPos.z / parentScale.z);
    return localPos;
}

Vector3 Transform::getGlobalPositionWithoutRotation() const {
    return getGlobalPositionWithoutRotationAux(position);

    /*
    Vector3 aux = position;
    if (parent != nullptr) {
        Vector3 parentScale = parent->getScale();
        aux = Vector3(aux.x * parentScale.x, aux.y * parentScale.y, aux.z * parentScale.z);
        aux = aux + parent->getGlobalPositionWithoutRotation();
    }
    return aux;
    */
}

Vector3 Transform::getGlobalPosition() const { return getGlobalPositionAux(position); }

Quaternion Transform::getGlobalRotation() const {
    /*
    Vector3 aux = rotation;
    if (parent != nullptr) {
        aux = parent->getGlobalRotation() + aux;
    }
    return aux;
    */

    Quaternion aux = rotation;
    if (parent != nullptr) aux = Quaternion(parent->getGlobalRotation()) * aux;
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

void Transform::setPosition(const Vector3& p, bool rb) {
    position = p;
    posChanged(rb);
}

void Transform::setGlobalPosition(const Vector3& p, bool rb) {
    position = getLocalPosition(p);
    posChanged(rb);
}

void Transform::setPositionXY(const Vector2& p, bool rb) {
    position = p;
    posChanged(rb);
}

void Transform::setRotation(const Vector3& r, bool rb) {
    rotation = Quaternion(r);
    posChanged(rb);
    rotChanged(rb);
}

void Transform::setRotation(const Quaternion& q, bool rb) {
    rotation = q;
    posChanged(rb);
    rotChanged(rb);
}

void Transform::setScale(const Vector3& s) {
    scale = s;
    posChanged();
    scaleChanged();
}

void Transform::setScaleXY(const Vector2& s) {
    scale = s;
    posChanged();
    scaleChanged();
}

void Transform::translate(const Vector3& p) {
    position += p;
    posChanged();
}
void Transform::rotate(const Vector3& r) {
    Quaternion q = Quaternion(r);
    rotation = rotation * q;
    posChanged();
    rotChanged();
}

Vector3 Transform::right() {

    Vector3 r(-1, 0, 0);   // X

    // Crear cuaterniones para las rotaciones en cada eje
    //Quaternion q_x(rotation.x, Vector3(1, 0, 0));   // Rotación en el eje X
    //Quaternion q_y(rotation.y, Vector3(0, 1, 0));   // Rotación en el eje Y
    //Quaternion q_z(rotation.z, Vector3(0, 0, 1));   // Rotación en el eje Z

    //// Multiplicar los cuaterniones en orden (Z * Y * X)
    //Quaternion combinedRotation = q_z * q_y * q_x;

    // Rotar el vector forward utilizando el cuaternion combinado
    Vector3 v = getGlobalRotation().rotatePoint(r);

    v.normalize();

    return v;
}

Vector3 Transform::up() {

    Vector3 u(0, 1, 0);   // Y

    // Crear cuaterniones para las rotaciones en cada eje
    //Quaternion q_x(rotation.x, Vector3(1, 0, 0));   // Rotación en el eje X
    //Quaternion q_y(rotation.y, Vector3(0, 1, 0));   // Rotación en el eje Y
    //Quaternion q_z(rotation.z, Vector3(0, 0, 1));   // Rotación en el eje Z

    //// Multiplicar los cuaterniones en orden (Z * Y * X)
    //Quaternion combinedRotation = q_z * q_y * q_x;


    // Rotar el vector forward utilizando el cuaternion combinado
    Vector3 v = getGlobalRotation().rotatePoint(u);

    v.normalize();

    return v;
}

Vector3 Transform::forward() {
    //Vector3 f(0, 0, 1);   // Z

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