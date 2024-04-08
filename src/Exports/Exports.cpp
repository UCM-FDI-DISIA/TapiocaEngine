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

// Componentes
#include "Structure/BasicBuilder.h"

// Core
#include "Components/Transform.h"
// Physics
#include "Components/RigidBody.h"
// Graphics
#include "Components/MeshRenderer.h"
#include "Components/CameraComponent.h"
#include "Components/Animator.h"
#include "Components/ParticleSystemComponent.h"
#include "Components/PlaneComponent.h"
#include "Components/SpriteRenderer.h"
#include "Components/LightDirComp.h"
#include "Components/SkyplaneComponent.h"
#include "Components/SkyboxComponent.h"
// UI
#include "Components/Button.h"
#include "Components/InputText.h"
#include "Components/Text.h"
#include "Components/ImageButton.h"

//Sound
#include "Components/AudioListenerComponent.h"
#include "Components/AudioSourceComponent.h"

namespace Tapioca {
void initEngine() {
    game = Tapioca::Game::create();
    createModules(game);
}

void deleteEngine() { delete game; }

void runEngine() {
    if (game->init()) {
        createEngineBuilders();

        // PRUEBAS DE SOUND
        //scenes->loadScene("archivo2.lua");
        SoundManager::instance()->testsample();

        Scene* mainScene = scenes->loadScene(game->getMainScene());
        game->run();
    }
    else
        logError("RunEngine: Error al inicializar un modulo.");
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
    if (graphics != nullptr) window->subscribeModule(graphics);
    if (ui != nullptr) window->subscribeModule(ui);
    if (window != nullptr) window->subscribeModule(input);
}

static void createEngineBuilders() {
    logInfo("Anadiendo contructores del motor.");
    FactoryManager* manager = FactoryManager::instance();
    // Core
    manager->addBuilder(new BasicBuilder<Transform>());
    // Physiscs
    manager->addBuilder(new BasicBuilder<RigidBody>());
    // Graphics
    manager->addBuilder(new BasicBuilder<MeshRenderer>());
    manager->addBuilder(new BasicBuilder<CameraComponent>());
    manager->addBuilder(new BasicBuilder<Animator>());
    manager->addBuilder(new BasicBuilder<ParticleSystemComponent>());
    manager->addBuilder(new BasicBuilder<SpriteRenderer>());
    manager->addBuilder(new BasicBuilder<PlaneComponent>());
    manager->addBuilder(new BasicBuilder<LightDirComp>());
    manager->addBuilder(new BasicBuilder<SkyplaneComponent>());
    manager->addBuilder(new BasicBuilder<SkyboxComponent>());
    // UI
    manager->addBuilder(new BasicBuilder<Button>());
    manager->addBuilder(new BasicBuilder<InputText>());
    manager->addBuilder(new BasicBuilder<Text>());
    manager->addBuilder(new BasicBuilder<ImageButton>());

    //Sound
    manager->addBuilder(new BasicBuilder<AudioListenerComponent>());
    manager->addBuilder(new BasicBuilder<AudioSourceComponent>());
}

void mapInput() {
    // Crea una instancia de LUA
    lua_State* luaState = luaL_newstate();

    // Construye la ruta completa al archivo LUA
    const std::string MAP_FILE = "controlsMapping.lua";
    std::string path = "assets\\controls\\" + MAP_FILE;

    // Si no puede cargar el archivo, se muestra un mensaje y cierra la instancia de LUA
    if (luaL_dofile(luaState, path.c_str()) != 0) {
        logError(("MapInput: Error al cargar el archivo Lua: " + std::string(lua_tostring(luaState, -1))).c_str());
        lua_close(luaState);
    }
    else {
        logInfo("MapInput: Mapa de controles cargado correctamente.");
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