#include "PlaneComponent.h"
#include "GraphicsManager.h"
#include "Components/Transform.h"
#include "Structure/GameObject.h"
#include "Utilities/RenderNode.h"
#include "Utilities/Plane.h"
#include "checkML.h"

namespace Tapioca {
PlaneComponent::PlaneComponent()
    : node(nullptr), transform(nullptr), plane(nullptr), rkNormal(Vector3(0.0f, 0.0f, 1.0f)),
      up(Vector3(0.0f, 1.0f, 0.0f)), width(1.0f), height(1.0f), xSegments(1), ySegments(1), planeName(""),
      materialName("") { }

PlaneComponent::~PlaneComponent() {
    if (node != nullptr) delete node;
    node = nullptr;
    transform = nullptr;
    plane = nullptr;
}

bool PlaneComponent::initComponent(const CompMap& variables) {
    if (!setValueFromMap(materialName, "materialName", variables)) {
        logInfo("PlaneComponent: No existe nombre para el material: se coloca el predefinido por la mesh.");
    }

    bool normalSet = setValueFromMap(rkNormal.x, "rkNormalX", variables) &&
        setValueFromMap(rkNormal.y, "rkNormalY", variables) && setValueFromMap(rkNormal.z, "rkNormalZ", variables);
    if (!normalSet) {
        logInfo("PlaneComponent: No se ha definido una normal para el plano, se usara el valor por defecto (0, 0, 1).");
    }

    bool upSet = setValueFromMap(up.x, "upX", variables) && setValueFromMap(up.y, "upY", variables) &&
        setValueFromMap(up.z, "upZ", variables);
    if (!upSet) {
        logInfo("PlaneComponent: No se ha definido un up para el plano, se usara el valor por defecto (0, 1, 0).");
    }

    if (!setValueFromMap(width, "width", variables)) {
        logInfo("PlaneComponent: No se ha definido una anchura, se usara el valor por defecto 1.");
    }

    if (!setValueFromMap(height, "height", variables)) {
        logInfo("PlaneComponent: No se ha definido una altura, se usara el valor por defecto 1.");
    }

    if (!setValueFromMap(xSegments, "xSegments", variables)) {
        logInfo(
            "PlaneComponent: No se ha definido el numero de segmentos en el eje x, se usara el valor por defecto 1.");
    }

    if (!setValueFromMap(ySegments, "ySegments", variables)) {
        logInfo(
            "PlaneComponent: No se ha definido el numero de segmentos en el eje y, se usara el valor por defecto 1.");
    }

    return true;
}

void PlaneComponent::awake() {
    GameObject* gameobject = getObject();
    transform = gameobject->getComponent<Transform>();
    GraphicsManager* g = GraphicsManager::instance();
    node = g->createNode();
    plane = g->createPlaneWithName(node, rkNormal, 0, up, width, height, xSegments, ySegments, materialName);

    if (materialName != "") plane->setMaterial(materialName);
}

void PlaneComponent::handleEvent(std::string const& id, void* info) {
    if (id == "posChanged") node->setPosition(transform->getGlobalPositionWithoutRotation());
}

void PlaneComponent::setVisible(const bool enable) { plane->setVisible(enable); }

bool PlaneComponent::isVisible() const { return plane->isVisible(); }
}
