#pragma once
#include "Utilities/defs.h"
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
#include "Structure/Scene.h"
using namespace std;
struct lua_State;

namespace Tapioca {
class Scene;
class Game;
class Component;
class GameObject;
class Transform;

/**
* @brief Clase Singleton y Módulo que se encarga de cargar las escenas del juego incluyendo los gameobjects y sus componentes
*/
class SceneManager : public Singleton<SceneManager>, public Module {
private:
    friend Singleton<SceneManager>;
    friend class Scene;

    HMODULE module;          // Módulo cargado en la memoria del proceso
    string scenesPath;       // Ruta del archivo de configuración de escenas
    vector<Scene*> scenes_debug; // Vector de escenas cargadas
    lua_State* L;			// Estado de lua

    /*
    * @brief Constructor: Inicializa el módulo, el punto de entrada y el estado de lua
    */
    SceneManager(HMODULE module, string scenesPath = "archivo.lua");

    /*
    * @brief Carga el archivo de configuración de escenas y crea las escenas, gameobjects y componentes y los inicializa
    * @return Devuelve true si se ha cargado correctamente
    */
    bool init() override;

    /**
    * @brief Crea las escenas, gameobjects y componentes
    * @return Devuelve true si se ha cargado correctamente
    */
    bool loadScenes();
    /**
    * @brief Carga una escena
    * @return Devuelve la escena cargada
    */
    bool loadScene(Scene* scene);

    /**
    * @brief Carga los gameobjects de una escena
    * @param scene Escena a la que se le van a cargar los gameobjects
    * @return Devuelve true si se ha cargado correctamente
    */
    bool loadGameObjects(Scene* scene);
    /**
    * @brief Carga un gameobject
    * @param scene Escena a la que pertenece el gameobject
    * @return Devuelve el gameobject cargado
	*/
    bool loadGameObject(GameObject* gameObject);

    /**
    * @brief Carga los componentes de un gameobject
    * @param gameObject Gameobject al que se le van a cargar los componentes
    * @return Devuelve true si se ha cargado correctamente
	*/
    bool loadComponents(GameObject* gameObject);
    /**
    * @brief Carga un componente
    * @param name Nombre del componente
    * @return Devuelve el componente cargado
	*/
    Component* loadComponent(string name);

public:
    SceneManager(SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

    /**
    * @brief Elimina las escenas, gameobjects y componentes
    */
    ~SceneManager();
};
}