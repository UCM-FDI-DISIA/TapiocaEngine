#pragma once
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
#include <string>
#include <vector>

struct lua_State;

namespace Tapioca {
class Scene;
class Game;
class FactoryManager;
class GameObject;
class Component;

/*
* @brief Clase Singleton y Modulo que se encarga de cargar las escenas del juego incluyendo los gameobjects y sus componentes
*/
class TAPIOCA_API SceneManager : public Singleton<SceneManager>, public Module {
private:
    friend Singleton<SceneManager>;
    friend class Scene;

    lua_State* luaState;        // Estado de lua
    Game* game;                 // Puntero a Game
    FactoryManager* factMngr;   // Puntero a FactoryManager
    std::string scenesPath;     // Ruta de las escenas

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
    * @param scene Escena a cargar
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadScene(Scene* const scene);

    /*
    * @brief Carga los gameobjects de una escena
    * @param scene Escena a la que se le van a cargar los gameobjects
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadGameObjects(Scene* const scene);
    /*
    * @brief Carga un gameobject y sus hijos recursivamente
    * @param gameObject GameObject a cargar, si tiene padre se le asigna
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadGameObject(GameObject* const gameObject);
    /*
    * @brief Carga los gameobjects de una escena con un padre
    * @param scene Escena a la que se le van a cargar los gameobjects
    * @param gameObjectList Lista de gameobjects ya cargados a la que se le van a anadir los nuevos
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadGameObjects(Scene* const scene, std::vector<GameObject*>& gameObjectList);

    /*
    * @brief Carga los componentes de un gameobject
    * @param gameObject Gameobject al que se le van a cargar los componentes
    * @return true si se ha cargado correctamente, false si no
	*/
    bool loadComponents(GameObject* const gameObject);
    /*
    * @brief Carga un componente
    * @param name Nombre del componente
    * @return Puntero al componente cargado
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

    /*
    * @brief Elimina las escenas, gameobjects y componentes
    */
    ~SceneManager();

    /*
    * @brief Carga la escena solicitada
    * @param sceneName Nombre de escena
    * @return true si se ha cargado correctamente, false si no
	*/
    bool loadScene(std::string const& sceneName);
};
}