#include "CreateBuilders.h"
#include "Structure/FactoryManager.h"
#include "TransformBuilder.h"
#include "Structure/BasicBuilder.h"
#include "Components/MeshRenderer.h"

namespace Tapioca {
void createEngineBuilders() {
	FactoryManager* manager = FactoryManager::instance();
	manager->addFactory("Transform", new TransformBuilder());
	manager->addFactory("MeshRenderer", new BasicBuilder<MeshRenderer>());
}
}
