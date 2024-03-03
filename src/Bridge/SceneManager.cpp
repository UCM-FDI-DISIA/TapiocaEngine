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

SceneManager::SceneManager(HMODULE module, std::string scenesPath)
    : module(module), scenesPath(scenesPath), luaState(nullptr) { }

SceneManager::~SceneManager() {
    for (Scene* s : scenes_debug)
        delete s;
    scenes_debug.clear();
}

bool SceneManager::init() {
    // Construye la ruta completa al archivo LUA
    std::string path = "assets\\scenes\\" + scenesPath;

    luaState = luaL_newstate();
    if (luaL_dofile(luaState, path.c_str()) != 0) {
#ifdef _DEBUG
        std::cerr << "Error al cargar el archivo LUA: " << lua_tostring(luaState, -1) << '\n';
#endif
        lua_close(luaState);
        return false;
    }

    if (!loadScenes()) {
#ifdef _DEBUG
        std::cerr << "Error al cargar las escenas, gameobjects y componentes\n";
#endif
        lua_close(luaState);
        return false;
    }
    // TODO: Anadir la escena preferida primero
    Game::get()->pushScene(scenes_debug.back());
    scenes_debug.pop_back();

#ifdef _DEBUG
    std::cout << "Archivo LUA cargado correctamente\n";
#endif
    lua_close(luaState);
    return true;
}

bool SceneManager::loadScenes() {
    lua_getglobal(luaState, "scenes");
    if (!lua_istable(luaState, -1)) return false;

    lua_pushnil(luaState);
    bool loaded = false;
    while (lua_next(luaState, -2) != 0) {
        Scene* scene = new Scene();
        std::string sceneName = lua_tostring(luaState, -2);

#ifdef _DEBUG
        std::cout << "Scene: " << sceneName << "\n";
#endif
        loaded = loadScene(scene);
        lua_pop(luaState, 1);

        //TODO: esta aqui para no dejar memoria de momento
        scenes_debug.push_back(scene);
    }
    return loaded;
}

bool SceneManager::loadScene(Scene* scene) {
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
        scene->addObject(gameObject, gameObjectName);
        lua_pop(luaState, 1);
    }
    return loaded;
}

bool SceneManager::loadGameObject(GameObject* gameObject) {
    if (!lua_istable(luaState, -1)) return false;

    lua_pushnil(luaState);
    return loadComponents(gameObject);
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
        // Si no tengo creado componente
        if (component != nullptr) gameObject->addComponent(component, componentName);
        lua_pop(luaState, 1);
    }
    return component != nullptr;
}

Component* SceneManager::loadComponent(std::string name) {
    if (!lua_istable(luaState, -1)) return nullptr;

    lua_pushnil(luaState);
    CompMap map;

    std::string key = "";
    CompValue value;
    while (lua_next(luaState, -2) != 0) {
        //TODO: no deberia pasar
        if (lua_istable(luaState, -1)) lua_pop(luaState, 1);
        else {
            key = lua_tostring(luaState, -2);
            if (lua_isboolean(luaState, -1)) {
                value = lua_toboolean(luaState, -1) == 1;
#ifdef _DEBUG
                std::cout << "\t\t\variable: " << key << " valor: " << (get<bool>(value) ? "true" : "false") << "\n";
#endif
            }
            else if (lua_isinteger(luaState, -1)) {
                value = (int)lua_tointeger(luaState, -1);
#ifdef _DEBUG
                std::cout << "\t\t\variable: " << key << " valor: " << get<int>(value) << "\n";
#endif
            }
            else if (lua_isnumber(luaState, -1)) {
                value = (float)lua_tonumber(luaState, -1);
#ifdef _DEBUG
                std::cout << "\t\t\variable: " << key << " valor: " << get<float>(value) << "\n";
#endif
            }
            else if (lua_isstring(luaState, -1)) {
                value = lua_tostring(luaState, -1);
#ifdef _DEBUG
                std::cout << "\t\t\variable: " << key << " valor: " << get<std::string>(value) << "\n";
#endif
            }

            map[key] = value;
            lua_pop(luaState, 1);
        }
    }

    Component* comp = FactoryManager::instance()->createComponent(name);
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

void SceneManager::start() {
    for (auto scene : scenes_debug)
        scene->start();
}
}