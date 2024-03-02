#pragma once
#include "Structure/Component.h"
#include "Structure/ComponentBuilder.h"
#include "EjemploComponent.h"
#include "defs.h"
using namespace Tapioca;

namespace TapiocaGame {
class EjemploComponentFactory : public ComponentBuilder {
public:
    Component* createComponent() override;
};
}