#pragma once

#include "Utilities/Singleton.h"
#include "Structure/Module.h"
#include "Structure/Scene.h"

#include <string>
#include <stack>
#include <Windows.h>

using CompValue = std::variant<char, int, float, bool, std::string>;
using CompMap = std::unordered_map<std::string, CompValue>;
typedef void(__cdecl* EntryPoint)(const char*);
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
    void addScene(Scene*);

    HMODULE module;
    EntryPoint entryPoint;
    std::stack<Scene*> scenes;
    std::vector<Scene*> toDelete;

    //TODO: esta aqui para guardar demomento escenas de prueba
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

    void initComponents(const CompMap& variables);
    void update(const uint64_t deltaTime) override;
    void handleEvents() override;
    void fixedUpdate() override;
    void refresh() override;
    void pushScene(Scene*);
    void popScene();
    void changeScene(Scene*);
};
}