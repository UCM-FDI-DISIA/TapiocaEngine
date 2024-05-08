#include "WindowManager.h"
#include "componentDefs.h"
#include "Structure/DynamicLibraryLoader.h"
#include "Structure/MainLoop.h"
#include <SDL.h>
#include <SDL_syswm.h>
#undef main
#include "checkML.h"

namespace Tapioca {
WindowManager::WindowManager(std::string const& windowName, const uint32_t w, const uint32_t h)
    : windowName(windowName), windowWidth(w), windowHeight(h), firstWindowWidth(windowWidth),
      firstWindowHeight(windowHeight), sdlWindow(nullptr), glContext(nullptr), resized(false), mainLoop(nullptr) { }

WindowManager::~WindowManager() {
    if (sdlWindow != nullptr) {
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();
        sdlWindow = nullptr;
    }
    glContext = nullptr;
    modules.clear();
    mainLoop = nullptr;
}

bool WindowManager::init() { return SDL_Init(SDL_INIT_EVERYTHING) == 0; }

bool WindowManager::initConfig() {
    mainLoop = MainLoop::instance();
    DynamicLibraryLoader* loader = mainLoop->getLoader();
    if (loader == nullptr) {
        logError("SceneLoader: Instancia de DynamicLibraryLoader invalida.");
        return false;
    }
    HMODULE module = loader->getModule();

    logInfo("WindowManager: Configurando el nombre de la ventana...");

    EntryPointGetWindowName wn = (EntryPointGetWindowName)GetProcAddress(module, "getWindowName");
    if (wn == nullptr) {
        logError("WindowManager: La DLL del juego no tiene la funcion \"getWindowName\".");
        return false;
    }
    setWindowName(wn());
    logInfo(("WindowManager: Nombre de la ventana configurado a \"" + windowName + "\".").c_str());

    EntryPointGetFullScreen fs = (EntryPointGetFullScreen)GetProcAddress(module, "getFullScreen");
    // Si no se ha encontrado la funcion de pantalla completa, se intenta obtener el tamano de la ventana y si no, se inicializa a los valores predefinidos
    if (fs == nullptr) {
        logWarn("WindowManager: La DLL del juego no tiene la funcion \"getFullScreen\". Se preguntara por el tamano de "
                "la ventana.");
        tryGetWindowSize();
    }
    // Si existe la funcion de pantalla completa, se comprueba si se quiere pantalla completa o no
    else if (fs()) {
        logInfo("WindowManager: Configurando la ventana a pantalla completa...");
        SDL_DisplayMode displayMode;
        if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
            logError(("WindowManager Error al conseguir los valores de pantalla completa: " +
                      std::string(SDL_GetError()) + '.')
                         .c_str());
            return false;
        }
        windowWidth = firstWindowWidth = displayMode.w;
        windowHeight = firstWindowHeight = displayMode.h;
        logInfo("WindowManager: Ventana configurada a pantalla completa.");
    }
    // Si no se quiere pantalla completa, se intenta obtener el tamano de la ventana y si no, se inicializa a los valores predefinidos
    else tryGetWindowSize();

    // Crear ventana
    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
                                 windowHeight, flags);
    if (sdlWindow == nullptr) {
        logError(("WindowManager: Error al crear la ventana de SDL: " + std::string(SDL_GetError()) + '.').c_str());
        return false;
    }
    logInfo("WindowManager: Cargada la ventana de SDL.");

    return true;
}

void WindowManager::update(const uint64_t deltaTime) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Si es un evento de cerrar la venetana, llama a que se cierre el juego
        if (event.type == SDL_WINDOWEVENT_CLOSE || event.type == SDL_QUIT) mainLoop->exit();

        // Si es un evento de redimensionar la ventana, actualiza sus dimensiones
        else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_GetWindowSize(sdlWindow, (int*)(&windowWidth), (int*)(&windowHeight));
                resized = false;
            }
        }

        // Gestiona los eventos de modulos si no se ha producido un evento de UI
        // Se asume que la UI va antes que el input y que estos 2 modulos son los ultimos del
        // vector para que el resto de eventos de ventana no se ignoren por los eventos de UI
        bool UIEvent = false;
        for (int i = 0; i < modules.size() && !UIEvent; i++)
            UIEvent = modules[i]->handleEvents(event);
    }
}

void WindowManager::subscribeModule(WindowModule* mod) { modules.push_back(mod); }

void WindowManager::sendEvent(std::string const& id, void* info) {
    if (id == "ev_CLOSE") mainLoop->exit();
    else mainLoop->pushEvent({nullptr, id, info, true});
}

void WindowManager::tryGetWindowSize() {
    DynamicLibraryLoader* loader = mainLoop->getLoader();
    if (loader == nullptr) return;

    logInfo("WindowManager: Configurando la ventana a modo ventana...");
    EntryPointGetWindowSize ws = (EntryPointGetWindowSize)GetProcAddress(loader->getModule(), "getWindowSize");
    if (ws == nullptr) {
        logError(("WindowManager: La DLL del juego no tiene la funcion \"getWindowSize\". Se usara el tamano "
                  "predefinido: " +
                  std::to_string(windowWidth) + "x" + std::to_string(windowHeight) + '.')
                     .c_str());
    }
    else {
        ws(windowWidth, windowHeight);
        firstWindowWidth = windowWidth;
        firstWindowHeight = windowHeight;
    }
    logInfo(("WindowManager: Ventana configurada a " + std::to_string(windowWidth) + "x" + std::to_string(windowHeight))
                .c_str());
}

void WindowManager::setWindowName(std::string const& name) {
    SDL_SetWindowTitle(sdlWindow, name.c_str());
    windowName = name;
}
}
