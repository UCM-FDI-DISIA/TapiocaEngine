#include "FactoryManager.h"
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "Components/Collider.h"
#include "Utilities/checkML.h"

Tapioca::FactoryManager::FactoryManager()
    : module(nullptr) { }

Tapioca::FactoryManager::FactoryManager(HMODULE module)
    : module(module) { }

Tapioca::FactoryManager::~FactoryManager() {
    for (auto& f : factories) {
        delete f.second;
    }
    factories.clear();
}
bool Tapioca::FactoryManager::init() {
    // TODO: Esto deberia estar en Main
    addFactory("rigidBody", new RigidBodyComponentFactory());
    addFactory("collider", new ColliderComponentFactory());

    EntryPoint eP = (EntryPoint)GetProcAddress(module, "getComponentFactories");

    int numFactories;
    FactoryInfo** fI = eP(numFactories);

    for (int i = 0; i < numFactories; ++i) {
        addFactory(fI[i]->name, fI[i]->factory);
	}
    return true;
}

Tapioca::Component* Tapioca::FactoryManager::createComponent(std::string name) {
    if (factories.find(name) != factories.end()) {
        return factories[name]->createComponent();
    }
    return nullptr;
}

void Tapioca::FactoryManager::addFactory(std::string name, ComponentFactory* factory) { factories[name] = factory; }
