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
class WindowManager;
class GameObject;
class Component;

/**
* @brief Carga las escenas del juego incluyendo los gameobjects y sus componentes
*/
class TAPIOCA_API SceneLoader : public Singleton<SceneLoader>, public Module {
private:
    friend Singleton<SceneLoader>;
    friend class Scene;

    lua_State* luaState;            // Estado de lua
    MainLoop* mainLoop;             // Puntero a MainLoop
    FactoryManager* factManager;    // Puntero a FactoryManager
    WindowManager* windowManager;   // Puntero a WindowManager

    // warning C4251 'Tapioca::SceneLoader::scenesPath' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::SceneLoader'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string scenesPath;   // Ruta de las escenas
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif


    /**
    * @brief Inicializa la ruta de las escenas
    */
    SceneLoader();

    /**
    * @brief Inicializa los punteros a MainLoop, FactoryManager y WindowManager
    * @return true si se ha inicializado correctamente, false si no
    */
    bool init() override;
    /**
    * @brief Consigue el nombre de la escena inicial a cargar y la carga
    * @return true si se ha inicializado correctamente, false si no
    */
    bool initConfig() override;

    /**
    * @brief Carga una escena
    * @param scene Escena a cargar
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadScene(Scene* const scene);

    /**
    * @brief Carga los MainLoopobjects de una escena
    * @param scene Escena a la que se le van a cargar los gameobjects
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadGameObjects(Scene* const scene);
    /**
    * @brief Carga un gameobject y sus hijos recursivamente
    * @param gameObject GameObject a cargar, si tiene padre se le asigna
    * @param zIndex Indice de la capa en la que se va a cargar
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadGameObject(GameObject* const gameObject, int& zIndex);
    /**
    * @brief Carga los gameobjects de una escena con un padre
    * @param scene Escena a la que se le van a cargar los gameobjects
    * @param gameObjectList Lista de gameobjects ya cargados a la que se le van a anadir los nuevos
    * @return true si se ha cargado correctamente, false si no
    */
    bool loadGameObjects(Scene* const scene, std::vector<GameObject*>& gameObjectList);

    /**
    * @brief Carga los componentes de un gameobject
    * @param gameObject Gameobject al que se le van a cargar los componentes
    * @return true si se ha cargado correctamente, false si no
	*/
    bool loadComponents(GameObject* const gameObject);
    /**
    * @brief Carga un componente
    * @param name Nombre del componente
    * @param gameObject Gameobject al que se le va a cargar el componente
    * @return true si se ha cargado correctamente, false si no
	*/
    bool loadComponent(std::string const& name, GameObject* const gameObject);

    /**
    * @brief Expone los valores de la interfaz grafica
    */
    void exposeUIvalues();

public:
    SceneLoader(SceneLoader&) = delete;
    SceneLoader(SceneLoader&&) = delete;
    SceneLoader& operator=(SceneLoader&) = delete;
    SceneLoader& operator=(SceneLoader&&) = delete;

    /**
    * @brief Elimina las escenas, gameobjects y componentes
    */
    ~SceneLoader();

    /**
    * @brief Carga la escena solicitada
    * @param sceneName Nombre de escena
    * @param active Si la escena se va a actualizar o no. Por defecto es true
    * @return Puntero a la escena cargada
	*/
    Scene* loadScene(std::string const& sceneName, const bool active = true);
};
}