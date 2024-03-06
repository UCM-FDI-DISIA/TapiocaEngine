#pragma once
#include <unordered_map>
#include <Windows.h>
#include "Utilities/Singleton.h"
#include "Structure/Module.h"

namespace Tapioca {
class Component;
class ComponentBuilder;
class DynamicLibraryLoader;

/**
* @brief Clase Singleton y Modulo que se encarga de gestionar las factorias de componentes
*/
class TAPIOCA_API FactoryManager : public Singleton<FactoryManager>, public Module {
private:
    friend Singleton<FactoryManager>;

    std::unordered_map<std::string, ComponentBuilder*> builders;   // Mapa de factorias de componentes
    DynamicLibraryLoader* loader;                           // Cargador de bibliotecas dinamicas

    /**
    * @brief Inicializa el modulo
    */
    FactoryManager();

    /**
    * @brief Crea las factorias de los componentes del motor
    */
    bool init() override;

    /**
    * @brief Crea las factorias de los componentes del juego
    */
    void createEngineBuilders();

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
    * @brief Carga la dll del juego y llama a su funcion de inicializacion
    * @return Si se ha podido cargar correctamente o no
    */
    bool initGame();

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