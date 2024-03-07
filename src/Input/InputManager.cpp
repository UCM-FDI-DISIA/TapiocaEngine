#include "InputManager.h"

#include <SDL.h>
#include <imgui_impl_sdl2.h>   // Para gestionar los eventos de la interfaz
#include "Structure/Game.h"

namespace Tapioca {

InputManager::InputManager()
    : /*inputText(""), compositionText(nullptr), cursor(0), selectionLen(0), removeChar(false), toggleTextInput(false),*/ mousePos({ 0, 0 }) { }

bool InputManager::init() {
    if (!SDL_WasInit(SDL_INIT_EVERYTHING)) SDL_Init(SDL_INIT_EVERYTHING);
    /*resetText();*/
    initControllers();
    return true;
}

InputManager::~InputManager() {
    for (auto ctrl : controllers) removeController(ctrl.first);
    controllers.clear();
}


void InputManager::mapInput(std::string const& evt, std::string const& src, int const& ctrl) {
    int control = ctrl;
    if (src == "ie_mouseMoving") control = MOUSE_MOTION_VALUE;
    else if (src == "ie_mouseWheel") control = MOUSE_WHEEL_VALUE;
    inputMap[src][control].push_back(evt);
}


void InputManager::initControllers() {
    if (SDL_NumJoysticks() > 0) SDL_JoystickEventState(SDL_ENABLE);
#ifdef _DEBUG
    else std::cout << "No hay joysticks conectados\n";
#endif
}

void InputManager::addController(const int i) {
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

void InputManager::removeController(const int i) {
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

void Tapioca::InputManager::sendEvent(std::string const& eventName, SDL_Event const& event, int const& value) {
    // Si el origen del evento no ha sido mapeado, lo ignora
    if (inputMap.find(eventName) == inputMap.end()) return;

    // Si no, si la tecla/boton/etc no ha sido mapeado, lo ignora
    else if (inputMap[eventName].find(value) == inputMap[eventName].end()) return;
    // Si no, envia todos los eventos que origine eventName y esten asociados a value
    else {
        for (auto evt : inputMap[eventName][value]) {
            /*if (evt == "ev_REMOVE_LAST_CHAR") removeChar = true;
            else if (evt == "ev_TOGGLE_TEXT_INPUT") toggleTextInput = true;
            else*/ Game::instance()->pushEvent(evt, {});
        }
    }
}

void InputManager::updateState(const SDL_Event& event) {
    std::string eventName = "";
    int value = -1;

    // Eventos de input
    switch (event.type) {
    // Ventana
    case SDL_WINDOWEVENT_CLOSE:
    case SDL_QUIT:
        Game::instance()->exit();
        //Game::instance()->pushEvent("ev_QUIT", {});
        break;
    case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) Game::instance()->pushEvent("ev_RESIZEWINDOW", {});
        break;

    // Teclado
    case SDL_KEYDOWN:
        eventName = "ie_keyDown";
        value = event.key.keysym.sym;
        break;
    case SDL_KEYUP:
        eventName = "ie_keyUp";
        value = event.key.keysym.sym;
        break;

    // Raton
    case SDL_MOUSEMOTION:
        eventName = "ie_mouseMoving";
        mousePos.first = event.motion.x;
        mousePos.second = event.motion.y;
        value = MOUSE_MOTION_VALUE;
        break;
    case SDL_MOUSEBUTTONDOWN:
        eventName = "ie_mouseButtonDown";
        value = event.button.button;
        break;
    case SDL_MOUSEBUTTONUP:
        eventName = "ie_mouseButtonUp";
        value = event.button.button;
        break;
    case SDL_MOUSEWHEEL:
        eventName = "ie_mouseWheel";
        value = MOUSE_WHEEL_VALUE;
        break;

    // Mando
    case SDL_CONTROLLERAXISMOTION:
        eventName = "ie_ctrlAxisMotion";
        value = event.caxis.axis;
        break;
    case SDL_CONTROLLERBUTTONDOWN:
        eventName = "ie_ctrlButtonDown";
        value = event.cbutton.button;
        break;
    case SDL_CONTROLLERBUTTONUP:
        eventName = "ie_ctrlButtonUp";
        value = event.cbutton.button;
        break;
    case SDL_JOYDEVICEADDED: addController(event.cdevice.which); break;
    case SDL_JOYDEVICEREMOVED: removeController(event.cdevice.which); break;

    //// Input de texto
    //case SDL_TEXTINPUT: inputText += event.text.text; break;
    //case SDL_TEXTEDITING:
    //    compositionText = event.edit.text;
    //    cursor = event.edit.start;
    //    selectionLen = event.edit.length;
    //    break;

    default: break;
    }


//    // Si esta activa la entrada de texto
//    if (SDL_IsTextInputActive()) {
//        // Borra el ultimo char
//        if (removeChar) {
//            removeLastChar();
//            removeChar = false;
//        }
//        // Desactiva la entrada de texto y limpia el texto introducido
//        else if (toggleTextInput) {
//            toggleTextInput = false;
//            SDL_StopTextInput();
//            resetText();
//            sendEvents();
//#ifdef _DEBUG
//            std::cout << "Entrada de texto finalizada\n";
//#endif
//        }
//    }

    sendEvent(eventName, event, value);
}

void InputManager::sendEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        updateState(event);
    }

}
}
