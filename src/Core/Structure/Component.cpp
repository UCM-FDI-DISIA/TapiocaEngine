#include "Component.h"
#include "GameObject.h"
#include "Game.h"
#include <string>

namespace Tapioca {
Component::Component() : parent(nullptr), alive(true), active(true), id(""),
    deltaTime(Game::get()->getDeltaTime()), fixedDeltaTime(Game::get()->getFixedDeltaTime()) {
    
}
}
