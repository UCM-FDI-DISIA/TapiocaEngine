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
    std::string e = "onCollisionEnter";
    pushEvent("Collider", &e, false);
}

void Collider::onCollisionExit(GameObject* other) {
#ifdef _DEBUG
    std::cout << "Salgo de collision\n";
#endif
    std::string e = "onCollisionExit";
    pushEvent("Collider", &e, false);
}

void Collider::onCollisionStay(GameObject* other) {
#ifdef _DEBUG
    std::cout << "Estoy en collision\n";
#endif
    std::string e = "onCollisionStay";
    pushEvent("Collider", &e, false);
}

}