#pragma once
#include "Structure/Component.h"

namespace Tapioca {
class GameObject;

class Collider : public Component {
public:
    Collider();
    ~Collider() {};
    void initComponent(const CompMap& variables) override {};

    void onCollisionEnter(GameObject* other);
    void onCollisionExit(GameObject* other);
    void onCollisionStay(GameObject* other);

};
}