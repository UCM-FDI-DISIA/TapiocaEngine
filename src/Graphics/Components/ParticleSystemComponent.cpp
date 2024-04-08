#include "ParticleSystemComponent.h"

#include "../GraphicsManager.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "../RenderNode.h"
#include "../ParticleSystem.h"

namespace Tapioca {

ParticleSystemComponent::ParticleSystemComponent()
    : node(nullptr), transform(nullptr), pSys(nullptr), pSysName(""), templateName(""), emitting(false) { }

ParticleSystemComponent::~ParticleSystemComponent() { delete node; }

bool ParticleSystemComponent::initComponent(const CompMap& variables) {
    // Da igual si no hay nombre para el sistema de particulas o si tiene un nombre vacio
    if (!setValueFromMap(pSysName, "pSysName", variables) || pSysName == "") {
        logInfo("ParticleSystem: No existe nombre para el sistema de particulas: se creara sin nombre.");
    }
    if (!setValueFromMap(templateName, "templateName", variables)) {
        logInfo("ParticleSystem: No existe nombre para el template: se creara uno por defecto.");
    }
    if (!setValueFromMap(emitting, "emitting", variables)) {
        logInfo("ParticleSystem: \"emitting\" no esta establecido: se pondra a false por defecto.");
    }

    return true;
}

void ParticleSystemComponent::awake() {
    GameObject* gameobject = getObject();
    transform = gameobject->getComponent<Transform>();
    node = GraphicsManager::instance()->createNode();
    pSys = GraphicsManager::instance()->createParticleSystem(node, pSysName, templateName, emitting);
}

void ParticleSystemComponent::handleEvent(std::string const& id, void* info) {
    if (id == "transformChanged") {
        node->setPosition(transform->getGlobalPosition());
        node->setScale(transform->getGlobalScale());
    }
}

void ParticleSystemComponent::setEmitting(const bool emitting) { pSys->setEmitting(emitting); }

bool ParticleSystemComponent::isEmitting() { return pSys->isEmitting(); }

void ParticleSystemComponent::setQuota(const int q) { pSys->setQuota(q); }

int ParticleSystemComponent::getQuota() { return pSys->getQuota(); }

void ParticleSystemComponent::setVisible(const bool v) { pSys->setVisible(v); }

bool ParticleSystemComponent::isVisible() const { return pSys->isVisible(); }

void ParticleSystemComponent::fastForward(const float time, const float interval) { pSys->fastForward(time, interval); }

}