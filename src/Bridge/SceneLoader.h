#pragma once
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
#include <string>
#include <vector>

struct lua_State;

namespace Tapioca {
class Scene;
class MainLoop;
class FactoryManager;
class GameObject;
class Component;
class WindowManager;

/*
* @brief Clase Singleton y Modulo que se encarga de cargar las escenas del juego incluyendo los gameobjects y sus componentes
*/
class TAPIOCA_API SceneLoader : public Singleton<SceneLoader>, public Module {
private:
    friend Singleton<SceneLoader>;
    friend class Scene;

    lua_State* luaState;         // Estado de lua
    MainLoop* mainLoop;          // Puntero a MainLoop
    FactoryManager* factMngr;    // Puntero a FactoryManager
    WindowManager* windowMngr;   // Puntero a WindowManager
    std::string scenesPath;      // Ruta de las escenas

    /*
    * @brief Constructor por defecto
    */
    SceneLoader();

    /*
    * @brief Inicializa los punteros a MainLoop y FactoryManager
    * @return true si se ha inicializado correctamente, false si no
    */
    bool init() override;
    /*
    * @brief Inicializa la configuracion del juego
    * @return true si se ha inicializado correctamente, false si no
    */
    bool initConfig() override;

    /*
    * @brief Carga una escena
    * @param scene Escena a cargar
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadScene(Scene* const scene);

    /*
    * @brief Carga los MainLoopobjects de una escena
    * @param scene Escena a la que se le van a cargar los gameobjects
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadGameObjects(Scene* const scene);
    /*
    * @brief Carga un gameobject y sus hijos recursivamente
    * @param gameObject GameObject a cargar, si tiene padre se le asigna
    * @param zIndex Indice de la capa en la que se va a cargar
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadGameObject(GameObject* const gameObject, int& zIndex);
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
    bool loadComponent(std::string const& name, GameObject* const gameObject);

    /*
    * @brief Expone los valores de la interfaz grafica
    */
    void exposeUIvalues();

public:
    SceneLoader(SceneLoader&) = delete;
    SceneLoader(SceneLoader&&) = delete;
    SceneLoader& operator=(SceneLoader&) = delete;
    SceneLoader& operator=(SceneLoader&&) = delete;

    /*
    * @brief Elimina las escenas, gameobjects y componentes
    */
    ~SceneLoader();

    /*
    * @brief Carga la escena solicitada
    * @param sceneName Nombre de escena
    * @return Puntero a la escena cargada
	*/
    Scene* loadScene(std::string const& sceneName, const bool active = true);
};
}