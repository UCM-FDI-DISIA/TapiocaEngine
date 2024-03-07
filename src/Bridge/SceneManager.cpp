#include "SceneManager.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <fstream>

#include "Structure/Game.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "Structure/FactoryManager.h"
#include "Structure/Scene.h"

namespace Tapioca {

SceneManager::SceneManager() : luaState(nullptr), factMngr(nullptr), game(nullptr) { }

bool SceneManager::init() {
    game = Game::instance();
    factMngr = FactoryManager::instance();
    return true;
}

SceneManager::~SceneManager() { }

bool SceneManager::loadScene(std::string const& sceneName) {
    if (factMngr == nullptr || game == nullptr) {
#ifdef _DEBUG
        std::cerr << "FactoryManager invalido\n";
#endif
        return false;
    }

    luaState = luaL_newstate();
    std::string path = "assets\\scenes\\" + sceneName;
    if (luaL_dofile(luaState, path.c_str()) != 0) {
#ifdef _DEBUG
        std::cerr << "Error al cargar el archivo LUA: " << lua_tostring(luaState, -1) << '\n';
#endif
        lua_close(luaState);
        return false;
    }
    Scene* scene = new Scene();
    if (!loadScene(scene)) {
#ifdef _DEBUG
        std::cerr << "Error al cargar de escena, gameobjects y componentes\n";
#endif
        lua_close(luaState);
        delete scene;
        return false;
    }
    game->pushScene(scene);
    lua_close(luaState);
#ifdef _DEBUG
    std::cout << "Escena cargado correctamente\n";
#endif
    return true;
}

bool SceneManager::loadScene(Scene* const scene) {
    lua_getglobal(luaState, "scene");
    if (!lua_istable(luaState, -1)) return false;
    lua_pushnil(luaState);
    return loadGameObjects(scene);
}

bool SceneManager::loadGameObjects(Scene* const scene) {
    while (lua_next(luaState, -2) != 0) {
        GameObject* gameObject = new GameObject();
        std::string gameObjectName = "";

        if (!lua_isinteger(luaState, -2)) gameObjectName = lua_tostring(luaState, -2);
#ifdef _DEBUG
        std::cout << "\tGameObject: " << gameObjectName << "\n";
#endif
        if (!scene->addObject(gameObject, gameObjectName) || !loadGameObject(gameObject)) {
            return false;
        }

        lua_pop(luaState, 1);
    }
    return true;
}

bool SceneManager::loadGameObject(GameObject* const gameObject) {
    lua_pushnil(luaState);
    std::string name = "";

    std::vector<GameObject*> children;

    while (lua_next(luaState, -2) != 0) {
        if (!lua_isinteger(luaState, -2)) name = lua_tostring(luaState, -2);
        if (name == "components") {
            lua_pushnil(luaState);
            if (!loadComponents(gameObject)) return false;
        }
        else if (name == "children") {
            lua_pushnil(luaState);
            if (!loadGameObjects(gameObject->getScene(), children)) {

                return false;
            }
        }
        lua_pop(luaState, 1);
    }
    //relacionar padre hijo
    Transform* tr = gameObject->getComponent<Transform>();
    for (GameObject* obj : children) {
        obj->getComponent<Transform>()->setParent(tr);
    }
    children.clear();
    return true;
}

bool SceneManager::loadGameObjects(Scene* const scene, std::vector<GameObject*>& gameObjects) {

#ifdef _DEBUG
    std::cout << "Children: start\n";
#endif
    while (lua_next(luaState, -2) != 0) {
        GameObject* gameObject = new GameObject();
        std::string gameObjectName = "";
        if (!lua_isinteger(luaState, -2)) gameObjectName = lua_tostring(luaState, -2);
        scene->addObject(gameObject, gameObjectName);
        gameObjects.push_back(gameObject);
#ifdef _DEBUG
        std::cout << "\tGameObject: " << gameObjectName << "\n";
#endif
        if (!loadGameObject(gameObject)) return false;

        lua_pop(luaState, 1);
    }

#ifdef _DEBUG
    std::cout << "Children: end\n";
#endif
    return true;
}

bool SceneManager::loadComponents(GameObject* const gameObject) {
    Component* component = nullptr;
    std::string componentName = "";
    while (lua_next(luaState, -2) != 0) {
        componentName = lua_tostring(luaState, -2);

#ifdef _DEBUG
        std::cout << "\t\tComponent: " << componentName << "\n";
#endif
        component = loadComponent(componentName);
        if (component == nullptr) return false;
        // Si no tengo creado componente
        gameObject->addComponent(component, componentName);
        lua_pop(luaState, 1);
    }
    if (gameObject->getComponent<Transform>() == nullptr) {
        gameObject->addComponent<Transform>();
    }
    return true;
}

Component* SceneManager::loadComponent(std::string const& name) {
    if (!lua_istable(luaState, -1)) return nullptr;

    lua_pushnil(luaState);
    CompMap map;

    std::string key = "";
    CompValue value;
    bool load = true;

    while (lua_next(luaState, -2) != 0 && load) {
        if (lua_istable(luaState, -1)) {
            lua_pop(luaState, 1);
            load = false;
        }
        else {
            key = lua_tostring(luaState, -2);
            if (lua_isboolean(luaState, -1)) {
                value = lua_toboolean(luaState, -1) == 1;
#ifdef _DEBUG
                std::cout << "\t\tvariable: " << key << " valor: " << (get<bool>(value) ? "true" : "false") << "\n";
#endif
            }
            else if (lua_isinteger(luaState, -1)) {
                value = (int)lua_tointeger(luaState, -1);
#ifdef _DEBUG
                std::cout << "\t\tvariable: " << key << " valor: " << get<int>(value) << "\n";
#endif
            }
            else if (lua_isnumber(luaState, -1)) {
                value = (float)lua_tonumber(luaState, -1);
#ifdef _DEBUG
                std::cout << "\t\tvariable: " << key << " valor: " << get<float>(value) << "\n";
#endif
            }
            else if (lua_isstring(luaState, -1)) {
                value = lua_tostring(luaState, -1);
#ifdef _DEBUG
                std::cout << "\t\tvariable: " << key << " valor: " << get<std::string>(value) << "\n";
#endif
            }

            map[key] = value;
            lua_pop(luaState, 1);
        }
    }
    Component* comp = nullptr;
    if (load) {
        comp = factMngr->createComponent(name);
    }
    if (comp == nullptr) {
#ifdef _DEBUG
        std::cerr << "No existe el componente: " << name << "\n";
#endif
        return nullptr;
    }

    if (!comp->initComponent(map)) {
#ifdef _DEBUG
        std::cerr << "Error al inicializar el componente: " << name << "\n";
#endif
        delete comp;
        return nullptr;
    }

    return comp;
}
}