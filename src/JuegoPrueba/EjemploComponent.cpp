#include "EjemploComponent.h"
using namespace Tapioca;
using namespace TapiocaGame;

EjemploComponent::EjemploComponent()
    : Component()
    , a(0)
    , b(0)
    , c(0)
    , d("") { }

void EjemploComponent::initComponent(const CompMap& map) {
    setValueFromMap(a, "a", map);
    setValueFromMap(b, "b", map);
    setValueFromMap(c, "c", map);
    setValueFromMap(d, "d", map);
}

// void EjemploComponent::update(const uint64_t deltaTime) { }
// void EjemploComponent::handleEvents() { }
// void EjemploComponent::fixedUpdate() { }