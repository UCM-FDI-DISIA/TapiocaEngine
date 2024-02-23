#include "InputManager.h"


#include <SDL_events.h>
#include "Utilities/Vector2.h"

#ifdef _DEBUG
#include <iostream>
#endif

Tapioca::InputManager::InputManager() : inputText("") {
    kbState = SDL_GetKeyboardState(0);
    
    resetText();
    initJoysticks();
    clearInput();

    mapInput();
}

Tapioca::InputManager::~InputManager() {
    for (auto joy : joysticks) removeJoystick(joy.first);
}

void Tapioca::InputManager::mapInput() {
    kbMapping.insert({ev_ACCEPT, SDL_SCANCODE_A});
    kbMapping.insert({ev_ACCEPT, SDL_SCANCODE_W});

    jMapping.insert({ev_ACCEPT, SDL_CONTROLLER_BUTTON_DPAD_UP});
}

bool Tapioca::InputManager::eventHappened(EVENTS ev) {
    bool kb = false, joy = false;

    for (auto k : kbMapping) {
        if (k.first == ev) kb |= kbState[k.second] == 1;
    }
    kb &= inputEventTriggered[ie_keyDown];

    for (int i = 0; i < joysticks.size(); i++) {
        for (auto b : jMapping)
            if (b.first == ev) joy |= jbState[i][b.second];
    }
    joy &= inputEventTriggered[ie_joyButtonDown];
    //return (inputEventTriggered[ie_keyDown] && sol)

    return kb || joy;
}

// Comprueba si hay algún joystick conectado y los inicializa todos
void Tapioca::InputManager::initJoysticks() {
    int initialJoysticks = SDL_NumJoysticks();

    // No hay joysticks conectados
    if (initialJoysticks < 1) {
#ifdef _DEBUG
        std::cout << "No joysticks connected\n";
#endif
        joystickConnected = false;
    }
    // Al menos hay un joystick conectado
    else {
        for (int i = 0; i < initialJoysticks; i++) {
            addJoystick(i);
        }

        // Empieza a escuchar eventos de joystick
        SDL_JoystickEventState(SDL_ENABLE);
        joystickConnected = true;
    }
}

void Tapioca::InputManager::addJoystick(int i) { 
    SDL_Joystick* joy = SDL_JoystickOpen(i);
    if (joy != nullptr) {
        // Añade el joystick al vector de joysticks
        joysticks.insert({i, SDL_JoystickOpen(i)});

        // Añade un set de valores de ejes al joystick
        joysticksValues.insert({i, {{0, 0}, {0, 0}}});

        // Añaade un set de botones al joystick
        std::vector<bool> tempButtons(SDL_JoystickNumButtons(joy), false);
        joystickDeadZones.insert({i, DEFAULT_DEADZONE});
        jbState.insert({i, tempButtons});

#ifdef _DEBUG
        std::cout << "Jotstick added\n";
#endif
    }
#ifdef _DEBUG
    else std::cout << "Unable to open game joystick, " << SDL_GetError() << "\n";
#endif
}

void Tapioca::InputManager::removeJoystick(int i) { 
    SDL_Joystick* joy = joysticks.at(i);
    if (joy != nullptr) { 
        joysticks.erase(i);
        joysticksValues.erase(i);
        joystickDeadZones.erase(i);
        jbState.erase(i);

        SDL_JoystickClose(joy);
        joy = nullptr;

#ifdef _DEBUG
        std::cout << "Jotstick removed\n";
#endif
    }

}


// Limpia el input
void Tapioca::InputManager::clearInput() {
    for (auto& e : inputEventTriggered) e = false;
    for (auto& m : mbState) m = false;
}

void Tapioca::InputManager::onMouseMotion(const SDL_Event& event) {
    inputEventTriggered[ie_mouseMoving] = true;
    mousePos.first = event.motion.x;
    mousePos.second = event.motion.y;
}

void Tapioca::InputManager::onMouseButtonChange(const SDL_Event& event, bool isDown) {
    inputEventTriggered[ie_mousebuttonDown] = true;

    switch (event.button.button) {
    case SDL_BUTTON_LEFT:
        mbState[m_left] = isDown;
        break;
    case SDL_BUTTON_MIDDLE:
        mbState[m_middle] = isDown;
        break;
    case SDL_BUTTON_RIGHT:
        mbState[m_right] = isDown;
        break;
    default:
        break;
    }
}

void Tapioca::InputManager::joyAxisMotionEvent(const SDL_Event& event) {
    int joystick = event.jaxis.which;
    int value = event.jaxis.value;

    // Stick izquierdo X
    if (event.jaxis.axis == 0) {
        if (value > joystickDeadZones[joystick]) joysticksValues[joystick].first.x = 1;
        else if (value < -joystickDeadZones[joystick]) joysticksValues[joystick].first.x = -1;
        else joysticksValues[joystick].first.x = 0;
    }
    // Stick izquierdo Y
    if (event.jaxis.axis == 1) {
        if (value > joystickDeadZones[joystick]) joysticksValues[joystick].first.y = 1;
        else if (value < -joystickDeadZones[joystick]) joysticksValues[joystick].first.y = -1;
        else joysticksValues[joystick].first.y = 0;
    }
    // Stick derecho X
    if (event.jaxis.axis == 2) {
        if (value > joystickDeadZones[joystick]) joysticksValues[joystick].second.x = 1;
        else if (value < -joystickDeadZones[joystick]) joysticksValues[joystick].second.x = -1;
        else joysticksValues[joystick].second.x = 0;
    }
    // Stick derecho Y
    if (event.jaxis.axis == 3) {
        if (value > joystickDeadZones[joystick]) joysticksValues[joystick].second.y = 1;
        else if (value < -joystickDeadZones[joystick]) joysticksValues[joystick].second.y = -1;
        else joysticksValues[joystick].second.y = 0;
    }
}

void Tapioca::InputManager::joyButtonDown(const SDL_Event& event) {
    inputEventTriggered[ie_joyButtonDown] = true;

    int joystick = event.jaxis.which;
    jbState[joystick][event.jbutton.button] = true;
}

void Tapioca::InputManager::joyButtonUp(const SDL_Event& event) {
    inputEventTriggered[ie_joyButtonUp] = true;

    int joystick = event.jaxis.which;
    jbState[joystick][event.jbutton.button] = false;
}



void Tapioca::InputManager::update(const SDL_Event& event) {
    // Eventos de ventana
    switch (event.window.event) {
    case SDL_WINDOWEVENT_CLOSE:
        inputEventTriggered[ie_closeWindow] = true;
        break;
    default:
        break;
    }

    // Eventos de input
    switch (event.type) {
    case SDL_KEYDOWN:
        onKeyDown();
        break;
    case SDL_KEYUP:
        onKeyUp();
        break;
    case SDL_MOUSEMOTION:
        onMouseMotion(event);
        break;
    case SDL_MOUSEBUTTONDOWN:
        onMouseButtonChange(event, true);
        break;
    case SDL_MOUSEBUTTONUP:
        onMouseButtonChange(event, false);
        break;
    case SDL_TEXTINPUT:
        inputText += event.text.text;
        break;
    case SDL_TEXTEDITING:
        compositionText = event.edit.text;
        cursor = event.edit.start;
        selectionLen = event.edit.length;
        break;
    case SDL_JOYAXISMOTION:
        joyAxisMotionEvent(event);
        break;
    case SDL_JOYBUTTONDOWN:
        joyButtonDown(event);
        break;
    case SDL_JOYBUTTONUP:
        joyButtonUp(event);
        break;

    default:
        break;
    }


    // Eventos de conectar/desconectar mando
    switch (event.type) {
    case SDL_JOYDEVICEADDED:
        addJoystick(event.cdevice.which);
        break;
    case SDL_JOYDEVICEREMOVED:
        removeJoystick(event.cdevice.which);
        break;
    default:
        break;
    }


    // Si está activa la entrada de texto
    if (SDL_IsTextInputActive()) {
//        // Borra el último char
//        if (isKeyDown(SDLK_BACKSPACE)) removeLastChar();
//
//        // Desactiva la entrada de texto y limpia el texto introducido
//        else if (isKeyDown(SDLK_RETURN)) {
//            SDL_StopTextInput();
//            resetText();
//            refresh();
//#ifdef _DEBUG
//            std::cout << "Ended text input\n";
//#endif
//        }
    }
}

void Tapioca::InputManager::refresh() {
    SDL_Event event;
    clearInput();
    while (SDL_PollEvent(&event)) update(event);
}
