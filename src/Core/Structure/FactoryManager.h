#pragma once
#include <Windows.h>
#include "Utilities/Singleton.h"
#include "Structure/Module.h"

namespace Tapioca {
class Component;
class ComponentBuilder;

/**
* @brief Clase Singleton y Modulo que se encarga de gestionar las factorias de componentes
*/
class TAPIOCA_API FactoryManager : public Singleton<FactoryManager>, public Module {
private:
    friend Singleton<FactoryManager>;

    std::unordered_map<std::string, ComponentBuilder*> builders;   // Mapa de factorias de componentes
    HMODULE module; // Modulo cargado en la memoria del proceso

    /**
    * @brief Inicializa el modulo
    */
    FactoryManager();
    FactoryManager(const HMODULE module);

public:
    FactoryManager(FactoryManager&) = delete;
    FactoryManager(FactoryManager&&) = delete;
    FactoryManager& operator=(FactoryManager&) = delete;
    FactoryManager& operator=(FactoryManager&&) = delete;

    /**
    * @brief Libera la memoria usada por las factorias
    */
    ~FactoryManager();

    /**
    * @brief Crea un componente a partir de su nombre
    */
    Component* createComponent(std::string const& name);

    /**
    * @brief Añade una factoria al mapa de factorias
    */
    void addFactory(std::string const& name, ComponentBuilder* const builder);
};
}