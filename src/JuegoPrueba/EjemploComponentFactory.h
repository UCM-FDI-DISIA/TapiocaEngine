#pragma once
#include "Structure/Component.h"
#include "Structure/ComponentFactory.h"
#include "EjemploComponent.h"
#include "defs.h"
using namespace Tapioca;

namespace TapiocaGame {
class EjemploComponentFactory : public ComponentFactory {
public:
    Component* createComponent() override;
};
}