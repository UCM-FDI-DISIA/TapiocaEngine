#pragma once
#include "Utilities/defs.h"

namespace Tapioca {
class Component;

class TAPIOCA_API ComponentBuilder {
public:
    virtual Component* createComponent() = 0;
};
}

