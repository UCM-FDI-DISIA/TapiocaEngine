#include "FactoryManager.h"
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "Components/Collider.h"

#include "Utilities/checkML.h"

namespace Tapioca {

FactoryManager::FactoryManager() : module(nullptr) { }

FactoryManager::FactoryManager(HMODULE module) : module(module) { }

FactoryManager::~FactoryManager() {
    for (auto& f : builders) delete f.second;
    builders.clear();
}

Component* FactoryManager::createComponent(std::string name) {
    if (builders.find(name) != builders.end()) return builders[name]->createComponent();
    return nullptr;
}

void FactoryManager::addFactory(std::string name, ComponentBuilder* builder) { builders[name] = builder; }


}