#pragma once
#include "Structure/Component.h"
#include "Structure/ComponentFactory.h"
#include "EjemploComponent.h"

namespace Tapioca {
class EjemploComponentFactory : public ComponentFactory {
public:
    Component* createComponent() override;
};
}