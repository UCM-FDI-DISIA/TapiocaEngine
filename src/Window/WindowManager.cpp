#include "WindowManager.h"

#include <SDL.h>
#include <SDL_syswm.h>
#undef main
#include <iostream>

#include "checkML.h"

namespace Tapioca {

WindowManager::WindowManager(const std::string const& windowName, const uint32_t w, const uint32_t h)
    : sdlWindow(nullptr), windowWidth(w), windowHeight(h) { }

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
}

WindowManager::~WindowManager() { 
	// eliminar la ventana de sdl
    if (sdlWindow != nullptr) {
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();
        sdlWindow = nullptr;
    }


}


}