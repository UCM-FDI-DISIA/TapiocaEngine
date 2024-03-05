#include "Collider.h"
#include "Structure/GameObject.h"

#ifdef _DEBUG
#include <iostream>
#endif

namespace Tapioca {

Collider::Collider() : Component() { }

void Collider::onCollisionEnter(GameObject* other) {
#ifdef _DEBUG
    std::cout << "Entro en collision\n";
#endif
    pushEvent("Collider", "onCollisionEnter", false);
}

void Collider::onCollisionExit(GameObject* other) {
#ifdef _DEBUG
    std::cout << "Salgo de collision\n";
#endif
    pushEvent("Collider", "onCollisionExit", false);
}

void Collider::onCollisionStay(GameObject* other) {
#ifdef _DEBUG
    std::cout << "Estoy en collision\n";
#endif
    pushEvent("Collider", "onCollisionStay", false);
}

}