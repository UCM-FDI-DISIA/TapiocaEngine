#include "SkyboxComponent.h"
#include "../GraphicsManager.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"

namespace Tapioca {

SkyboxComponent::SkyboxComponent()
    : skybox(nullptr), node(nullptr), transform(nullptr), materialName(), skyboxName(), distC(), orderC() { }

SkyboxComponent::~SkyboxComponent() { delete node; }

bool SkyboxComponent::initComponent(const CompMap& variables) { 

    //No se ha podido establecer o No hay nombre de mesh
    if (!setValueFromMap(skyboxName, "planeName", variables) || skyboxName == "") {
#ifdef _DEBUG
        std::cerr << "Error: SkyboxComponent: no se pudo inicializar el nombre del skybox.\n";
#endif
        return false;
    }

    // Da igual si no hay material o si el material tiene un nombre vacio
    if (!setValueFromMap(materialName, "materialName", variables)) {
#ifdef _DEBUG
        std::cout << "No existe nombre para el material: se coloca el predefinido por la mesh.\n";
#endif
    }

    bool bDistC = setValueFromMap(distC, "distC", variables);
    if (!bDistC) {
#ifdef _DEBUG
        std::cout << "No se ha definido una distancia entre el skybox y la cámara, se usara el valor por defecto 5000s.\n";
#endif
        distC = 5000.f;
    }

    if (!setValueFromMap(orderC, "orderC", variables)) {
#ifdef _DEBUG
        std::cout << "No se ha definido si el skybox se pinta antes o después de la escena, se usara el valor por defecto true.\n";
#endif
        orderC = true;
    }

    return true;
}

void SkyboxComponent::awake() { 
    GameObject* gameobject = getObject();
    transform = gameobject->getComponent<Transform>();
    GraphicsManager* g = GraphicsManager::instance();
    node = g->createNode();
    skybox = g->createSkybox(node, materialName, skyboxName, distC, orderC);

    //if (materialName != "") skybox->setMaterial(materialName);
}

void SkyboxComponent::update(uint64_t delt) { }

}