#include "InputManager.h"
#include <SDL.h>
#include "WindowManager.h"
#include "checkML.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<InputManager>;
template<>
InputManager* Singleton<InputManager>::instance_ = nullptr;

InputManager::InputManager() : mousePos({0, 0}) { }

bool InputManager::init() {
    initControllers();
    return true;
}

InputManager::~InputManager() {
    for (std::pair<const int, SDL_GameController*> ctrl : controllers)
        removeController(ctrl.first);
    controllers.clear();
}

void InputManager::mapInput(std::string const& evt, std::string const& src, int const& ctrl) {
    int control = ctrl;
    if (src == "ie_mouseMoving") control = MOUSE_MOTION_VALUE;
    else if (src == "ie_mouseWheel")
        control = MOUSE_WHEEL_VALUE;
    inputMap[src][control].push_back(evt);
}

void InputManager::initControllers() {
    if (SDL_NumJoysticks() > 0) SDL_JoystickEventState(SDL_ENABLE);
    else
        logInfo("InputManager: No hay joysticks conectados.");
}

void InputManager::addController(const int i) {
    SDL_GameController* ctrl = SDL_GameControllerOpen(i);
    if (ctrl != nullptr) {
        controllers.insert({i, ctrl});
        deadZones.insert({i, DEFAULT_DEADZONE});
        logInfo(("InputManager: Mando conectado (" + std::string(SDL_GameControllerName(ctrl)) + ").").c_str());
    }
    else
        logError(("InputManager: No se pudo abrir mando, " + std::string(SDL_GetError()) + ".").c_str());
}

void InputManager::removeController(const int i) {
    SDL_GameController* ctrl = controllers.at(i);
    if (ctrl != nullptr) {
        deadZones.erase(i);
        SDL_GameControllerClose(ctrl);
        ctrl = nullptr;

        logInfo("InputManager: Mando desconectado.");
    }
}

void Tapioca::InputManager::sendEvent(std::string const& eventName, SDL_Event const& event, int const& value) {
    // Si el origen del evento no ha sido mapeado, lo ignora
    if (inputMap.find(eventName) == inputMap.end()) return;

    // Si no, si la tecla/boton/etc no ha sido mapeado, lo ignora
    else if (inputMap[eventName].find(value) == inputMap[eventName].end())
        return;
    // Si no, envia todos los eventos que origine eventName y esten asociados a value
    else {
        for (const auto& evt : inputMap[eventName][value])
            WindowManager::instance()->sendEvent(evt, {});
    }
}

bool InputManager::handleEvents(const SDL_Event& event) {
    std::string eventName = "";
    int value = -1;
    bool happened = true;

    switch (event.type) {
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

    default: happened = false; break;
    }

    sendEvent(eventName, event, value);

    return happened;
}
}
