#include "ComponentFactory.h"
#include <iostream>

namespace Tapioca {
ComponentFactory::ComponentFactory() { }

ComponentFactory ::~ComponentFactory() { }

Component* ComponentFactory::getComponent(std::string s) { return new Component(); } //por implementar
}
