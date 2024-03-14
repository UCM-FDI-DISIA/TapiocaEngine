#include "FactoryManager.h"
#include "SceneManager.h"

#include "Component.h"
#include "ComponentBuilder.h"
#include "Structure/BasicBuilder.h"

#include "Components/Transform.h"
#include "Components/Collider.h"
#include "Components/RigidBody.h"
#include "Components/MeshRenderer.h"
#include "Components/Animator.h"

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
    manager->addFactory("Transform", new BasicBuilder<Transform>());
    manager->addFactory("MeshRenderer", new BasicBuilder<MeshRenderer>());
    manager->addFactory("Collider", new BasicBuilder<Collider>());
    manager->addFactory("RigidBody", new BasicBuilder<RigidBody>());
    manager->addFactory("Animator", new BasicBuilder<Animator>());
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