
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#include <Ogre.h>
#include <OgreGL3PlusRenderSystem.h>
#include <SDL.h>
#undef main   //sdl lo define aparentemente
#include <SDL_syswm.h>
#include "InputManager.h"
std::string windowName = "motor";
Ogre::RenderWindow* window;
int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);   // Check Memory Leaks
    SDL_Init(SDL_INIT_EVERYTHING);
    Uint32 flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    SDL_Window* native = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720, 480, flags);
    
    
    InputManager* ih = InputManager::init();

    bool done = false;
    while (!done && !ih->windowClosed()) {
        try {
            ih->refresh();
        } catch (Ogre::Exception& e) {

            // std::cout << e.getFullDescription() << '\n';
        };
    }

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
    std::cout << "final";
    return 0;
}
