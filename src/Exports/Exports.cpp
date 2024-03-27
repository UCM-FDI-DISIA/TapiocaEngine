#include <lua.hpp>
#include <sstream>
#include "Exports.h"
#include "Structure/Game.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "Structure/FactoryManager.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "UIManager.h"
#include "SoundManager.h"

// PRUEBAS GRAPHICS
#include "Viewport.h"
#include "Camera.h"
#include "RenderNode.h"
#include "LightPoint.h"
#include "LightRectlight.h"
#include "LightSpotlight.h"
#include "LightDirectional.h"

namespace Tapioca {
void initEngine() {
    game = Tapioca::Game::create();
    createModules(game);
}

void deleteEngine() { delete game; }

void runEngine() {
    if (game->init()) {
        // PRUEBAS GRAPHICS
        /*auto nodeAux = graphics->createNode(Vector3(0.0f, 0.0f, -30.0f));
        auto plane1 = graphics->createPlane(nodeAux, Vector3(0.0f, 0.0f, 1.0f), 0.0f, "hola1", 100, 100, 100, 100);*/
        /*auto nodeAux2 = graphics->createNode(Vector3(0.0f, 0.0f, 0.0f));
        auto plane2 = graphics->createPlane(nodeAux2, Vector3(0.0f, 0.0f, 1.0f), 0.0f, "hola2", 50, 50, 100, 100);*/
        /*auto node = graphics->createNode(Vector3(20.0f, 0.0f, 50.0f));
        auto light = graphics->createLightSpotlight(node, Vector3(0, 0, -1));*/
        //auto light = graphics->createLightPoint(node);
        //auto light = graphics->createLightDirectional(node, Tapioca::Vector3(0.0f, -1.0f, -1.0f));
        /*auto node2 = graphics->createNode(Vector3(-20.0f, 0.0f, 70.0f));
        auto light2 = graphics->createLightSpotlight(node2, Vector3(0, 0, -1));*/
        //auto light2 = graphics->createLightDirectional(node2, Tapioca::Vector3(0.0f, 0.0f, -1.0f));
        //scenes->loadScene("archivo2.lua");

        // PRUEBAS DE UI
        scenes->loadScene("startScene.lua");

        game->run();
    }
#ifdef _DEBUG
    else
        std::cerr << "Error al inicializar un modulo\n";
#endif
}

static void createModules(Tapioca::Game* game) {
    window = Tapioca::WindowManager::create();
    game->addModule(window);

    graphics = Tapioca::GraphicsManager::create();
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

    sound = Tapioca::SoundManager::create();
    game->addModule(sound);

    // Importante: UI tiene que suscribirse antes que input
    // para ignorar eventos de input que interactuen con la UI
    // y ambos deberian ser los ultimos para que el resto de
    // eventos de ventana no se ignoren por los eventos de UI
    window->subscribeModule(graphics);
    window->subscribeModule(ui);
    window->subscribeModule(input);
}

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
                while (std::getline(ss, token, delimiter))
                    tokens.push_back(token);

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
                        // el valor es la tecla/boton/eje/etc. mapeado al evento
                        if (key == "src") src = value;
                        else if (key == "control")
                            ctrl = stoi(value);
                    }
                    // A�ade el evento al mapa de input

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
}