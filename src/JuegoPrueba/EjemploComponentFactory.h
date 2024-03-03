#pragma once
#include "Structure/ComponentBuilder.h"
#include "Structure/Component.h"

namespace TapiocaGame {
class EjemploComponentFactory : public Tapioca::ComponentBuilder {
public:
    Tapioca::Component* createComponent() override;
};
}