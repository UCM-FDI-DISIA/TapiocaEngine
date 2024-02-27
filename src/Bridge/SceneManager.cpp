#include <fstream>
#include <iostream>
#include <lua.hpp>
#include <LuaBridge.h>
#include "Utilities/checkML.h"
#include "SceneManager.h"
#include "Structure/Scene.h"
#include "Structure/GameObject.h"
using namespace Tapioca;

SceneManager::SceneManager(HMODULE module) : module(module) { }

SceneManager::~SceneManager() {
    while (!scenes.empty()) {
        delete scenes.top();
        scenes.pop();
    }
}

void SceneManager::init() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    if (luaL_dofile(L, "archivo.lua") != 0) {
        std::cerr << "Error al cargar el archivo LUA: " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
    } else {
        std::cout << "Archivo LUA cargado correctamente" << std::endl;
    }

    // REALIZAR LA CREACIÓN DE LAS ESCENAS Y LOS OBJETOS CON SUS COMPONENTES
    // A PARTIR DE LA INFORMACIÓN DEL ARCHIVO LUA
    
    Scene* newScene = new Scene();
    
    GameObject* obj = new GameObject(newScene);
    //obj->addComponent();
}

void SceneManager::addScene(Scene* scene) { scenes.push(scene); }

void SceneManager::initComponents() { scenes.top()->initComponent(); }


void SceneManager::pushScene(Scene* sc) { scenes.push(sc); }

void SceneManager::popScene() {
    toDelete.push_back(scenes.top());
    scenes.pop();

    //if (scenes.empty()) finish = true;
}

void SceneManager::changeScene(Scene* sc) {
    toDelete.push_back(scenes.top());
    scenes.pop();
    scenes.push(sc);
}

//void Game::addScene(Scene* sc) { scenes.push(sc); }
//
//void SceneManager::update() { scenes.top()->update(); }
//
//void SceneManager::handleEvents() { scenes.top()->handleEvents(); }
//
//void SceneManager::fixedUpdate() { scenes.top()->fixedUpdate(); }
//
//void SceneManager::refresh() {
//    for (Scene* sc : toDelete)
//        delete sc;
//    toDelete.clear();
//
//    scenes.top()->refresh();
//}