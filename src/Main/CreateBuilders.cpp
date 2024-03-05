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
    FactoryManager* manager = FactoryManager::instance();
    manager->addFactory("Transform", new TransformBuilder());
    manager->addFactory("MeshRenderer", new BasicBuilder<MeshRenderer>());
    manager->addFactory("Collider", new ColliderBuilder());
    manager->addFactory("RigidBody", new BasicBuilder<RigidBody>());
}

void createGameBuilders(HMODULE module) {
    FactoryManager* manager = FactoryManager::instance();

    EntryPoint eP = (EntryPoint)GetProcAddress(module, "getComponentFactories");

    int numFactories;
    FactoryInfo** fI = eP(numFactories);
    for (int i = 0; i < numFactories; ++i) {
        manager->addFactory(fI[i]->name, fI[i]->builder);
        delete fI[i];
    }
    delete[] fI;
}
}
