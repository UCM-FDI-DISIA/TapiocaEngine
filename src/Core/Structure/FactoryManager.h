#pragma once
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
#include "Structure/ComponentFactory.h"
#include "Utilities/defs.h"
using namespace std;

namespace Tapioca {
/**
* @brief Clase Singleton y Módulo que se encarga de gestionar las factorías de componentes
*/
class FactoryManager : public Singleton<FactoryManager>, public Module {
private:
    friend Singleton<FactoryManager>;

    unordered_map<string, ComponentFactory*> factories; // Mapa de factorías de componentes
    HMODULE module; // Módulo cargado en la memoria del proceso

    /**
    * @brief Inicializa el módulo
    */
    FactoryManager();
    FactoryManager(HMODULE module);

public:
    FactoryManager(FactoryManager&) = delete;
    FactoryManager(FactoryManager&&) = delete;
    FactoryManager& operator=(FactoryManager&) = delete;
    FactoryManager& operator=(FactoryManager&&) = delete;

    /**
    * @brief Libera la memoria usada por las factorías
    */
    ~FactoryManager();

    /**
    * @brief Inicializa el módulo
    * @return Devuelve true si se ha inicializado correctamente
    */
    bool init() override;

    /**
    * @brief Crea un componente a partir de su nombre
    */
    Component* createComponent(string name);

    /**
    * @brief Añade una factoría al mapa de factorías
    */
    void addFactory(string name, ComponentFactory* factory);
};
}