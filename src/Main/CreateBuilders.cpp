#include "CreateBuilders.h"
#include "Structure/FactoryManager.h"
#include "TransformBuilder.h"
#include "ColliderBuilder.h"
//#include "RigidBodyBuilder.h"
#include "Structure/BasicBuilder.h"
#include "Components/MeshRenderer.h"
#include "Components/RigidBody.h"

namespace Tapioca {
void createEngineBuilders() {
#ifdef _DEBUG
    std::cout << "Anadiendo factorias del motor\n";
#endif
    FactoryManager* manager = FactoryManager::instance();
    manager->addFactory("Transform", new TransformBuilder());
    manager->addFactory("MeshRenderer", new BasicBuilder<MeshRenderer>());
    manager->addFactory("Collider", new ColliderBuilder());
    manager->addFactory("RigidBody", new BasicBuilder<RigidBody>());
}

void createGameBuilders(const HMODULE module) {
    EntryPoint eP = (EntryPoint)GetProcAddress(module, "init");
    eP(FactoryManager::instance());
}
}
