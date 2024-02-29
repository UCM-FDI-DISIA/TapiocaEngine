#pragma once
#include "../Structure/Component.h"
#include <Windows.h>
//typedef void(__cdecl* EntryPoint)(const char*);

namespace Tapioca {

class ComponentFactory {

public:
    virtual Component* createComponent() = 0;

   // Component* createComponent2() override{ return createComponent(); }
};

class TransformComponentFactory : public ComponentFactory {
public:
    Component* createComponent() override;
};
class ColliderComponentFactory : public ComponentFactory {
public:
    Component* createComponent() override;
};
class RigidBodyComponentFactory : public ComponentFactory {
public:
    Component* createComponent() override;
};
}

