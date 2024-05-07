#include "ParticleSystemComponent.h"
#include "GraphicsManager.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "Utilities/RenderNode.h"
#include "Utilities/ParticleSystem.h"
#include "checkML.h"

namespace Tapioca {

ParticleSystemComponent::ParticleSystemComponent()
    : node(nullptr), transform(nullptr), pSys(nullptr), pSysName(""), templateName(""), emitting(false) { }

ParticleSystemComponent::~ParticleSystemComponent() {
    if (node != nullptr) delete node;
    node = nullptr;
    transform = nullptr;
    pSys = nullptr;
}

bool ParticleSystemComponent::initComponent(const CompMap& variables) {
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
    if (id == "posChanged") node->setPosition(transform->getGlobalPosition());
}

void ParticleSystemComponent::setEmitting(const bool emitting) { pSys->setEmitting(emitting); }

bool ParticleSystemComponent::isEmitting() { return pSys->isEmitting(); }

void ParticleSystemComponent::setQuota(const size_t q) { pSys->setQuota(q); }

size_t ParticleSystemComponent::getQuota() { return pSys->getQuota(); }

void ParticleSystemComponent::setVisible(const bool v) { pSys->setVisible(v); }

bool ParticleSystemComponent::isVisible() const { return pSys->isVisible(); }

void ParticleSystemComponent::fastForward(const float time, const float interval) { pSys->fastForward(time, interval); }
}
