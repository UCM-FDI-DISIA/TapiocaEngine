#include "Component.h"
#include "GameObject.h"
#include "Game.h"
#include <string>

namespace Tapioca {
    std::unordered_set<Component*> Component::listeners = std::unordered_set<Component*>();

    Component::Component() : parent(nullptr), alive(true), active(true), id(""),
        deltaTime(Game::get()->getDeltaTime()), fixedDeltaTime(Game::get()->getFixedDeltaTime()) {
    
    }
    void Component::addListener(Component* component) { listeners.insert(component); }
    void Component::removeListener(Component* component) { listeners.erase(component); }
    void Component::sendEvent(Component* component, messageType m) { for (auto l : listeners) l->receiveEvent(component, m); }
}
