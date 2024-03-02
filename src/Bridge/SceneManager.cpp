#include "SceneManager.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <fstream>
#include <iostream>

#include "Utilities/checkML.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "Structure/FactoryManager.h"

#include "Structure/Game.h"

using namespace std;
using namespace Tapioca;

SceneManager::SceneManager(HMODULE module, string scenesPath)
    : module(module)
    , scenesPath(scenesPath)
    , L(nullptr) { }

SceneManager::~SceneManager() {
    for (Scene* s : scenes_debug) {
        delete s;
    }
    scenes_debug.clear();
}

bool SceneManager::init() {
    // Construye la ruta completa al archivo LUA
    string path = "assets\\scenes\\" + scenesPath;

    L = luaL_newstate();
    if (luaL_dofile(L, path.c_str()) != 0) {
        cerr << "Error al cargar el archivo LUA: " << lua_tostring(L, -1) << '\n';
        lua_close(L);
        return false;
    }

    if (!loadScenes()) {
        cerr << "Error al cargar las escenas, gameobjects y componentes\n";
        lua_close(L);
        return false;
    }
    // TODO: Anadir la escena preferida primero
    Game::get()->pushScene(scenes_debug.back());
    scenes_debug.pop_back();

#ifdef _DEBUG
    cout << "Archivo LUA cargado correctamente\n";
#endif
    lua_close(L);
    return true;
}

bool SceneManager::loadScenes() {
    lua_getglobal(L, "scenes");
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        bool loaded = false;
        while (lua_next(L, -2) != 0) {
            Scene* scene = new Scene();
            string sceneName = lua_tostring(L, -2);

#ifdef _DEBUG
            cout << "Scene: " << sceneName << "\n";
#endif
            loaded = loadScene(scene);
            lua_pop(L, 1);

            //TODO: esta aqui para no dejar memoria de momento
            scenes_debug.push_back(scene);
        }
        return loaded;
    }
    return false;
}

bool SceneManager::loadScene(Scene* scene) {
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        return loadGameObjects(scene);
    }
    return false;
}

bool SceneManager::loadGameObjects(Scene* scene) {
    bool loaded = false;
    while (lua_next(L, -2) != 0) {
        GameObject* gameObject = new GameObject();
        string gameObjectName = "";
        if (!lua_isinteger(L, -2)) gameObjectName = lua_tostring(L, -2);

#ifdef _DEBUG
        cout << "\tGameObject: " << gameObjectName << "\n";
#endif
        loaded = loadGameObject(gameObject);
        scene->addObject(gameObject, gameObjectName);
        lua_pop(L, 1);
    }
    return loaded;
}

bool SceneManager::loadGameObject(GameObject* gameObject) {
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        return loadComponents(gameObject);
    }
    return false;
}

bool SceneManager::loadComponents(GameObject* gameObject) {
    Component* component = nullptr;
    string componentName = "";
    while (lua_next(L, -2) != 0) {
        componentName = lua_tostring(L, -2);

#ifdef _DEBUG
        cout << "\t\tComponent: " << componentName << "\n";
#endif
        component = loadComponent(componentName);
        //este if es porque no tengo creado componente
        if (component != nullptr) {
            gameObject->addComponent(component, componentName);
        }
        lua_pop(L, 1);
    }
    return component != nullptr;
}

Component* SceneManager::loadComponent(string name) {
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        CompMap map;

        string key = "";
        CompValue value;
        while (lua_next(L, -2) != 0) {
            //TODO: no deberia pasar
            if (lua_istable(L, -1)) {
                lua_pop(L, 1);
                continue;
            }
            key = lua_tostring(L, -2);
            if (lua_isboolean(L, -1)) {
                value = lua_toboolean(L, -1) == 1;
#ifdef _DEBUG
                cout << "\t\t\tkey: " << key << " valor: " << (get<bool>(value) ? "true" : "false") << "\n";
#endif
            } else if (lua_isinteger(L, -1)) {
                value = (int)lua_tointeger(L, -1);
#ifdef _DEBUG
                cout << "\t\t\tkey: " << key << " valor: " << get<int>(value) << "\n";
#endif
            } else if (lua_isnumber(L, -1)) {
                value = (float)lua_tonumber(L, -1);
#ifdef _DEBUG
                cout << "\t\t\tkey: " << key << " valor: " << get<float>(value) << "\n";
#endif
            } else if (lua_isstring(L, -1)) {
                value = lua_tostring(L, -1);
#ifdef _DEBUG
                cout << "\t\t\tkey: " << key << " valor: " << get<string>(value) << "\n";
#endif
            }

            map[key] = value;
            lua_pop(L, 1);
        }

        Component* comp = FactoryManager::instance()->createComponent(name);
        if (comp == nullptr) {
            cerr << "No existe el componente: " << name << "\n";
            return nullptr;
        }
        comp->initComponent(map);
        return comp;
    }
    return nullptr;
}
