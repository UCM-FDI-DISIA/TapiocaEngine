#pragma once
#include <unordered_map>
#include <string>
#include "Utilities/Singleton.h"
#include "Structure/Module.h"

namespace Tapioca {
class Component;
class ComponentBuilder;

/*
* @brief Clase Singleton y Modulo que se encarga de gestionar las factorias de componentes
*/
class TAPIOCA_API FactoryManager : public Singleton<FactoryManager>, public Module {
private:
    friend Singleton<FactoryManager>;

    // warning C4251 'Tapioca::FactoryManager::builders' :
    // class 'std::unordered_map<std::string,Tapioca::ComponentBuilder *,std::hash<std::string>,
    // std::equal_to<std::string>,std::allocator<std::pair<const std::string,Tapioca::ComponentBuilder *>>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::FactoryManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::unordered_map<std::string, ComponentBuilder*> builders;   // Mapa de factorias de componentes
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    /*
    * @brief Inicializa el modulo
    */
    FactoryManager();

    /*
    * @brief Crea las factorias de los componentes del motor
    */
    bool init() override;

    /*
    * @brief Crea las factorias de los componentes del juego
    */
    void createEngineBuilders();

public:
    FactoryManager(FactoryManager&) = delete;
    FactoryManager(FactoryManager&&) = delete;
    FactoryManager& operator=(FactoryManager&) = delete;
    FactoryManager& operator=(FactoryManager&&) = delete;

    /*
    * @brief Libera la memoria usada por las factorias
    */
    ~FactoryManager();

    /**
    * @brief Crea un componente a partir de su nombre
    */
    Component* createComponent(std::string const& name);

    /*
    * @brief Añade una factoria al mapa de factorias
    */
    void addFactory(std::string const& name, ComponentBuilder* const builder);
};
}