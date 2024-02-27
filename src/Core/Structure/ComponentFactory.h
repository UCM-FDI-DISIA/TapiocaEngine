#pragma once
#include "../Structure/Factory.h"
#include "../Structure/Component.h"
#include <Windows.h>
typedef void(__cdecl* EntryPoint)(const char*);

namespace Tapioca {
class ComponentFactory : public Factory {
private:
    HMODULE module;
    EntryPoint entryPoint;

    ComponentFactory();

public:
    ~ComponentFactory();
    virtual Component* getComponent(const std::string& s) = 0;
};
}