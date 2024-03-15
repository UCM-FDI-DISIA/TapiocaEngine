#pragma once
#include <Structure/Module.h>
union SDL_Event;

namespace Tapioca {

class TAPIOCA_API WindowModule : public Module {
public:
    virtual bool handleEvents(const SDL_Event& event) { return true; }
};

}