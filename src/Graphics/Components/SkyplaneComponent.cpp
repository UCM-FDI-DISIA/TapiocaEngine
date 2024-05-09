#include "SkyplaneComponent.h"
#include "GraphicsManager.h"
#include "Utilities/RenderNode.h"
#include "Components/Transform.h"
#include "Structure/GameObject.h"
#include "Utilities/Skyplane.h"
#include "checkML.h"

namespace Tapioca {

SkyplaneComponent::SkyplaneComponent() : node(nullptr), transform(nullptr), skyplane(nullptr), 
    materialName(""), rkNormal(Vector3(0.0f, 0.0f, 1.0f)), fConstant(0.0f), scale(1000.0f), 
    tiling(10.0f), drawFirst(true), bow(0.0f), xSegments(1), ySegments(1) { }

SkyplaneComponent::~SkyplaneComponent() {
    if (node != nullptr) delete node;
    node = nullptr;
    transform = nullptr;
    skyplane = nullptr;
}

bool SkyplaneComponent::initComponent(const CompMap& variables) {
    // Da igual si no hay material o si el material tiene un nombre vacio
    if (!setValueFromMap(materialName, "materialName", variables)) {
        logInfo("SkyplaneComponent: No existe nombre para el material: se coloca el predefinido por la mesh.");
    }

    bool normalSet = setValueFromMap(rkNormal.x, "rkNormalX", variables) &&
                     setValueFromMap(rkNormal.y, "rkNormalY", variables) && 
                     setValueFromMap(rkNormal.z, "rkNormalZ", variables);
    if (!normalSet) {
        logInfo("SkyplaneComponent: No se ha definido una normal para el plano, se usara el valor por defecto (0, 0, 1).");
    }

    if (!setValueFromMap(fConstant, "f", variables)) {
        logInfo("SkyplaneComponent: No se ha definido una constante f, se usara el valor por defecto 0.");
    }

    bool scale = setValueFromMap(scale, "scale", variables);
    if (!scale) {
        logInfo("SkyplaneComponent: No se ha definido un scale para skyplane, se usara el valor por defecto 1000.");
    }

    if (!setValueFromMap(tiling, "tiling", variables)) {
        logInfo("SkyplaneComponent: No se ha definido un tiling, se usara el valor por defecto 10.");
    }

    if (!setValueFromMap(drawFirst, "drawFirst", variables)) {
        logInfo("SkyplaneComponent: No se ha definido un valor para drawFirst, se usara el valor por defecto true.");
    }

    if (!setValueFromMap(bow, "bow", variables)) {
        logInfo("SkyplaneComponent: No se ha definido un valor para bow, se usara el valor por defecto 0.");
    }

    if (!setValueFromMap(xSegments, "xSegments", variables)) {
        logInfo("SkyplaneComponent: No se ha definido el numero de segmentos en el eje x, se usara el valor por "
                "defecto 1.");
    }

    if (!setValueFromMap(ySegments, "ySegments", variables)) {
        logInfo("SkyplaneComponent: No se ha definido el numero de segmentos en el eje y, se usara el valor por "
                "defecto 1.");
    }

    return true;
}

void SkyplaneComponent::awake() {
    transform = object->getComponent<Transform>();
    GraphicsManager* g = GraphicsManager::instance();
    node = g->createNode();
}
}
