#pragma once
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
#include "Structure/ComponentFactory.h"
#include "Utilities/defs.h"

namespace Tapioca {
class FactoryManager : public Singleton<FactoryManager>, public Module {
private:
    friend Singleton<FactoryManager>;

    std::unordered_map<std::string, ComponentFactory*> factories;

    //TODO: llamar metodo de juego para registrar las factorias de componentes del juego
    HMODULE module;
    FactoryManager();
    FactoryManager(HMODULE module);

public:
    FactoryManager(FactoryManager&) = delete;
    FactoryManager(FactoryManager&&) = delete;
    FactoryManager& operator=(FactoryManager&) = delete;
    FactoryManager& operator=(FactoryManager&&) = delete;

    ~FactoryManager();

    bool init() override;

    Component* createComponent(std::string name);

    void addFactory(std::string name, ComponentFactory* factory);
};
}