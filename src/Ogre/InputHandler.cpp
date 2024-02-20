#include "InputHandler.h"

#include <SDL.h>
#include <array>
#include <vector>

InputHandler::InputHandler() {
    kbState_ = SDL_GetKeyboardState(0);

    text_ = "";
    resetText();

    initJoystick();

    clear();
}

InputHandler::~InputHandler() {
    for (auto cont : joysticks_) {
        SDL_JoystickClose(cont);
        cont = NULL;
    }
}

void InputHandler::initJoystick() {
    int numJoysticks = SDL_NumJoysticks();

    // No hay joysticks conectados
    if (numJoysticks < 1) {
        std::cout << "No joysticks connected\n";
        joystickConnected_ = false;
    }
    // Al menos hay un joystick conectado
    else {
        for (int i = 0; i < numJoysticks; i++) {
            SDL_Joystick* joy = SDL_JoystickOpen(i);
            if (joy != NULL) {
                // Añade el joystick al vector de joysticks
                joysticks_.push_back(SDL_JoystickOpen(i));

                // Añade un set de valores de ejes al joystick
                joysticksValues_.push_back({{0, 0}, {0, 0}});

                // Añaade un set de botones al joystick
                std::vector<bool> tempButtons(SDL_JoystickNumButtons(joy), false);
                jbState_.push_back(tempButtons);

            } else
                std::cout << "Unable to open game joystick, " << SDL_GetError() << std::endl;
        }

        // Empieza a escuchar eventos de joystick
        SDL_JoystickEventState(SDL_ENABLE);
        joystickConnected_ = true;
    }
}

void InputHandler::clear() {
    for (auto& e : eventTriggered_)
        e = false;
    for (auto& m : mbState_)
        m = false;
}

void InputHandler::onKeyDown() { eventTriggered_[e_keyDown] = true; }

void InputHandler::onKeyUp() { eventTriggered_[e_keyUp] = true; }

void InputHandler::onMouseMotion(const SDL_Event& event) {
    eventTriggered_[e_mouseMoving] = true;
    mousePos_.first = event.motion.x;
    mousePos_.second = event.motion.y;
}

void InputHandler::onMouseButtonChange(const SDL_Event& event, bool isDown) {
    eventTriggered_[e_mousebuttonDown] = true;

    switch (event.button.button) {
    case SDL_BUTTON_LEFT:
        mbState_[LEFT] = isDown;
        break;
    case SDL_BUTTON_MIDDLE:
        mbState_[MIDDLE] = isDown;
        break;
    case SDL_BUTTON_RIGHT:
        mbState_[RIGHT] = isDown;
        break;
    default:
        break;
    }
}

void InputHandler::handleWindowEvent(const SDL_Event& event) {
    switch (event.window.event) {
    case SDL_WINDOWEVENT_CLOSE:
        eventTriggered_[e_closeWindow] = true;
        break;
    default:
        break;
    }
}

void InputHandler::joyAxisMotionEvent(const SDL_Event& event) {
    int joystick = event.jaxis.which;
    int value = event.jaxis.value;

    // Stick izquierdo X
    if (event.jaxis.axis == 0) {
        if (value > JOYSTICK_DEAD_ZONE) joysticksValues_[joystick].first.setX(1);
        else if (value < -JOYSTICK_DEAD_ZONE)
            joysticksValues_[joystick].first.setX(-1);
        else
            joysticksValues_[joystick].first.setX(0);
    }
    // Stick izquierdo Y
    if (event.jaxis.axis == 1) {
        if (value > JOYSTICK_DEAD_ZONE) joysticksValues_[joystick].first.setY(1);
        else if (value < -JOYSTICK_DEAD_ZONE)
            joysticksValues_[joystick].first.setY(-1);
        else
            joysticksValues_[joystick].first.setY(0);
    }
    // Stick derecho X
    if (event.jaxis.axis == 2) {
        if (value > JOYSTICK_DEAD_ZONE) joysticksValues_[joystick].second.setX(1);
        else if (value < -JOYSTICK_DEAD_ZONE)
            joysticksValues_[joystick].second.setX(-1);
        else
            joysticksValues_[joystick].second.setX(0);
    }
    // Stick derecho Y
    if (event.jaxis.axis == 3) {
        if (value > JOYSTICK_DEAD_ZONE) joysticksValues_[joystick].second.setY(1);
        else if (value < -JOYSTICK_DEAD_ZONE)
            joysticksValues_[joystick].second.setY(-1);
        else
            joysticksValues_[joystick].second.setY(0);
    }
}

void InputHandler::joyButtonDown(const SDL_Event& event) {
    eventTriggered_[e_joyButtonDown] = true;

    int joystick = event.jaxis.which;
    jbState_[joystick][event.jbutton.button] = true;
}

void InputHandler::joyButtonUp(const SDL_Event& event) {
    eventTriggered_[e_joyButtonUp] = true;

    int joystick = event.jaxis.which;
    jbState_[joystick][event.jbutton.button] = false;
}


void InputHandler::update(const SDL_Event& event) {
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
    case SDL_WINDOWEVENT:
        handleWindowEvent(event);
        break;
    case SDL_TEXTINPUT:
        text_ += event.text.text;
        break;
    case SDL_TEXTEDITING:
        composition_ = event.edit.text;
        cursor_ = event.edit.start;
        selection_len_ = event.edit.length;
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

    // Si está activa la entrada de texto
    if (SDL_IsTextInputActive()) {
        // Borra el último char
        if (isKeyDown(SDLK_BACKSPACE)) removeLastChar();

        // Desactiva la entrada de texto y limpia el texto introducido
        else if (isKeyDown(SDLK_RETURN)) {
            SDL_StopTextInput();
            resetText();
            refresh();
            std::cout << "Ended text input\n";
        }
    }
}

void InputHandler::refresh() {
    SDL_Event event;
    clear();
    while (SDL_PollEvent(&event))
        update(event);
}


bool InputHandler::isWindowClosed() { return eventTriggered_[e_closeWindow]; }

bool InputHandler::keyDownEvent() { return eventTriggered_[e_keyDown]; }

bool InputHandler::isKeyDown(SDL_Scancode key) { return keyDownEvent() && kbState_[key] == 1; }

bool InputHandler::isKeyDown(SDL_Keycode key) { return isKeyDown(SDL_GetScancodeFromKey(key)); }

bool InputHandler::keyUpEvent() { return eventTriggered_[e_keyUp]; }

bool InputHandler::isKeyUp(SDL_Scancode key) { return keyUpEvent() && kbState_[key] == 0; }

bool InputHandler::isKeyUp(SDL_Keycode key) { return isKeyUp(SDL_GetScancodeFromKey(key)); }

bool InputHandler::isMouseMoving() { return eventTriggered_[e_mouseMoving]; }

bool InputHandler::isMouseButtonDown() { return eventTriggered_[e_mousebuttonDown]; }

const std::pair<Sint32, Sint32>& InputHandler::getMousePos() { return mousePos_; }

int InputHandler::isMouseButtonDown(MOUSEBUTTON b) { return mbState_[b]; }

std::string InputHandler::getInputText() { return text_; }

void InputHandler::resetText() { text_.clear(); }

void InputHandler::removeLastChar() {
    if (!text_.empty()) text_.pop_back();
}

bool InputHandler::isjoystickConnected() { return joystickConnected_; }

int InputHandler::joystickXValue(int joystick, int stick) {
    if (!joysticksValues_.empty()) {
        if (stick == 1) return joysticksValues_[joystick].first.getX();
        else if (stick == 2)
            return joysticksValues_[joystick].second.getX();
        else
            return 0;
    }
}

int InputHandler::joystickYValue(int joystick, int stick) {
    if (!joysticksValues_.empty()) {
        if (stick == 1) return joysticksValues_[joystick].first.getY();
        else if (stick == 2)
            return joysticksValues_[joystick].second.getY();
        else
            return 0;
    }
}

bool InputHandler::joystickButtonDownEvent() { return eventTriggered_[e_joyButtonDown]; }

bool InputHandler::joystickButtonUpEvent() { return eventTriggered_[e_joyButtonUp]; }

bool InputHandler::isJoystickButtonDown(int joystick, int button) { return jbState_[joystick][button]; }