#include "WindowManager.h"
#include "componentDefs.h"
#include "Structure/DynamicLibraryLoader.h"
#include "Structure/MainLoop.h"

#include <SDL.h>
#include <SDL_syswm.h>
#undef main


#ifdef _DEBUG
#include <iostream>
#endif
#include "checkML.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<WindowManager>;
template<>
WindowManager* Singleton<WindowManager>::instance_ = nullptr;

WindowManager::WindowManager(std::string const& windowName, const uint32_t w, const uint32_t h)
    : sdlWindow(nullptr), glContext(nullptr), windowName(windowName), windowWidth(w), windowHeight(h),
      firstWindowWidth(w), firstWindowHeight(h), resized(true), modules(), mainLoop(nullptr) { }

WindowManager::~WindowManager() {
    // eliminar la ventana de sdl
    if (sdlWindow != nullptr) {
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();
        sdlWindow = nullptr;
    }
}

bool WindowManager::init() {
    // Iniciar SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) return false;

    // Crear ventana
    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
                                 windowHeight, flags);
    if (sdlWindow == nullptr) {
        logError(("WindowManager: Error al crear la ventana de SDL: " + std::string(SDL_GetError()) + '.').c_str());
        return false;
    }
    logInfo("WindowManager: Cargada la ventana de SDL.");

    mainLoop = MainLoop::instance();

    return true;
}

bool WindowManager::initConfig() {
    logInfo("WindowManager: Configurando el nombre de la ventana...");

    EntryPointGetWindowName wn = (EntryPointGetWindowName)GetProcAddress(DynamicLibraryLoader::module, "getWindowName");
    if (wn == nullptr) {
        logError("WindowManager: La DLL del juego no tiene la funcion \"getWindowName\".");
        return false;
    }
    setWindowName(wn());
    logInfo(("WindowManager: Nombre de la ventana configurado a \"" + windowName + "\".").c_str());
    return true;
}

void WindowManager::setWindowName(std::string const& name) {
    SDL_SetWindowTitle(sdlWindow, name.c_str());
    windowName = name;
}

void WindowManager::subscribeModule(WindowModule* mod) { modules.push_back(mod); }

void WindowManager::sendEvent(std::string const& id, void* info) {
    if (id == "ev_CLOSE") mainLoop->exit();
    else
        mainLoop->pushEvent({nullptr, id, info,true});
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
}