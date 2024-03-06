#pragma once
#include "Structure/Game.h"

#include "GraphicsEngine.h"
#include "InputManager.h"
#include "Structure/FactoryManager.h"
#include "SceneManager.h"
#include "PhysicsManager.h"
#include "UIManager.h"
// #include "AudioManager.h" Descomentar cuando se implemente

#include "Utilities/checkML.h"
#ifdef _DEBUG
#include <iostream>
#endif

Tapioca::InputManager* input;
Tapioca::FactoryManager* factories;
Tapioca::SceneManager* scenes;
Tapioca::GraphicsEngine* graphics;
Tapioca::PhysicsManager* physics;
Tapioca::UIManager* ui;
//Tapioca::AudioManager* audio;

static void createModules(Tapioca::Game*);

// TODO: solo para pruebas, borrar
#include "Node.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "Viewport.h"
#include "Utilities/Vector3.h"
#include "BillboardSet.h"
#include "Billboard.h"

int main(int argc, char** argv) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    Tapioca::Game* game = new Tapioca::Game();
    createModules(game);
    if (game->init()) {
        //* Prueba

        // IMPORTANTE: NO PREOCUPARSE POR SI NO SE PUEDE ACCEDER A UN OBJETO PARA BORRAR
        // LUEGO, EL NODO VA A BORRAR TODO
        auto nodeCamera = graphics->createNode(Tapioca::Vector3(0.0f, 0.0f, 20.0f));
        auto camera = graphics->createCamera(nodeCamera, "MainCamera");
        auto viewport = graphics->createViewport(camera, 0);
        viewport->setBackground(Tapioca::Vector3(0.925f, 0.698f, 0.941f));

        auto node = graphics->createNode();
        auto light = graphics->createLightDirectional(node, Tapioca::Vector3(0.0f, -1.0f, -1.0f));
        //auto plane = graphics->createPlane(node, 0.f, 1.f, 1.f, 0.f, "abc", 5, 5, 1, 1, 0, -1, -5);

        auto nodeBil = graphics->createNode();
        auto bil = graphics->createBillboardSet(nodeBil, "Holaaa", 2);
        bil->addBillboard(Tapioca::Vector3(0, 0, 0));
        bil->removeBillboard(0);

        //node->setParent(nodeCamera);
        //*/

        game->run();

        //*
        delete nodeBil;
        delete bil;

        delete node;
        delete nodeCamera;
        delete camera;
        delete viewport;
        delete light;
        //*/
    }
#ifdef _DEBUG
    else
        std::cerr << "Error al inicializar un modulo\n";
#endif

    delete game;

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    return 0;
}

void mapInput();

static void createModules(Tapioca::Game* game) {
    graphics = Tapioca::GraphicsEngine::create();
    game->addModule(graphics);

    ui = Tapioca::UIManager::create();
    game->addModule(ui);
    
    input = Tapioca::InputManager::create();
    game->addModule(input);
    mapInput();

    factories = Tapioca::FactoryManager::create();
    game->addModule(factories);
    
    physics = Tapioca::PhysicsManager::create();
    game->addModule(physics);
    
    scenes = Tapioca::SceneManager::create();
    game->addModule(scenes);
    
    // audio = AudioManager::create();
    // game->addModule(audio);
}

#include <lua.hpp>
#include <sstream>
void mapInput() {
    // Crea una instancia de LUA
    lua_State* luaState = luaL_newstate();

    // Construye la ruta completa al archivo LUA
    const std::string MAP_FILE = "controlsMapping.lua";
    std::string path = "assets\\controls\\" + MAP_FILE;

    // Si no puede cargar el archivo, se muestra un mensaje y cierra la instancia de LUA
    if (luaL_dofile(luaState, path.c_str()) != 0) {
#ifdef _DEBUG
        std::cerr << "Error al cargar archivo LUA: " << lua_tostring(luaState, -1) << '\n';
#endif
        lua_close(luaState);
    }
    else {
#ifdef _DEBUG
        std::cout << "Mapa de controles cargado correctamente" << '\n';
#endif
        // Mete en el stack de luaState la variable global con el nombre events
        lua_getglobal(luaState, "events");

        // Si lo que haya en el stack de luaState es una tabla
        // (-1 es el nombre de la variable, en este caso, events)
        if (lua_istable(luaState, -1)) {
            // "Entra" a leer las variables de la tabla
            lua_pushnil(luaState);

            // Itera sobre todos los elementos de la tabla
            // (-2 es el valor de la variable. En este caso, todo lo que
            // haya dentro del {} despues de events =)
            while (lua_next(luaState, -2) != 0) {
                // Lee le nombre del evento
                std::string evt = lua_tostring(luaState, -2);

                // Lo separa por el caracter "_" (por si hay
                // varios controles mapeados al mismo evento)
                std::vector<std::string> tokens;
                std::stringstream ss(evt);
                std::string token = "";
                char delimiter = '_';
                while (std::getline(ss, token, delimiter)) tokens.push_back(token);

                // Obtiene el nombre del evento ("ev" + "_" + "nombre")
                evt = tokens[0] + "_" + tokens[1];


                std::string src = "";
                int ctrl = 0;

                // Si lo que haya en el stack de luaState es una tabla
                // (-1 es el nombre de la variable, en este caso, el nombre del evento)
                if (lua_istable(luaState, -1)) {
                    // "Entra" a leer las variables de la tabla
                    lua_pushnil(luaState);

                    // Itera sobre todos los elementos de la tabla
                    // (-2 es el valor de la variable. En este caso, todo lo que
                    // haya dentro del {} despues de ev_EVENTO =)
                    while (lua_next(luaState, -2) != 0) {
                        // Lee el nombre de la variable y su valor
                        std::string key = lua_tostring(luaState, -2);
                        std::string value = lua_tostring(luaState, -1);

                        // Lo saca del stack para leer la siguiente variable
                        lua_pop(luaState, 1);

                        // Si la variable es "src", el valor es la clave
                        // del evento de inputEventTriggered. Si es "control",
                        // el valor es la tecla/boton/eje/etc mapeado al evento
                        if (key == "src") src = value;
                        else if (key == "control") ctrl = stoi(value);
                    }
                    // Añade el evento al mapa de input

                    input->mapInput(evt, src, ctrl);

                    // Saca el evento del stack para leer el siguiente
                    lua_pop(luaState, 1);
                }
            }
            // Saca events del stack, en caso de que hubiera otra variable global a leer
            lua_pop(luaState, 1);
        }
    }

    lua_close(luaState);
}