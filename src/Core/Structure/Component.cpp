#include "Component.h"
#include "GameObject.h"
#include "Game.h"
#include <string>

namespace Tapioca {

std::unordered_set<Component*> Component::listeners = std::unordered_set<Component*>();

Component::Component() : object(nullptr), alive(true), active(true), id("") { }

void Component::pushEvent(std::string const& id, void* info, bool global) {
	object->pushEvent(id, info, global);
}

}
