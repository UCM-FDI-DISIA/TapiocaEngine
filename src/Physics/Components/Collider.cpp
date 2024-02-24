#include "Collider.h"
#include <iostream>
#include "../../Core/Structure/GameObject.h"
namespace Tapioca {

Collider::Collider()
    : Component() { }

void Collider::onCollisionEnter(GameObject* other) {
    std::cout << "Entro en collision" << std::endl;
    parent->onCollisionEnter(other);
}

void Collider::onCollisionExit(GameObject* other) {
    std::cout << "Salgo de collision" << std::endl;
    parent->onCollisionExit(other);
}

void Collider::onCollisionStay(GameObject* other) {
    std::cout << "Estoy en collision" << std::endl;
    parent->onCollisionStay(other);
}

}