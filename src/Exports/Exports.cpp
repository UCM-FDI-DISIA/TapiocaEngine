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

// PRUEBAS DE UI
#include <SDL.h>
#include "Structure/DynamicLibraryLoader.h"
#include "Text.h"
#include "Button.h"
#include "InputText.h"

namespace Tapioca {
void initEngine() {
    game = Tapioca::Game::create();
    createModules(game);
}

void deleteEngine() { delete game; }

void runEngine() {
    if (game->init()) {
        // PRUEBAS DE UI
        Button::ButtonOptions buttonOptions;
        buttonOptions.name = "Boton1";
        buttonOptions.constSize = ImVec2(130, 40);
        buttonOptions.position = ImVec2((window->getWindowW() - buttonOptions.constSize.x) / 2,
                                        (window->getWindowH() - buttonOptions.constSize.y) / 2);
        buttonOptions.text = "Play";
        buttonOptions.onClick = [&]() {
            if (!DynamicLibraryLoader::initGame()) {
                Button* button = ui->getButton(buttonOptions.name);
                if (button != nullptr) {
                    button->setText("Couldn't init game");
                    button->setTextColor(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                }
            }
        };
        buttonOptions.textFont = ui->getFont("impact.ttf");
        auto button = ui->createButton(buttonOptions);

        InputText::InputTextOptions inputOptions;
        inputOptions.name = "Input1";
        inputOptions.position = ImVec2(0, 0);
        inputOptions.placeHolderText = "Inserta nombre";
        inputOptions.onTextEntered = [&]() {
            InputText* input = ui->getInputText(inputOptions.name);
            if (input != nullptr) {
#ifdef _DEBUG
                std::cout << "Se ha hecho ENTER en la caja de texto\n";
#endif
                input->setPlaceHolderText("Se ha hecho ENTER");
                input->setTextColor(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                input->setBgColor(ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            }
        };
        inputOptions.constWidth = 500;
        inputOptions.bufferSize = 100;
        inputOptions.textFont = ui->getFont("arial.ttf", 20.0f);
        inputOptions.textColor = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
        auto input = ui->createInputText(inputOptions);

        Text::TextOptions textOptions;
        textOptions.name = "Texto1"; 
        textOptions.position = ImVec2(500, 400.0f);
        textOptions.text = "Tapioca Engine";
        textOptions.constSize = ImVec2(100, 100);
        textOptions.textFont = ui->getFont("AGENCYB.TTF", 30.0f);
        textOptions.textColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
        auto text = ui->createText(textOptions);

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

    window->subscribeModule(input);
    window->subscribeModule(graphics);
    window->subscribeModule(ui);
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