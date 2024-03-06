#include "FactoryManager.h"
#include "DynamicLibraryLoader.h"

#include "Component.h"
#include "ComponentBuilder.h"
#include "Structure/BasicBuilder.h"

#include "Components/Transform.h"
#include "Components/TransformBuilder.h"
#include "Components/Collider.h"
#include "Components/ColliderBuilder.h"
#include "Components/RigidBody.h"
#include "Components/RigidBodyBuilder.h"
#include "Components/MeshRenderer.h"

namespace Tapioca {

FactoryManager::FactoryManager() { loader = new DynamicLibraryLoader(); }

FactoryManager::~FactoryManager() {
    for (auto& f : builders)
        delete f.second;
    builders.clear();

    delete loader;
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
    manager->addFactory("Transform", new TransformBuilder());
    manager->addFactory("MeshRenderer", new BasicBuilder<MeshRenderer>());
    manager->addFactory("Collider", new ColliderBuilder());
    manager->addFactory("RigidBody", new BasicBuilder<RigidBody>());
}

bool FactoryManager::initGame() {
    if (loader->load()) {
        EntryPoint eP = (EntryPoint)GetProcAddress(loader->getModule(), "init");
		eP(FactoryManager::instance());
	}
    else {
#ifdef _DEBUG
        std::cerr << "Error al cargar la DLL del juego\n";
#endif
        return false;
    }
    return true;
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