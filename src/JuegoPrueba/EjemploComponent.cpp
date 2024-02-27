#include "EjemploComponent.h"

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