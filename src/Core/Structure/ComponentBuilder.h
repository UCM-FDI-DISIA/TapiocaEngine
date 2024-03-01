#pragma once
#include "../Structure/Component.h"

namespace Tapioca {
class ComponentBuilder {
public:
    virtual Component* createComponent() = 0;
};
}

