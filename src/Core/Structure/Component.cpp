#include "Component.h"
#include "GameObject.h"
#include "Game.h"
#include <string>

namespace Tapioca {
Component::Component(GameObject* parent, std::string id, bool active) : parent(parent), alive(true), active(active),
    deltaTime(Game::get()->getDeltaTime()), fixedDeltaTime(Game::get()->getFixedDeltaTime()) {
    parent->addComponent(this, id);
}
}
