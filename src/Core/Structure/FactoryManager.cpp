#include "FactoryManager.h"

#include "Component.h"
#include "ComponentBuilder.h"
#include "Structure/BasicBuilder.h"

// Core
#include "Components/Transform.h"
// Physics
#include "Collider.h"
#include "RigidBody.h"
// Graphics
#include "MeshRenderer.h"
#include "CameraComponent.h"
#include "Animator.h"
#include "ParticleSystemComponent.h"
// UI
#include "Button.h"
#include "InputText.h"
#include "Text.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<FactoryManager>;
template<>
FactoryManager* Singleton<FactoryManager>::instance_ = nullptr;

FactoryManager::FactoryManager() { }

FactoryManager::~FactoryManager() {
    for (auto& f : builders)
        delete f.second;
    builders.clear();
}

bool FactoryManager::init() {
    createEngineBuilders();
    return true;
}

void FactoryManager::createEngineBuilders() {
#ifdef _DEBUG
    std::cout << "Anadiendo factorias del motor\n";
#endif
    FactoryManager* manager = FactoryManager::instance();
    // Core
    manager->addFactory("Transform", new BasicBuilder<Transform>());
    // Physiscs
    manager->addFactory("Collider", new BasicBuilder<Collider>());
    manager->addFactory("RigidBody", new BasicBuilder<RigidBody>());
    // Graphics
    manager->addFactory("MeshRenderer", new BasicBuilder<MeshRenderer>());
    manager->addFactory("CameraComponent", new BasicBuilder<CameraComponent>());
    manager->addFactory("Animator", new BasicBuilder<Animator>());
    manager->addFactory("ParticleSystemComponent", new BasicBuilder<ParticleSystemComponent>());
    // UI
    manager->addFactory("Button", new BasicBuilder<Button>());
    manager->addFactory("InputText", new BasicBuilder<InputText>());
    manager->addFactory("Text", new BasicBuilder<Text>());
}

Component* FactoryManager::createComponent(std::string const& name) {
    if (builders.find(name) != builders.end()) return builders[name]->createComponent();
    return nullptr;
}

void FactoryManager::addFactory(std::string const& name, ComponentBuilder* const builder) {
#ifdef _DEBUG
    std::cout << "Anadiendo la factoria " << name << "\n";
#endif
    builders[name] = builder;
}
}