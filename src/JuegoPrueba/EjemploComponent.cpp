#include "EjemploComponent.h"

namespace TapiocaGame {

EjemploComponent::EjemploComponent() : Tapioca::Component(), a(0), b(0), c(0), d("") { }

bool EjemploComponent::initComponent(const CompMap& variables) {
    return setValueFromMap(a, "a", variables) && 
           setValueFromMap(b, "b", variables) && 
           setValueFromMap(c, "c", variables) &&
           setValueFromMap(d, "d", variables);
}

// void EjemploComponent::update(const uint64_t deltaTime) { }
// void EjemploComponent::handleEvents() { }
// void EjemploComponent::fixedUpdate() { }

}
