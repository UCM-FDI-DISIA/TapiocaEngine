#include "WindowManager.h"

#include <Structure/Game.h>
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
    : sdlWindow(nullptr), glContext(nullptr), windowWidth(w), windowHeight(h), modules(), game(nullptr) {
}

bool WindowManager::init() {
    // Iniciar SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) return false;

    // Crear ventana
    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
                                 windowHeight, flags);
    if (sdlWindow == nullptr) {
#ifdef _DEBUG
        std::cerr << "Error al crear la ventana de SDL: " << SDL_GetError() << '\n';
#endif
        return false;
    }

    game = Game::instance();

    return true;
}

WindowManager::~WindowManager() { 
	// eliminar la ventana de sdl
    if (sdlWindow != nullptr) {
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();
        sdlWindow = nullptr;
    }


}

void WindowManager::update(const uint64_t deltaTime) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_WINDOWEVENT_CLOSE || event.type == SDL_QUIT) game->exit();
        else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_GetWindowSize(sdlWindow, (int*)(&windowWidth), (int*)(&windowHeight));
            }
        }

        for (auto mod : modules) mod->handleEvents(event);
    }
}

void WindowManager::subscribeModule(WindowModule* mod) { modules.push_back(mod); }

void WindowManager::sendEvent(std::string const& id, void* info) { game->pushEvent(id, info); }



}