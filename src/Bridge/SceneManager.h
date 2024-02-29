#pragma once
#include "Utilities/defs.h"
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
#include "Structure/Scene.h"
struct lua_State;

namespace Tapioca {
    class Scene;
    class Game;
    class Component;
    class GameObject;
    class Transform;

class SceneManager : public Singleton<SceneManager>, public Module {
private:
    friend Singleton<SceneManager>;
    friend class Scene;

    HMODULE module;
    EntryPoint entryPoint;

    //TODO: esta aqui para guardar de momento escenas de prueba
    std::vector<Scene*> scenes_debug;

    lua_State* L;

    SceneManager(HMODULE module);

    bool init() override;

    bool loadScenes();
    Scene* loadScene();

    bool loadGameObjects(Scene* scene);
    GameObject* loadGameObject(Scene* scene);

    bool loadComponents(GameObject* gameObject);
    Component* loadComponent();

public:
    ~SceneManager();

    SceneManager(SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;
};
}