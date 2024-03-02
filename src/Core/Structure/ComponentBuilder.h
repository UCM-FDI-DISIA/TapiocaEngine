#pragma once

namespace Tapioca {
class Component;

class ComponentBuilder {
public:
    virtual Component* createComponent() = 0;
};
}

