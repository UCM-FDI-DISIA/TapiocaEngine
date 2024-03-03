#pragma once
#include "Structure/ComponentBuilder.h"


namespace TapiocaGame {
class EjemploComponentFactory : public Tapioca::ComponentBuilder {
public:
    Tapioca::Component* createComponent() override;
};
}