#include "SceneManager.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <fstream>
#include <iostream>
//#include <variant>
//#include <map>

#include "Utilities/checkML.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "Structure/FactoryManager.h"
using namespace std;
using namespace Tapioca;

SceneManager::SceneManager(HMODULE module)
    : module(module)
    , entryPoint(nullptr)
    , L(nullptr) { }

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
            cout << "Scene: " << sceneName << "\n";
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
    while (lua_next(L, -2) != 0) {
        //scene = new Scene();
        GameObject* gameObject;
        string gameObjectName = "";
        if (!lua_isinteger(L, -2)) gameObjectName = lua_tostring(L, -2);

#ifdef _DEBUG
        cout << "\tGameObject: " << gameObjectName << "\n";
#endif

        gameObject = loadGameObject(scene);
        scene->addObject(gameObject, gameObjectName);
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
    return true;
}

Component* SceneManager::loadComponent(std::string name) {
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
        //TODO: usando map , crea mediante factoria un componente
        //return nullptr;
    }
    return nullptr;
}
