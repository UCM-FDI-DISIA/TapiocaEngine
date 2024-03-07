#pragma once
#include <string>
#include <vector>
#include "Utilities/Singleton.h"
#include "Structure/Module.h"

struct lua_State;

namespace Tapioca {
class Scene;
class Game;
class Component;
class GameObject;
class Transform;
class FactoryManager;

/**
* @brief Clase Singleton y Modulo que se encarga de cargar las escenas del juego incluyendo los gameobjects y sus componentes
*/
class TAPIOCA_API SceneManager : public Singleton<SceneManager>, public Module {
private:
    friend Singleton<SceneManager>;
    friend class Scene;
    lua_State* luaState;   // Estado de lua

    /*
    * @brief Constructor: Inicializa el modulo, el punto de entrada y el estado de lua
    */
    SceneManager();

    /**
    * @brief Inicializa las escenas, gameobjects y componentes
    */
    void start() override {};

    /**
    * @brief Carga una escena
    * @return Devuelve la escena cargada
    */
    bool loadScene(Scene* const scene, FactoryManager* factMngr);

    /**
    * @brief Carga los gameobjects de una escena
    * @param scene Escena a la que se le van a cargar los gameobjects
    * @return Devuelve true si se ha cargado correctamente
    */
    bool loadGameObjects(Scene* const scene, FactoryManager* factMngr);

    /**
    * @brief Carga los gameobjects de una escena con un padre
    * @return Devuelve true si se ha cargado correctamente
    */
    bool loadGameObjects(Scene* const scene, std::vector<GameObject*>& gameObjectList, FactoryManager* factMngr);

    /**
    * @brief Carga un gameobject
    * @param scene Escena a la que pertenece el gameobject
    * @return Devuelve el gameobject cargado
	*/
    bool loadGameObject(GameObject* const gameObject, FactoryManager* factMngr);

    /**
    * @brief Carga los componentes de un gameobject
    * @param gameObject Gameobject al que se le van a cargar los componentes
    * @return Devuelve true si se ha cargado correctamente
	*/
    bool loadComponents(GameObject* const gameObject, FactoryManager* factMngr);
    /**
    * @brief Carga un componente
    * @param name Nombre del componente
    * @return Devuelve el componente cargado
	*/
    Component* loadComponent(std::string const& name, FactoryManager* factMngr);


public:
    SceneManager(SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

    /**
    * @brief Elimina las escenas, gameobjects y componentes
    */
    ~SceneManager();

    /**
    * @brief Carga la escena 
    * @param nombre de escena
    * @return Devuelve true si se ha cargado correctamente
	*/
    bool loadScene(std::string const& sceneName, FactoryManager* factMngr);
};
}