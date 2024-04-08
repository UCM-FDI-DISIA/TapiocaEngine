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
#ifdef _DEBUG
        std::cerr << "Error: SkyplaneComponent: no se pudo inicializar el nombre del skyplane.\n";
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

    bool scale = setValueFromMap(scale, "scale", variables);
    if (!scale) {
#ifdef _DEBUG
        std::cout << "No se ha definido un scale para skyplane, se usara el valor por defecto 1000.\n";
#endif
        scale = 1000.f;
    }

    if (!setValueFromMap(tiling, "tiling", variables)) {
#ifdef _DEBUG
        std::cout << "No se ha definido un tiling, se usara el valor por defecto 10.\n";
#endif
        tiling = 10.f;
    }

    if (!setValueFromMap(drawFirst, "drawFirst", variables)) {
#ifdef _DEBUG
        std::cout << "No se ha definido un valor para drawFirst, se usara el valor por defecto true.\n";
#endif
        drawFirst = true;
    }

    if (!setValueFromMap(bow, "bow", variables)) {
#ifdef _DEBUG
        std::cout << "No se ha definido un valor para bow, se usara el valor por defecto 0.\n";
#endif
        bow = 0.f;
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
