#include "FactoryManager.h"

#include "Component.h"
#include "ComponentBuilder.h"

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

Component* FactoryManager::createComponent(std::string const& name) {
    if (builders.find(name) != builders.end()) return builders[name]->createComponent();
    return nullptr;
}

void FactoryManager::addFactory(ComponentBuilder* const builder) {
#ifdef _DEBUG
    std::cout << "Anadiendo la factoria " << builder->id << "\n";
#endif
    builders[builder->id] = builder;
}
}