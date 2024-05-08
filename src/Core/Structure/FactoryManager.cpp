#include "FactoryManager.h"
#include "Component.h"
#include "ComponentBuilder.h"
#include "checkML.h"

namespace Tapioca {
FactoryManager::~FactoryManager() {
    for (auto& f : builders) delete f.second;
    builders.clear();
}

Component* FactoryManager::createComponent(std::string const& name) {
    if (builders.find(name) != builders.end()) return builders[name]->createComponent();
    return nullptr;
}

void FactoryManager::addBuilder(ComponentBuilder* const builder) {
    logInfo(("FactoryManager: Anadiendo el constructor \"" + std::string(builder->id) + "\".").c_str());
    builders[builder->id] = builder;
}
}