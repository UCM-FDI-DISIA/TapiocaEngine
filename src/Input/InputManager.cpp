#include "InputManager.h"

#include <SDL.h>
#include <imgui_impl_sdl2.h>      // Para gestionar los eventos de la interfaz
//#include <lua.hpp>
#include <sstream>
#include "Structure/Game.h"
//#include "GraphicsEngine.h"

#include "Utilities/checkML.h"
#ifdef _DEBUG
#include <iostream>
#endif


namespace Tapioca {

InputManager::InputManager() : inputText(""), luaState(nullptr) {
    if (!SDL_WasInit(SDL_INIT_EVERYTHING)) SDL_Init(SDL_INIT_EVERYTHING);

    inputEventTriggered = {
        {"ie_closeWindow", {}}, 
        {"ie_keyUp", {}}, 
        {"ie_keyDown", {}},

        {"ie_mouseMoving", {}}, 
        {"ie_mouseButtonUp", {}}, 
        {"ie_mouseButtonDown", {}}, 
        {"ie_mouseWheel", {}},

        {"ie_ctrlAxisMotion", {}}, 
        {"ie_ctrlButtonUp", {}}, 
        {"ie_ctrlButtonDown", {}}
    };
    mapInput();
}

bool InputManager::init() { 
    resetText();
    initControllers();
    clearInput();

    /*sdlWindow = GraphicsEngine::instance()->getSDLWindow();
    ogreWindow = GraphicsEngine::instance()->getOgreWindow();*/

    return true;
}

InputManager::~InputManager() {
    clearInput();
    for (auto ctrl : controllers) removeController(ctrl.first);
    controllers.clear();
}


void InputManager::mapInput() {
//    // Crea una instancia de LUA
//    luaState = luaL_newstate();
//
//    // Construye la ruta completa al archivo LUA
//    std::string path = "assets\\controls\\" + MAP_FILE;
//
//    // Si no puede cargar el archivo, se muestra un mensaje y cierra la instancia de LUA
//    if (luaL_dofile(luaState, path.c_str()) != 0) {
//#ifdef _DEBUG
//        std::cerr << "Error al cargar archivo LUA: " << lua_tostring(luaState, -1) << '\n';
//#endif
//        lua_close(luaState);
//    } 
//    else {
//#ifdef _DEBUG
//        std::cout << "Mapa de controles cargado correctamente" << '\n';
//#endif
//        // Mete en el stack de luaState la variable global con el nombre events
//        lua_getglobal(luaState, "events");
//
//        // Si lo que haya en el stack de luaState es una tabla
//        // (-1 es el nombre de la variable, en este caso, events)
//        if (lua_istable(luaState, -1)) {
//            // "Entra" a leer las variables de la tabla
//            lua_pushnil(luaState);
//
//            // Itera sobre todos los elementos de la tabla
//            // (-2 es el valor de la variable. En este caso, todo lo que
//            // haya dentro del {} despues de events =)
//            while (lua_next(luaState, -2) != 0) {
//                // Lee le nombre del evento
//                std::string evt = lua_tostring(luaState, -2);
//
//                // Lo separa por el caracter "_" (por si hay
//                // varios controles mapeados al mismo evento)
//                std::vector<std::string> tokens;
//                std::stringstream ss(evt);
//                std::string token = "";
//                char delimiter = '_';
//                while (std::getline(ss, token, delimiter)) tokens.push_back(token);
//
//                // Obtiene el nombre del evento ("ev" + "_" + "nombre")
//                evt = tokens[0] + "_" + tokens[1];
//
//
//                std::string src = "";
//                int ctrl = 0;
//
//                // Si lo que haya en el stack de luaState es una tabla
//                // (-1 es el nombre de la variable, en este caso, el nombre del evento)
//                if (lua_istable(luaState, -1)) {
//                    // "Entra" a leer las variables de la tabla
//                    lua_pushnil(luaState);
//
//                    // Itera sobre todos los elementos de la tabla
//                    // (-2 es el valor de la variable. En este caso, todo lo que
//                    // haya dentro del {} despues de ev_EVENTO =)
//                    while (lua_next(luaState, -2) != 0) {
//                        // Lee el nombre de la variable y su valor
//                        std::string key = lua_tostring(luaState, -2);
//                        std::string value = lua_tostring(luaState, -1);
//
//                        // Lo saca del stack para leer la siguiente variable
//                        lua_pop(luaState, 1);
//
//                        // Si la variable es "src", el valor es la clave
//                        // del evento de inputEventTriggered. Si es "control",
//                        // el valor es la tecla/boton/eje/etc mapeado al evento
//                        if (key == "src") src = value;
//                        else if (key == "control")
//                            ctrl = stoi(value);
//                    }
//                    // Añade el evento al mapa de input
//                    inputMap[evt].push_back({src, ctrl});
//
//                    // Saca el evento del stack para leer el siguiente
//                    lua_pop(luaState, 1);
//                }
//            }
//            // Saca events del stack, en caso de que hubiera otra variable global a leer
//            lua_pop(luaState, 1);
//        }
//    }
//
//
//    lua_close(luaState);
}

bool InputManager::eventHappened(std::string const& event) {
    if (inputMap.find(event) == inputMap.end()) return false;

    bool happened = false;

    // Recorre todos los controles mapeados a event
    for (auto elem : inputMap[event]) {
        // Si ya se ha encontrado que el evento se ha
        // producido, devuelve true y deja de buscar
        if (happened) return true;

        // Recorre todos los eventos del tipo inputEvent que se hayan producido
        for (auto& evt : inputEventTriggered[elem.first]) {
            // Si son eventos de tecla, comprueba si la tecla coincide con la mapeada
            if (elem.first == "ie_keyDown" || elem.first == "ie_keyUp") happened = evt.key.keysym.sym == elem.second;

            // Si son eventos de movimiento de raton, lo pone en true
            else if (elem.first == "ie_mouseMoving")
                happened = true;

            // Si son eventos de botones de raton, comprueba si el boton coincide con el mapeado
            else if (elem.first == "ie_mouseButtonUp" || elem.first == "ie_mouseButtonDown")
                happened = evt.button.button == elem.second;

            // Si son eventos de movimiento de ejes, comprueba si el eje del mando supera la
            // deadzone y si el eje que coincide es el del mando que ha enviado el evento
            else if (elem.first == "ie_ctrlAxisMotion")
                happened = evt.caxis.axis == (SDL_GameControllerAxis)elem.second &&
                           SDL_GameControllerGetAxis(controllers[evt.cdevice.which], (SDL_GameControllerAxis)elem.second) >= deadZones[evt.cdevice.which];

            // Si son eventos de boton de mando, comprueba si el boton coincide con el mapeado
            // y si el boton que coincide ha sido pulsado en el mando que ha enviado el evento
            else if (elem.first == "ie_ctrlButtonUp" || elem.first == "ie_ctrlButtonDown")
                happened = evt.cbutton.button == (SDL_GameControllerButton)elem.second &&
                           SDL_GameControllerGetButton(controllers[evt.cdevice.which], (SDL_GameControllerButton)elem.second) == evt.cbutton.state;
        }
    }
    if (happened) return true;
    else return false;
}


void InputManager::initControllers() {
    if (SDL_NumJoysticks() > 0) SDL_JoystickEventState(SDL_ENABLE);
#ifdef _DEBUG
    else std::cout << "No hay joysticks conectados\n";
#endif
}

void InputManager::addController(int i) {
    SDL_GameController* ctrl = SDL_GameControllerOpen(i);
    if (ctrl != nullptr) {
        controllers.insert({i, ctrl});
        deadZones.insert({i, DEFAULT_DEADZONE});

#ifdef _DEBUG
        std::cout << "Mando conectado (" << SDL_GameControllerName(ctrl) << ")\n";
    } 
    else std::cerr << "No se pudo abrir mando, " << SDL_GetError() << "\n";
#else
    }
#endif
}

void InputManager::removeController(int i) {
    SDL_GameController* ctrl = controllers.at(i);
    if (ctrl != nullptr) {
        deadZones.erase(i);
        SDL_GameControllerClose(ctrl);
        ctrl = nullptr;

#ifdef _DEBUG
        std::cout << "Mando desconectado\n";
#endif
    }
}


void InputManager::clearInput() {
    for (auto& e : inputEventTriggered) e.second.clear();
}

void InputManager::updateState(const SDL_Event& event) {
    // Eventos de input
    switch (event.type) {
    // Ventana
    case SDL_WINDOWEVENT_CLOSE: case SDL_QUIT:
        Game::get()->exit();
        break;
    /*
    case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            int x, y;
            SDL_GetWindowSize(sdlWindow, &x, &y);
            ogreWindow->resize(x, y);
        }
        break;
    */
    // Teclado
    case SDL_KEYDOWN:
        inputEventTriggered["ie_keyDown"].push_back(event);
        break;
    case SDL_KEYUP:
        inputEventTriggered["ie_keyUp"].push_back(event);
        break;

    // Raton
    case SDL_MOUSEMOTION:
        inputEventTriggered["ie_mouseMoving"].push_back(event);
        mousePos.first = event.motion.x;
        mousePos.second = event.motion.y;
        break;
    case SDL_MOUSEBUTTONDOWN:
        inputEventTriggered["ie_mouseButtonDown"].push_back(event);
        break;
    case SDL_MOUSEBUTTONUP:
        inputEventTriggered["ie_mouseButtonUp"].push_back(event);
        break;
    case SDL_MOUSEWHEEL:
        inputEventTriggered["ie_mouseWheel"].push_back(event);
        break;

    // Mando
    case SDL_CONTROLLERAXISMOTION:
        inputEventTriggered["ie_ctrlAxisMotion"].push_back(event);
        break;
    case SDL_CONTROLLERBUTTONDOWN:
        inputEventTriggered["ie_ctrlButtonDown"].push_back(event);
        break;
    case SDL_CONTROLLERBUTTONUP:
        inputEventTriggered["ie_ctrlButtonUp"].push_back(event);
        break;
    case SDL_JOYDEVICEADDED:
        addController(event.cdevice.which);
        break;
    case SDL_JOYDEVICEREMOVED:
        removeController(event.cdevice.which);
        break;

    // Input de texto
    case SDL_TEXTINPUT:
        inputText += event.text.text;
        break;
    case SDL_TEXTEDITING:
        compositionText = event.edit.text;
        cursor = event.edit.start;
        selectionLen = event.edit.length;
        break;

    default:
        break;
    }


    // Si esta activa la entrada de texto
    if (SDL_IsTextInputActive()) {
        // Borra el ultimo char
        if (eventHappened("ev_TOGGLE_TEXT_INPUT")) removeLastChar();

        // Desactiva la entrada de texto y limpia el texto introducido
        else if (eventHappened("ev_TOGGLE_TEXT_INPUT")) {
            SDL_StopTextInput();
            resetText();
            handleEvents();
#ifdef _DEBUG
            std::cout << "Entrada de texto finalizada\n";
#endif
        }
    }
}

void InputManager::handleEvents() {
    SDL_Event event;
    clearInput();
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        updateState(event);
    }

    if (eventHappened("ev_CLOSE")) Game::get()->exit();

#ifdef _DEBUG
    if (eventHappened("ev_ACCEPT")) std::cout << "Aceptar\n";
#endif
}


}
