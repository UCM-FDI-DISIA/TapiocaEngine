
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#include <SDL.h>
#undef main   //sdl lo define aparentemente

#include "InputManager.h"

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);   // Check Memory Leaks
    SDL_Init(SDL_INIT_EVERYTHING);
    Uint32 flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    SDL_Window* native = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720, 480, flags);
    
    
    Tapioca::InputManager* ih = Tapioca::InputManager::create();

    bool done = false;
    while (!done && !ih->windowClosed()) {
        if (ih->eventHappened(Tapioca::EVENTS::ev_ACCEPT)) std::cout << "event accept happened\n";
        ih->refresh();
    }

    SDL_Quit();
    

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
    std::cout << "final";
    return 0;
}
