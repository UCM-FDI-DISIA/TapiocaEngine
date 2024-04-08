#include "SkyplaneComponent.h"
#include "../RenderNode.h"
#include "../GraphicsManager.h"
#include "Components/Transform.h"
#include "Structure/GameObject.h"
#include "../Skyplane.h"

namespace Tapioca {

SkyplaneComponent::SkyplaneComponent()
    : skyplane(nullptr), node(nullptr), transform(nullptr), materialName(), skyplaneName(), rkNormal(), fConstant(),
      scale(), tiling(), drawFirst(), bow(), xSegments(), ySegments() { }

SkyplaneComponent::~SkyplaneComponent() { delete node; }

bool SkyplaneComponent::initComponent(const CompMap& variables) {

    // No se ha podido establecer o No hay nombre de mesh
    if (!setValueFromMap(skyplaneName, "planeName", variables) || skyplaneName == "") {
        logError("SkyplaneComponent: No se pudo inicializar el nombre del skyplane.");
        return false;
    }

    // Da igual si no hay material o si el material tiene un nombre vacio
    if (!setValueFromMap(materialName, "materialName", variables)) {
        logInfo("SkyplaneComponent: No existe nombre para el material: se coloca el predefinido por la mesh.");
    }

    bool normalSet = setValueFromMap(rkNormal.x, "rkNormalX", variables) &&
        setValueFromMap(rkNormal.y, "rkNormalY", variables) && setValueFromMap(rkNormal.z, "rkNormalZ", variables);
    if (!normalSet) {
        logInfo("SkyplaneComponent: No se ha definido una normal para el plano, se usara el valor por defecto (0, 0, 1).");
        rkNormal = Tapioca::Vector3(0.f, 0.f, 1.f);
    }

    if (!setValueFromMap(fConstant, "f", variables)) {
        logInfo("SkyplaneComponent: No se ha definido una constante f, se usara el valor por defecto 0.");
        fConstant = 0.f;
    }

    bool scale = setValueFromMap(scale, "scale", variables);
    if (!scale) {
        logInfo("SkyplaneComponent: No se ha definido un scale para skyplane, se usara el valor por defecto 1000.");
        scale = 1000.f;
    }

    if (!setValueFromMap(tiling, "tiling", variables)) {
        logInfo("SkyplaneComponent: No se ha definido un tiling, se usara el valor por defecto 10.");
        tiling = 10.f;
    }

    if (!setValueFromMap(drawFirst, "drawFirst", variables)) {
        logInfo("SkyplaneComponent: No se ha definido un valor para drawFirst, se usara el valor por defecto true.");
        drawFirst = true;
    }

    if (!setValueFromMap(bow, "bow", variables)) {
        logInfo("SkyplaneComponent: No se ha definido un valor para bow, se usara el valor por defecto 0.");
        bow = 0.f;
    }

    if (!setValueFromMap(xSegments, "xSegments", variables)) {
        logInfo("SkyplaneComponent: No se ha definido el numero de segmentos en el eje x, se usara el valor por defecto 1.");
        xSegments = 1;
    }

    if (!setValueFromMap(ySegments, "ySegments", variables)) {
        logInfo("SkyplaneComponent: No se ha definido el numero de segmentos en el eje y, se usara el valor por defecto 1.");
        ySegments = 1;
    }

    return true;
}

void SkyplaneComponent::awake() {
    GameObject* gameobject = getObject();
    transform = gameobject->getComponent<Transform>();
    GraphicsManager* g = GraphicsManager::instance();
    node = g->createNode();
    //skyplane = g->createPlane(node, rkNormal, fConstant, up, planeName, width, height, xSegments, ySegments, materialName);

    //if (materialName != "") skyplane->setMaterial(materialName);
}

void SkyplaneComponent::update(uint64_t delt) { }

}
