#pragma once
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
#include "Structure/ComponentFactory.h"
#include "Utilities/defs.h"
using namespace std;

namespace Tapioca {
/**
* @brief Clase Singleton y M�dulo que se encarga de gestionar las factor�as de componentes
*/
class FactoryManager : public Singleton<FactoryManager>, public Module {
private:
    friend Singleton<FactoryManager>;

    unordered_map<string, ComponentFactory*> factories; // Mapa de factor�as de componentes
    HMODULE module; // M�dulo cargado en la memoria del proceso

    /**
    * @brief Inicializa el m�dulo
    */
    FactoryManager();
    FactoryManager(HMODULE module);

public:
    FactoryManager(FactoryManager&) = delete;
    FactoryManager(FactoryManager&&) = delete;
    FactoryManager& operator=(FactoryManager&) = delete;
    FactoryManager& operator=(FactoryManager&&) = delete;

    /**
    * @brief Libera la memoria usada por las factor�as
    */
    ~FactoryManager();

    /**
    * @brief Inicializa el m�dulo
    * @return Devuelve true si se ha inicializado correctamente
    */
    bool init() override;

    /**
    * @brief Crea un componente a partir de su nombre
    */
    Component* createComponent(string name);

    /**
    * @brief A�ade una factor�a al mapa de factor�as
    */
    void addFactory(string name, ComponentFactory* factory);
};
}