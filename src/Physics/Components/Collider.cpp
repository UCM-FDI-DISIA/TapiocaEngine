#include "Collider.h"
#include "Structure/GameObject.h"

#include <iostream>

namespace Tapioca {

Collider::Collider() : Component() { }

void Collider::onCollisionEnter(GameObject* other) {
#ifdef _DEBUG
    std::cout << "Entro en collision\n";
#endif
    parent->onCollisionEnter(other);
}

void Collider::onCollisionExit(GameObject* other) {
#ifdef _DEBUG
    std::cout << "Salgo de collision\n";
#endif
    parent->onCollisionExit(other);
}

void Collider::onCollisionStay(GameObject* other) {
#ifdef _DEBUG
    std::cout << "Estoy en collision\n";
#endif
    parent->onCollisionStay(other);
}

}