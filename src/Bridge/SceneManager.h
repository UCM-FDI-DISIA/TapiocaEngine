#pragma once
#include <string>
#include <vector>
#include "Utilities/Singleton.h"
#include "Structure/Module.h"

struct lua_State;

namespace Tapioca {
class Game;
class FactoryManager;
class Scene;
class GameObject;
class Component;
class Transform;

/*
* @brief Clase Singleton y Modulo que se encarga de cargar las escenas del juego incluyendo los gameobjects y sus componentes
*/
class TAPIOCA_API SceneManager : public Singleton<SceneManager>, public Module {
private:
    friend Singleton<SceneManager>;
    friend class Scene;

    lua_State* luaState;        // Estado de lua
    FactoryManager* factMngr;   // Puntero a FactoryManager
    Game* game;                 // Puntero a Game

    /*
    * @brief Constructor por defecto
    */
    SceneManager();

    /*
    * @brief Inicializa los punteros a Game y FactoryManager
    */
    bool init() override;

    /*
    * @brief Carga una escena
    * @return Devuelve la escena cargada
    */
    bool loadScene(Scene* const scene);

    /*
    * @brief Carga los gameobjects de una escena
    * @param scene Escena a la que se le van a cargar los gameobjects
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadGameObjects(Scene* const scene);

    /*
    * @brief Carga los gameobjects de una escena con un padre
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadGameObjects(Scene* const scene, std::vector<GameObject*>& gameObjectList);
    /*
    * @brief Carga un gameobject
    * @param scene Escena a la que pertenece el gameobject
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadGameObject(GameObject* const gameObject);

    /*
    * @brief Carga los componentes de un gameobject
    * @param gameObject Gameobject al que se le van a cargar los componentes
    * @return Devuelve true si se ha cargado correctamente
	*/
    bool loadComponents(GameObject* const gameObject);
    /*
    * @brief Carga un componente
    * @param name Nombre del componente
    * @return Devuelve el componente cargado
	*/
    Component* loadComponent(std::string const& name);

    /*
    * @brief Expone los valores de la interfaz grafica
    */
    void exposeUIvalues();

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
    * @return true si se ha cargado correctamente, false si no
	*/
    bool loadScene(std::string const& sceneName);
};
}