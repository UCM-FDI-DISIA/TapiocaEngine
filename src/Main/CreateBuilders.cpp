#include "CreateBuilders.h"
#include "Structure/FactoryManager.h"
#include "TransformBuilder.h"

namespace Tapioca {
void createEngineBuilders() {
	FactoryManager* manager = FactoryManager::instance();
	manager->addFactory("transform", new TransformBuilder());
}
}
