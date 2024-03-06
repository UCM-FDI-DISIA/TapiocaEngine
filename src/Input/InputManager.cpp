#include "InputManager.h"

#include <SDL.h>
#include <imgui_impl_sdl2.h>      // Para gestionar los eventos de la interfaz
#include "Structure/Game.h"

namespace Tapioca {

InputManager::InputManager() 
    : inputText(""), compositionText(nullptr), cursor(0), selectionLen(0), removeChar(false), toggleTextInput(false) { }

bool InputManager::init() { 
    if (!SDL_WasInit(SDL_INIT_EVERYTHING)) SDL_Init(SDL_INIT_EVERYTHING);
    resetText();
    initControllers();
    clearInput();
    return true;
}

InputManager::~InputManager() {
    clearInput();
    for (auto ctrl : controllers) removeController(ctrl.first);
    controllers.clear();
}


void InputManager::mapInput(std::string const& evt, std::string const& src, int const& ctrl) {
    int control = ctrl;
    if (src == "ie_mouseMoving") control = 0;
    iMap[src][control].push_back(evt);
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


void InputManager::clearInput() {
    for (auto& e : inputEventTriggered) e.second.clear();
}

void Tapioca::InputManager::sendEvent(std::string const& event, SDL_Event const& eventInfo) { 
    // Si el origen del evento no ha sido mapeado, lo ignora
    if (iMap.find(event) == iMap.end()) return;

    // Asigna el valor de la tecla/boton/etc que haya generado el evento
    int value = -1;
    if (event == "ie_keyDown" || event == "ie_keyUp") value = eventInfo.key.keysym.sym;
    else if (event == "ie_mouseMoving") value = 0;
    else if (event == "ie_mouseButtonUp" || event == "ie_mouseButtonDown") value = eventInfo.button.button;
    else if (event == "ie_ctrlAxisMotion") value = eventInfo.caxis.axis;
    else if (event == "ie_ctrlButtonUp" || event == "ie_ctrlButtonDown") value = eventInfo.cbutton.button;

    // Si no ha sido mapeado, lo ignora
    if (iMap[event].find(value) == iMap[event].end()) return;

    // Si ha sido mapeado, envia todos los eventos que se originen por event asociados al control value
    for (auto evt : iMap[event][value]) {
        if (evt == "ev_REMOVE_LAST_CHAR") removeChar = true;
        else if (evt == "ev_TOGGLE_TEXT_INPUT") toggleTextInput = true;
        else Game::get()->pushEvent(evt, {});
    }

}

void InputManager::updateState(const SDL_Event& event) {
    std::string eventName = "";

    // Eventos de input
    switch (event.type) {
    // Ventana
    case SDL_WINDOWEVENT_CLOSE: case SDL_QUIT:
        Game::get()->exit();
        break;
    case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) Game::get()->pushEvent("ev_RESIZEWINDOW", {});
        break;
    
    // Teclado
    case SDL_KEYDOWN:
        eventName = "ie_keyDown";
        inputEventTriggered["ie_keyDown"].push_back(event);
        break;
    case SDL_KEYUP:
        eventName = "ie_keyUp";
        inputEventTriggered["ie_keyUp"].push_back(event);
        break;

    // Raton
    case SDL_MOUSEMOTION:
        eventName = "ie_mouseMoving";
        inputEventTriggered["ie_mouseMoving"].push_back(event);
        mousePos.first = event.motion.x;
        mousePos.second = event.motion.y;
        break;
    case SDL_MOUSEBUTTONDOWN:
        eventName = "ie_mouseButtonDown";
        inputEventTriggered["ie_mouseButtonDown"].push_back(event);
        break;
    case SDL_MOUSEBUTTONUP:
        eventName = "ie_mouseButtonUp";
        inputEventTriggered["ie_mouseButtonUp"].push_back(event);
        break;
    case SDL_MOUSEWHEEL:
        eventName = "ie_mouseWheel";
        inputEventTriggered["ie_mouseWheel"].push_back(event);
        break;

    // Mando
    case SDL_CONTROLLERAXISMOTION:
        eventName = "ie_ctrlAxisMotion";
        inputEventTriggered["ie_ctrlAxisMotion"].push_back(event);
        break;
    case SDL_CONTROLLERBUTTONDOWN:
        eventName = "ie_ctrlButtonDown";
        inputEventTriggered["ie_ctrlButtonDown"].push_back(event);
        break;
    case SDL_CONTROLLERBUTTONUP:
        eventName = "ie_ctrlButtonUp";
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
        if (removeChar) {
            removeLastChar();
            removeChar = false;
        }
        // Desactiva la entrada de texto y limpia el texto introducido
        else if (toggleTextInput) {
            toggleTextInput = false;
            SDL_StopTextInput();
            resetText();
            handleEvents();
#ifdef _DEBUG
            std::cout << "Entrada de texto finalizada\n";
#endif
        }
    }


    sendEvent(eventName, event);
}

void InputManager::handleEvents() {
    SDL_Event event;
    clearInput();
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        updateState(event);
    }
}


}
