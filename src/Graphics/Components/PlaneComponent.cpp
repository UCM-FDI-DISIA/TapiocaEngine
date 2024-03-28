#include "PlaneComponent.h"
#include "../GraphicsManager.h"
#include "Components/Transform.h"
#include "Structure/GameObject.h"
#include "../RenderNode.h"
#include "../Plane.h"

namespace Tapioca {
PlaneComponent::PlaneComponent()
    : plane(nullptr), node(nullptr), transform(nullptr), initialRotation(Vector3(0.0f)), fConstant(), height(), width(),
      xSegments(), ySegments() { }

PlaneComponent::~PlaneComponent() { delete node; }

bool PlaneComponent::initComponent(const CompMap& variables) {   // No se ha podido establecer o No hay nombre de mesh
    // No se ha podido establecer o No hay nombre de mesh
    if (!setValueFromMap(planeName, "planeName", variables) || planeName == "") {
#ifdef _DEBUG
        std::cerr << "Error: PlaneComponent: no se pudo inicializar el nombre del plano.\n";
#endif
        return false;
    }

    // Da igual si no hay material o si el material tiene un nombre vacio
    if (!setValueFromMap(materialName, "materialName", variables)) {
#ifdef _DEBUG
        std::cout << "No existe nombre para el material: se coloca el predefinido por la mesh.\n";
#endif
    }

    bool normalSet = setValueFromMap(rkNormal.x, "rkNormalX", variables) &&
        setValueFromMap(rkNormal.y, "rkNormalY", variables) && setValueFromMap(rkNormal.z, "rkNormalZ", variables);
    if (!normalSet) {
#ifdef _DEBUG
        std::cout << "No se ha definido una normal para el plano, se usara el valor por defecto (0, 0, 1).\n";
#endif
        rkNormal = Tapioca::Vector3(0.f, 0.f, 1.f);
    }

    if (!setValueFromMap(fConstant, "f", variables)) {
#ifdef _DEBUG
        std::cout << "No se ha definido una constante f, se usara el valor por defecto 0.\n";
#endif
        fConstant = 0.f;
    }

    if (!setValueFromMap(width, "width", variables)) {
#ifdef _DEBUG
        std::cout << "No se ha definido una anchura, se usara el valor por defecto 1.\n";
#endif
        width = 1.f;
    }

    if (!setValueFromMap(height, "height", variables)) {
#ifdef _DEBUG
        std::cout << "No se ha definido una altura, se usara el valor por defecto 1.\n";
#endif
        height = 1.f;
    }

    if (!setValueFromMap(xSegments, "xSegments", variables)) {
#ifdef _DEBUG
        std::cout << "No se ha definido el numero de segmentos en el eje x, se usara el valor por defecto 1.\n";
#endif
        xSegments = 1;
    }

    if (!setValueFromMap(ySegments, "ySegments", variables)) {
#ifdef _DEBUG
        std::cout << "No se ha definido el numero de segmentos en el eje y, se usara el valor por defecto 1.\n";
#endif
        ySegments = 1;
    }

    bool rotationSet = setValueFromMap(initialRotation.x, "initRotationX", variables) &&
        setValueFromMap(initialRotation.y, "initRotationY", variables) &&
        setValueFromMap(initialRotation.z, "initRotationZ", variables);
    if (!rotationSet) {
#ifdef _DEBUG
        std::cerr << "La rotacion inicial es Vector3(0,0,0).\n";
#endif
    }

    return true;
}

void PlaneComponent::start() {
    GameObject* gameobject = getObject();
    transform = gameobject->getComponent<Transform>();
    GraphicsManager* g = GraphicsManager::instance();
    node = g->createNode();
    plane = g->createPlane(node, rkNormal, fConstant, planeName, width, height, xSegments, ySegments, materialName);

    if (materialName != "") plane->setMaterial(materialName);
}

void PlaneComponent::handleEvent(std::string const& id, void* info) {
    if (id == "transformChanged") {
        node->setPosition(transform->getGlobalPosition());
        node->setRotation(initialRotation + transform->getGlobalRotation());
        node->setScale(transform->getGlobalScale());
    }
}

Plane* PlaneComponent::getPlane() const { return plane; }
}