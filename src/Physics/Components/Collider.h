#pragma once
#include "../../Core/Structure/Component.h"
#include "../Core/Utilities/Vector3.h"
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