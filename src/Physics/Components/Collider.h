#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"
namespace Tapioca {
class Vector3;
class GameObject;
class Collider : public Component {
public:
    Collider();
    ~Collider() {};

    void onCollisionEnter(GameObject* other);
    void onCollisionExit(GameObject* other);
    void onCollisionStay(GameObject* other);

};
}