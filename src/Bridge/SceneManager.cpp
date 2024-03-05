#include "SceneManager.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <fstream>

#include "Structure/Game.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "Structure/FactoryManager.h"
#include "Structure/Scene.h"

#include "Utilities/checkML.h"

#ifdef _DEBUG
#include <iostream>
#endif

namespace Tapioca {

SceneManager::SceneManager() : luaState(nullptr) { }

SceneManager::~SceneManager() { }

bool SceneManager::init() {
    //TODO: llamar esto en otro sitio
    loadScene("archivo.lua");
    return true;
}

bool SceneManager::loadScene(const std::string& scenesPath) {
    luaState = luaL_newstate();
    std::string path = "assets\\scenes\\" + scenesPath;
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
    Game::get()->pushScene(scene);
    lua_close(luaState);
#ifdef _DEBUG
    std::cout << "Escena cargado correctamente\n";
#endif
    return true;
}

bool SceneManager::loadScene(Scene* scene) {
    lua_getglobal(luaState, "scene");
    if (!lua_istable(luaState, -1)) return false;
    lua_pushnil(luaState);
    return loadGameObjects(scene);
}

bool SceneManager::loadGameObjects(Scene* scene) {
    bool loaded = false;
    while (lua_next(luaState, -2) != 0) {
        GameObject* gameObject = new GameObject();
        std::string gameObjectName = "";
        if (!lua_isinteger(luaState, -2)) gameObjectName = lua_tostring(luaState, -2);
#ifdef _DEBUG
        std::cout << "\tGameObject: " << gameObjectName << "\n";
#endif
        loaded = loadGameObject(gameObject);
        if (!loaded) {
            delete gameObject;
            return false;
        }

        scene->addObject(gameObject, gameObjectName);
        lua_pop(luaState, 1);
    }
    return loaded;
}

bool SceneManager::loadGameObjects(GameObject* parent) {

#ifdef _DEBUG
    std::cout << "Children: start\n";
#endif
    Scene* scene = parent->getScene();

    bool loaded = true;
    while (lua_next(luaState, -2) != 0) {
        GameObject* gameObject = new GameObject();
        std::string gameObjectName = "";
        if (!lua_isinteger(luaState, -2)) gameObjectName = lua_tostring(luaState, -2);
#ifdef _DEBUG
        std::cout << "\tGameObject: " << gameObjectName << "\n";
#endif
        loaded = loadGameObject(gameObject);
        if (!loaded) {
            delete gameObject;
            return false;
        }
        //TODO: relaciona lo de padre hijo
        scene->addObject(gameObject, gameObjectName);
        lua_pop(luaState, 1);
    }

#ifdef _DEBUG
    std::cout << "Children: end\n";
#endif
    return loaded;
}

bool SceneManager::loadGameObject(GameObject* gameObject) {
    lua_pushnil(luaState);
    std::string name = "";
    while (lua_next(luaState, -2) != 0) {
        if (!lua_isinteger(luaState, -2)) name = lua_tostring(luaState, -2);
        if (name == "components") {
            lua_pushnil(luaState);
            if (!loadComponents(gameObject)) return false;
        }
        else if (name == "children") {
            //lua_pushnil(luaState);
            //if (!loadGameObjects(gameObject)) return false;
        }
        lua_pop(luaState, 1);
    }
    return true;
}

bool SceneManager::loadComponents(GameObject* gameObject) {
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
    return component != nullptr;
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
        comp = FactoryManager::instance()->createComponent(name);
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