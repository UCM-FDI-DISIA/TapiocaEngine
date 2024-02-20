#include "Component.h"
#include "GameObject.h"
#include "Scene.h"

namespace Tapioca {
Component::Component(GameObject* parent, std::string id, bool active) : parent(parent), alive(true), active(active), deltaTime(0x00) {
    parent->addComponent(this, id);
}

void Component::setActive(bool b) { active = b; }
}
