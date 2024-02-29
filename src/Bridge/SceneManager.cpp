#include "SceneManager.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <fstream>
#include <iostream>
#include <variant>
#include <map>
#include "Structure/Game.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"

#include "Utilities/checkML.h"

Tapioca::SceneManager::SceneManager(HMODULE module) : module(module), entryPoint(nullptr), L(nullptr) { }

Tapioca::SceneManager::~SceneManager() {
    while (!scenes.empty()) {
        delete scenes.top();
        scenes.pop();
    }
    for (Scene* s : scenes_debug) {
        delete s;
    }
    scenes_debug.clear();
}

bool Tapioca::SceneManager::init() {
    // Obtiene la ruta del ejecutable
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    std::string dir = std::string(buffer).substr(0, pos);

    // Construye la ruta completa al archivo LUA
    std::string path = dir + "\\assets\\scenes\\archivo.lua";

    L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_dofile(L, path.c_str()) != 0) {
        std::cerr << "Error al cargar el archivo LUA: " << lua_tostring(L, -1) << '\n';
        lua_close(L);
        return false;
    }

#ifdef _DEBUG
    std::cout << "Archivo LUA cargado correctamente\n";
#endif

    //// REALIZAR LA CREACIÓN DE LAS ESCENAS Y LOS OBJETOS CON SUS COMPONENTES
    //// A PARTIR DE LA INFORMACIÓN DEL ARCHIVO LUA
    //for (cada escena) {
    //    Scene* newScene = new Scene();
    //    for (cada objeto) {
    //        GameObject* obj = new GameObject(newScene);
    //        for (cada componente) {
    //            // comprobar si es un componente que existe en el motor
    //            // (hay que cachear la string del componente)
    //            // si esta en el motor, crearlo
    //            // HAY QUE USAR FACTORIAS
    //            Transform* comp = obj->addComponent<Transform>();   // por ejemplo
    //            comp->setPosition(Vector3(...));
    //            comp->setRotation(Vector3(...));
    //            comp->setScale(Vector3(...));
    //            // si no esta en el motor, crearlo en el juego
    //            // HAY QUE USAR FACTORIAS EN EL JUEGO
    //        }
    //    }
    //    addScene(newScene);
    //}
    
    loadScenes();

    lua_close(L);
    return true;
}


bool Tapioca::SceneManager::loadScenes() {
    lua_getglobal(L, "scenes");
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        Scene* scene;
        std::string sceneName = "";
        while (lua_next(L, -2) != 0) {
            //scene = new Scene();
            sceneName = lua_tostring(L, -2);
            std::cout << "Scene: "<< sceneName << "\n";
            scene = loadScene();
            lua_pop(L, 1);

            //TODO: esta aqui para no deja memoria de momento 
            scenes_debug.push_back(scene);
        }
    }
    return false;
}

Tapioca::Scene* Tapioca::SceneManager::loadScene() {
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        Scene* scene = new Scene();
        loadGameObjects(scene);
        return scene;
    }
    return nullptr;
}

bool Tapioca::SceneManager::loadGameObjects(Scene* scene) {
    GameObject* gameObject;
    std::string gameObjectName = "";
    while (lua_next(L, -2) != 0) {
        //scene = new Scene();
        gameObjectName = lua_tostring(L, -2);

        std::cout << "  GameObject: "<< gameObjectName << "\n";

        gameObject = loadGameObject(scene);
        scene->addObject(gameObject, "");
        lua_pop(L, 1);
    }

    return true;
}

Tapioca::GameObject* Tapioca::SceneManager::loadGameObject(Scene* scene) {
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        GameObject* gameObject = new GameObject();
        loadComponents(gameObject);
        return gameObject;
    }
    return nullptr;
}

bool Tapioca::SceneManager::loadComponents(Tapioca::GameObject* gameObject) {
    Component* component;
    std::string componentName = "";
    while (lua_next(L, -2) != 0) {
        //scene = new Scene();
        componentName = lua_tostring(L, -2);

        std::cout << "      Component: "<< componentName << "\n";

        component = loadComponent();
        //este if es porque no tengo creado componente 
        if (component != nullptr) {
            gameObject->addComponent(component, componentName);
        }
        lua_pop(L, 1);
    }
    return true;
}

Tapioca::Component* Tapioca::SceneManager::loadComponent() {
    if (lua_istable(L, -1)) {
        lua_pushnil(L);
        CompMap map;

        std::string key = "";
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

            std::string valor_debug = lua_tostring(L, -1);
            std::cout << "          key: " << key << " valor: " << valor_debug << "\n";

            map[key] = value;
            lua_pop(L, 1);
        }

        //TODO: usando map , crea mediante factoria un componente 
        return nullptr;
    }
    return nullptr;
}

void Tapioca::SceneManager::addScene(Scene* scene) { scenes.push(scene); }

void Tapioca::SceneManager::initComponents(const CompMap& variables) { scenes.top()->initComponents(variables); }

void Tapioca::SceneManager::update(const uint64_t deltaTime) {
    if (!scenes.empty()) scenes.top()->update(deltaTime);
}

void Tapioca::SceneManager::handleEvents() {
    if (!scenes.empty()) scenes.top()->handleEvents();
}

void Tapioca::SceneManager::fixedUpdate() {
    if (!scenes.empty()) scenes.top()->fixedUpdate();
}

void Tapioca::SceneManager::refresh() {
    for (Scene* sc : toDelete)
        delete sc;
    toDelete.clear();

    if (!scenes.empty()) scenes.top()->refresh();
}

void Tapioca::SceneManager::pushScene(Scene* sc) { scenes.push(sc); }

void Tapioca::SceneManager::popScene() {
    toDelete.push_back(scenes.top());
    if (!scenes.empty()) scenes.pop();
    // else /*if (scenes.empty())*/ finish = true; // TODO
}

void Tapioca::SceneManager::changeScene(Scene* sc) {
    toDelete.push_back(scenes.top());
    scenes.pop();
    scenes.push(sc);
}
