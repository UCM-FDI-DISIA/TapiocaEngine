#include "Component.h"
#include <string>
#include "GameObject.h"
#include "checkML.h"

namespace Tapioca {
Component::Component() : object(nullptr), alive(true), active(true) { }

void Component::pushEvent(std::string const& id, void* info, const bool global, const bool delay) {
	object->pushEvent(id, info, global, delay);
}
}
