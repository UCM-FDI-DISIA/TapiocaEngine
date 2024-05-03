#include "Component.h"
#include "GameObject.h"
#include "MainLoop.h"
#include <string>

namespace Tapioca {
Component::Component() : object(nullptr), alive(true), active(true) { }

void Component::pushEvent(std::string const& id, void* info, const bool global, const bool delay) {
	object->pushEvent(id, info, global,delay);
}
}
