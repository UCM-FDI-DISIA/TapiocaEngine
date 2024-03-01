#include "EjemploComponent.h"

Tapioca::EjemploComponent::EjemploComponent()
    : Tapioca::Component()
    , a(0)
    , b(0)
    , c(0)
    , d(0)
    , name("") { }

void Tapioca::EjemploComponent::initComponent(const CompMap& map) {
    
    setValueFromMap(a, map);
    setValueFromMap(b, map);
    setValueFromMap(c, map);
    setValueFromMap(d, map);
    setValueFromMap(name, map);
    
}

// void Tapioca::EjemploComponent::update(const uint64_t deltaTime) { }
// void Tapioca::EjemploComponent::handleEvents() { }
// void Tapioca::EjemploComponent::fixedUpdate() { }