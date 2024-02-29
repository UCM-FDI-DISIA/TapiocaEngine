#include "SceneManager.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <fstream>
#include <iostream>
#include <variant>
#include <map>

#include "Utilities/checkML.h"
#include "Structure/Game.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
using namespace std;
using namespace Tapioca;

SceneManager::SceneManager(HMODULE module) : module(module), entryPoint(nullptr), L(nullptr) { }

SceneManager::~SceneManager() {
    for (Scene* s : scenes_debug) {
        delete s;
    }
    scenes_debug.clear();
}

bool SceneManager::init() {
    // Obtiene la ruta del ejecutable
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    string::size_type pos = string(buffer).find_last_of("\\/");
    string dir = string(buffer).substr(0, pos);

    // Construye la ruta completa al archivo LUA
    string path = dir + "\\assets\\scenes\\archivo.lua";

    L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_dofile(L, path.c_str()) != 0) {
        cerr << "Error al cargar el archivo LUA: " << lua_tostring(L, -1) << '\n';
        lua_close(L);
        return false;
    }

#ifdef _DEBUG
    cout << "Archivo LUA cargado correctamente\n";
#endif
    
    loadScenes();
    lua_close(L);
    return true;
}

bool SceneManager::loadScenes() {
    lua_getglobal(L, "scenes");
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        Scene* scene;
        string sceneName = "";
        while (lua_next(L, -2) != 0) {
            //scene = new Scene();
            sceneName = lua_tostring(L, -2);
            cout << "Scene: "<< sceneName << "\n";
            scene = loadScene();
            lua_pop(L, 1);

            //TODO: esta aqui para no dejar memoria de momento 
            scenes_debug.push_back(scene);
        }
    }
    return false;
}

Scene* SceneManager::loadScene() {
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        Scene* scene = new Scene();
        loadGameObjects(scene);
        return scene;
    }
    return nullptr;
}

bool SceneManager::loadGameObjects(Scene* scene) {
    GameObject* gameObject;
    string gameObjectName = "";
    while (lua_next(L, -2) != 0) {
        //scene = new Scene();
        gameObjectName = lua_tostring(L, -2);

        cout << "\tGameObject: "<< gameObjectName << "\n";

        gameObject = loadGameObject(scene);
        scene->addObject(gameObject, "");
        lua_pop(L, 1);
    }

    return true;
}

GameObject* SceneManager::loadGameObject(Scene* scene) {
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        GameObject* gameObject = new GameObject();
        loadComponents(gameObject);
        return gameObject;
    }
    return nullptr;
}

bool SceneManager::loadComponents(GameObject* gameObject) {
    Component* component;
    string componentName = "";
    while (lua_next(L, -2) != 0) {
        //scene = new Scene();
        componentName = lua_tostring(L, -2);

        cout << "\t\tComponent: "<< componentName << "\n";

        component = loadComponent();
        //este if es porque no tengo creado componente 
        if (component != nullptr) {
            gameObject->addComponent(component, componentName);
        }
        lua_pop(L, 1);
    }
    return true;
}

Component* SceneManager::loadComponent() {
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        CompMap map;

        string key = "";
        CompValue value;
        while (lua_next(L, -2) != 0) {
            if (lua_istable(L, -1)) {
                lua_pop(L, 1);
                continue;
            }
            key = lua_tostring(L, -2);
            if (lua_isstring(L, -1)) {
                value = lua_tostring(L, -1);
            } else if (lua_isinteger(L, -1)) {
                value =(int) lua_tointeger(L, -1);
            } else if (lua_isboolean(L, -1)) {
                value = lua_toboolean(L, -1);
            } else {
                value = lua_tostring(L, -1);
                //value = *(float*)lua_touserdata(L, -1);
            }

            string valor_debug = lua_tostring(L, -1);
            cout << "\t\t\tkey: " << key << " valor: " << valor_debug << "\n";

            map[key] = value;
            lua_pop(L, 1);
        }

        //TODO: usando map , crea mediante factoria un componente 
        return nullptr;
    }
    return nullptr;
}
