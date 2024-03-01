#pragma once
#include "Structure/Module.h"
#include "Utilities/Singleton.h"
class SDL_Window;

namespace Tapioca {
class UIManager : public Singleton<UIManager>, public Module {
    SDL_Window* myWindow;

public:
    UIManager(SDL_Window* window);

    bool init() override;
};
}