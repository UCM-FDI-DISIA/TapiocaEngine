#include "Collider.h"
#include <iostream>
#include "Structure/GameObject.h"
namespace Tapioca {

Collider::Collider()
    : Component() { }

void Collider::onCollisionEnter(GameObject* other) {
#ifdef _DEBUG
    std::cout << "Entro en collision" << std::endl;
#endif
    parent->onCollisionEnter(other);
}

void Collider::onCollisionExit(GameObject* other) {
#ifdef _DEBUG
    std::cout << "Salgo de collision" << std::endl;
#endif
    parent->onCollisionExit(other);
}

void Collider::onCollisionStay(GameObject* other) {
#ifdef _DEBUG
    std::cout << "Estoy en collision" << std::endl;
#endif
    parent->onCollisionStay(other);
}

}