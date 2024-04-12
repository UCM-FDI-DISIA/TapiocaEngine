#include "SkyboxComponent.h"
#include "GraphicsManager.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "Utilities/RenderNode.h"

namespace Tapioca {

SkyboxComponent::SkyboxComponent()
    : skybox(nullptr), node(nullptr), transform(nullptr), materialName(), skyboxName(), distC(), orderC() { }

SkyboxComponent::~SkyboxComponent() { delete node; }

bool SkyboxComponent::initComponent(const CompMap& variables) { 

    // No se ha podido establecer o No hay nombre de mesh
    if (!setValueFromMap(skyboxName, "planeName", variables) || skyboxName == "") {
        logError("SkyboxComponent: No se pudo inicializar el nombre del skybox.");
        return false;
    }

    // Da igual si no hay material o si el material tiene un nombre vacio
    if (!setValueFromMap(materialName, "materialName", variables)) {
        logInfo("SkyboxComponent: No existe nombre para el material: se coloca el predefinido por la mesh.");
    }

    bool bDistC = setValueFromMap(distC, "distC", variables);
    if (!bDistC) {
        logInfo("SkyboxComponent: No se ha definido una distancia entre el skybox y la c�mara, se usara el valor por defecto 5000s.");
        distC = 5000.f;
    }

    if (!setValueFromMap(orderC, "orderC", variables)) {
        logInfo("SkyboxComponent: No se ha definido si el skybox se pinta antes o despu�s de la escena, se usara el valor por defecto true.");
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
}