#include <lua.hpp>
#include <sstream>
#include "Exports.h"
#include "Structure/MainLoop.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "Structure/FactoryManager.h"
#include "SceneLoader.h"
#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "UIManager.h"
#include "SoundManager.h"
#include "LuaManager.h"
#include "LuaRegistry.h"
#include "Structure/DynamicLibraryLoader.h"

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
#include "Components/LightPointComp.h"
#include "Components/LightRectComp.h"
#include "Components/LightSpotComp.h"
#include "Components/SkyplaneComponent.h"
#include "Components/SkyboxComponent.h"
#include "Components/KeyFrameAnimator.h"
// UI
#include "Components/Button.h"
#include "Components/DropBox.h"
#include "Components/Image.h"
#include "Components/ImageTextButton.h"
#include "Components/InputText.h"
#include "Components/Line.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/Text.h"
// Sound
#include "Components/AudioListenerComponent.h"
#include "Components/AudioSourceComponent.h"

namespace Tapioca {
void initEngine() {
    mainLoop = Tapioca::MainLoop::create();
    createModules(mainLoop);
}

void deleteEngine() { delete mainLoop; }

void runEngine() {
    if (mainLoop->init()) {
        createEngineBuilders();
        registerLuaFunctions();
        if (DynamicLibraryLoader::initGame()) mainLoop->run();
		else
			logError("RunEngine: Error al inicializar el juego.");
    }
    else
        logError("RunEngine: Error al inicializar un modulo.");
}

static void createModules(Tapioca::MainLoop* mainLoop) {
    window = Tapioca::WindowManager::create();
    mainLoop->addModule(window);

    graphics = Tapioca::GraphicsManager::create();
    mainLoop->addModule(graphics);

    ui = Tapioca::UIManager::create();
    mainLoop->addModule(ui);

    input = Tapioca::InputManager::create();
    mainLoop->addModule(input);
    mapInput();

    factories = Tapioca::FactoryManager::create();
    mainLoop->addModule(factories);

    physics = Tapioca::PhysicsManager::create();
    mainLoop->addModule(physics);

    scenes = Tapioca::SceneLoader::create();
    mainLoop->addModule(scenes);

    sound = Tapioca::SoundManager::create();
    mainLoop->addModule(sound);

    lua = Tapioca::LuaManager::create();
    mainLoop->addModule(lua);

    // Importante: UI tiene que suscribirse antes que input
    // para ignorar eventos de input que interactuen con la UI
    // y ambos deberian ser los ultimos para que el resto de
    // eventos de ventana no se ignoren por los eventos de UI
    if (graphics != nullptr) window->subscribeModule(graphics);
    if (ui != nullptr) window->subscribeModule(ui);
    if (window != nullptr) window->subscribeModule(input);
}

static void createEngineBuilders() {
    logInfo("RunEngine: Anadiendo contructores del motor.");
    FactoryManager* manager = FactoryManager::instance();
    // Core
    manager->addBuilder(new BasicBuilder<Transform>());
    // Physiscs
    manager->addBuilder(new BasicBuilder<RigidBody>());
    // Graphics
    manager->addBuilder(new BasicBuilder<MeshRenderer>());
    manager->addBuilder(new BasicBuilder<CameraComponent>());
    manager->addBuilder(new BasicBuilder<Animator>());
    manager->addBuilder(new BasicBuilder<KeyFrameAnimator>());
    manager->addBuilder(new BasicBuilder<ParticleSystemComponent>());
    manager->addBuilder(new BasicBuilder<SpriteRenderer>());
    manager->addBuilder(new BasicBuilder<PlaneComponent>());
    manager->addBuilder(new BasicBuilder<LightDirComp>());
    manager->addBuilder(new BasicBuilder<LightPointComp>());
    manager->addBuilder(new BasicBuilder<LightRectComp>());
    manager->addBuilder(new BasicBuilder<LightSpotComp>());
    manager->addBuilder(new BasicBuilder<SkyplaneComponent>());
    manager->addBuilder(new BasicBuilder<SkyboxComponent>());
    // UI
    manager->addBuilder(new BasicBuilder<Button>());
    manager->addBuilder(new BasicBuilder<DropBox>());
    manager->addBuilder(new BasicBuilder<Image>());
    manager->addBuilder(new BasicBuilder<ImageTextButton>());
    manager->addBuilder(new BasicBuilder<InputText>());
    manager->addBuilder(new BasicBuilder<Line>());
    manager->addBuilder(new BasicBuilder<ProgressBar>());
    manager->addBuilder(new BasicBuilder<Slider>());
    manager->addBuilder(new BasicBuilder<Text>());
    // Sound
    manager->addBuilder(new BasicBuilder<AudioListenerComponent>());
    manager->addBuilder(new BasicBuilder<AudioSourceComponent>());
}

static void registerLuaFunctions() {
    LuaRegistry* reg = lua->getRegistry();

    luabridge::getGlobalNamespace(lua->getLuaState())
        .beginNamespace("Tapioca")
        .addFunction("loadScene",
                     [](std::string name) -> bool {
                         //logInfo("loadScene llamado desde Lua");
                         return scenes->loadScene(name) != nullptr;
                     })
        .addFunction("exit",
                     []() -> void {
                         //logInfo("exit llamado desde Lua");
                         mainLoop->exit();
                     })
        .addFunction("deleteScene",
                     [](std::string name) -> void {
                         //logInfo("deleteScene llamado desde Lua");
                         mainLoop->deleteScene(name);
                     })
        //.addFunction("", []() -> void {})
        .endNamespace();
}

void mapInput() {
    // Crea una instancia de LUA
    lua_State* luaState = luaL_newstate();

    // Construye la ruta completa al archivo LUA
    const std::string MAP_FILE = "controlsMapping.lua";
    std::string path = "assets\\" + MAP_FILE;

    // Si no puede cargar el archivo, se muestra un mensaje y cierra la instancia de LUA
    if (luaL_dofile(luaState, path.c_str()) != 0) {
        logError(("MapInput: Error al cargar el archivo Lua: " + std::string(lua_tostring(luaState, -1))).c_str());
        lua_close(luaState);
        return;
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
                    // Anade el evento al mapa de input

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