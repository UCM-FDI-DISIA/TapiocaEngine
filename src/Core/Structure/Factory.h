#pragma once
#include "../Structure/Component.h"

namespace Tapioca {
class Factory {
public:
    virtual Component* createComponent() = 0;
};
}
