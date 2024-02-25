#include "InputManager.h"

#include <SDL_events.h>
#include "Utilities/Vector2.h"

#ifdef _DEBUG
#include <iostream>
#endif

Tapioca::InputManager::InputManager() : inputText("") {
    resetText();
    initControllers();
    clearInput();

    mapInput();
}

Tapioca::InputManager::~InputManager() {
    clearInput();
    for (auto ctrl : controllers) removeController(ctrl.first);
    controllers.clear();
}


// Para mapear el input a diferentes eventos
void Tapioca::InputManager::mapInput() {
    inputMap[ev_ACCEPT].push_back({ie_keyDown, SDLK_a});
    inputMap[ev_ACCEPT].push_back({ie_keyUp, SDLK_w});

    inputMap[ev_ACCEPT].push_back({ie_ctrlButtonDown, SDL_CONTROLLER_BUTTON_A});
    inputMap[ev_ACCEPT].push_back({ie_ctrlButtonUp, SDL_CONTROLLER_BUTTON_B});
}

// Devuelve si se ha producido el evento indicado
bool Tapioca::InputManager::eventHappened(EVENTS event) {
    bool happened = false;

    // Recorre todos los controles mapeados a event
    for (auto elem : inputMap[event]) {
        // Si ya se ha encontrado que el evento se ha 
        // producido, devuelve true y deja de buscar
        if (happened) return true;

        // Recorre todos los eventos del tipo inputEvent que se hayan producido
        for (auto& evt : inputEventTriggered[elem.first]) {
            switch (elem.first) { 
                // Si son eventos de tecla, comprueba si la tecla coincide con la mapeada
                case ie_keyDown: case ie_keyUp:
                    happened |= evt.key.keysym.sym == elem.second;
                    break;

                // Si son eventos de botón de mando, comprueba si el botón coincide con el mapeado 
                // y si el botón que coincide ha sido pulsado en el mando que ha enviado el evento
                case ie_ctrlButtonUp: case ie_ctrlButtonDown:
                    bool sameBttn = evt.cbutton.button == (SDL_GameControllerButton)elem.second;
                    bool sameCtrl = 
                        SDL_GameControllerGetButton(controllers[evt.cdevice.which], (SDL_GameControllerButton)elem.second) == evt.cbutton.state;
                    happened |= sameBttn && sameCtrl;
                    break;

                //case :
                //    break;
            }
        }
    }
    if (happened) return true;
    else return false;
}


// Comprueba si hay algún joystick conectado y si hay al menos uno, empieza a escuchar eventos de joystick
void Tapioca::InputManager::initControllers() {
    if (SDL_NumJoysticks() > 0) SDL_JoystickEventState(SDL_ENABLE);
#ifdef _DEBUG
    else std::cout << "No joysticks connected\n";
#endif
}

// Añade el mando con su deadzone por defecto al mapa de mandos 
void Tapioca::InputManager::addController(int i) {
    SDL_GameController* ctrl = SDL_GameControllerOpen(i);
    if (ctrl != nullptr) {
        controllers.insert({i, ctrl});
        deadZones.insert({i, DEFAULT_DEADZONE});

#ifdef _DEBUG
        std::cout << "Controller added (" << SDL_GameControllerName(ctrl) << ")\n";
    } 
    else std::cout << "Unable to open game controller, " << SDL_GetError() << "\n";
#else
    }
#endif
}

// Elimina el mando y su deadzone del mapa de mandos
void Tapioca::InputManager::removeController(int i) { 
    SDL_GameController* ctrl = controllers.at(i);
    if (ctrl != nullptr) { 
        deadZones.erase(i);
        SDL_GameControllerClose(ctrl);
        ctrl = nullptr;

#ifdef _DEBUG
        std::cout << "Jotstick removed\n";
#endif
    }

}


// Limpia el input
void Tapioca::InputManager::clearInput() {
    for (auto& e : inputEventTriggered) e.clear();
}

/*
void Tapioca::InputManager::joyAxisMotionEvent(const SDL_Event& event) {
    int joystick = event.jaxis.which;
    int value = event.jaxis.value;

    // Stick izquierdo X
    if (event.jaxis.axis == 0) {
        if (value > deadZones[joystick]) joysticksValues[joystick].first.x = 1;
        else if (value < -deadZones[joystick]) joysticksValues[joystick].first.x = -1;
        else joysticksValues[joystick].first.x = 0;
    }
    // Stick izquierdo Y
    if (event.jaxis.axis == 1) {
        if (value > deadZones[joystick]) joysticksValues[joystick].first.y = 1;
        else if (value < -deadZones[joystick]) joysticksValues[joystick].first.y = -1;
        else joysticksValues[joystick].first.y = 0;
    }
    // Stick derecho X
    if (event.jaxis.axis == 2) {
        if (value > deadZones[joystick]) joysticksValues[joystick].second.x = 1;
        else if (value < -deadZones[joystick]) joysticksValues[joystick].second.x = -1;
        else joysticksValues[joystick].second.x = 0;
    }
    // Stick derecho Y
    if (event.jaxis.axis == 3) {
        if (value > deadZones[joystick]) joysticksValues[joystick].second.y = 1;
        else if (value < -deadZones[joystick]) joysticksValues[joystick].second.y = -1;
        else joysticksValues[joystick].second.y = 0;
    }
}
*/


// Devuelve si se ha cerrado la ventana (si no está vacío el vector, significa que no se ha llamado el evento
bool Tapioca::InputManager::windowClosed() { return !inputEventTriggered[ie_closeWindow].empty(); }

void Tapioca::InputManager::updateState(const SDL_Event& event) {
    // Eventos de ventana
    switch (event.window.event) {
    case SDL_WINDOWEVENT_CLOSE:
        inputEventTriggered[ie_closeWindow].push_back(event);
        break;
    default:
        break;
    }

    // Eventos de input
    switch (event.type) {
    case SDL_KEYDOWN:
        inputEventTriggered[ie_keyDown].push_back(event);
        break;
    case SDL_KEYUP:
        inputEventTriggered[ie_keyUp].push_back(event);
        break;
    case SDL_MOUSEMOTION:
        inputEventTriggered[ie_mouseMoving].push_back(event);
        mousePos.first = event.motion.x;
        mousePos.second = event.motion.y;
        break;
    case SDL_MOUSEBUTTONDOWN:
        inputEventTriggered[ie_mouseButtonDown].push_back(event);
        break;
    case SDL_MOUSEBUTTONUP:
        inputEventTriggered[ie_mouseButtonUp].push_back(event);
        break;
    case SDL_TEXTINPUT:
        inputText += event.text.text;
        break;
    
    // Mando
    case SDL_JOYAXISMOTION:
        //joyAxisMotionEvent(event);
        break;
    case SDL_JOYBUTTONDOWN:
        inputEventTriggered[ie_ctrlButtonDown].push_back(event);
        break;
    case SDL_JOYBUTTONUP:
        inputEventTriggered[ie_ctrlButtonUp].push_back(event);
        break;
    case SDL_JOYDEVICEADDED:
        addController(event.cdevice.which);
        break;
    case SDL_JOYDEVICEREMOVED:
        removeController(event.cdevice.which);
        break;

    case SDL_TEXTEDITING:
        compositionText = event.edit.text;
        cursor = event.edit.start;
        selectionLen = event.edit.length;
        break;
    default:
        break;
    }


    // Si está activa la entrada de texto
    if (SDL_IsTextInputActive()) {
        // Borra el último char
        if (eventHappened(ev_TOGGLE_TEXT_INPUT)) removeLastChar();

        // Desactiva la entrada de texto y limpia el texto introducido
        else if (eventHappened(ev_TOGGLE_TEXT_INPUT)) {
            SDL_StopTextInput();
            resetText();
            update();
#ifdef _DEBUG
            std::cout << "Ended text input\n";
#endif
        }
    }
}

void Tapioca::InputManager::update() {
    SDL_Event event;
    clearInput();
    while (SDL_PollEvent(&event)) updateState(event);
}
