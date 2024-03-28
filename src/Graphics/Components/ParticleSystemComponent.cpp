#include "ParticleSystemComponent.h"

#include "../GraphicsManager.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "../RenderNode.h"
#include "../ParticleSystem.h"

namespace Tapioca {

ParticleSystemComponent::ParticleSystemComponent()
    : node(nullptr), transform(nullptr), pSys(nullptr), pSysName(""), templateName(""), emitting(false), initialRotation(Vector3(0.0f)) { }

ParticleSystemComponent::~ParticleSystemComponent() { delete node; }

bool ParticleSystemComponent::initComponent(const CompMap& variables) { 
    // Da igual si no hay nombre para el sistema de particulas o si tiene un nombre vacio
    if (!setValueFromMap(pSysName, "pSysName", variables) || pSysName == "") {
#ifdef _DEBUG
        std::cout << "ParticleSystem: no existe nombre para el sistema de particulas: se creara sin nombre.\n";
#endif
    }
    if (!setValueFromMap(templateName, "templateName", variables)) {
#ifdef _DEBUG
        std::cout << "ParticleSystem: no existe nombre para el template: se creara uno por defecto.\n";
#endif
    }
    if (!setValueFromMap(emitting, "emitting", variables)) {
#ifdef _DEBUG
        std::cout << "ParticleSystem: emitting no esta establecido: se pondra a false por defecto.\n";
#endif
    }

    return true;
}



void ParticleSystemComponent::awake() {
    GameObject* gameobject = getObject();
    transform = gameobject->getComponent<Transform>();
    GraphicsManager* g = GraphicsManager::instance();
    node = g->createNode();
    pSys = g->createParticleSystem(node, pSysName, templateName, emitting);
}

void ParticleSystemComponent::handleEvent(std::string const& id, void* info) {
    if (id == "transformChanged") {
        node->setPosition(transform->getGlobalPosition());
        node->setRotation(initialRotation + transform->getGlobalRotation());
        node->setScale(transform->getGlobalScale());
    }
}


void ParticleSystemComponent::setEmitting(const bool emitting) { pSys->setEmitting(emitting); }

bool ParticleSystemComponent::isEmitting() { return pSys->isEmitting(); }

void ParticleSystemComponent::fastForward(const float time, const float interval) { pSys->fastForward(time, interval); }


//void ParticleSystemComponent::setSystemName(const std::string& n) { pSysName = n; }

//void ParticleSystemComponent::setTemplateName(const std::string& n) { templateName = n; }

}