#pragma once
#include "Structure/Component.h"

namespace Tapioca {
class GameObject;

class Collider : public Component {
public:
    COMPONENT_ID("Collider");
    Collider();
    ~Collider() {};

    bool initComponent(const CompMap& variables) override { return true; };

    void onCollisionEnter(GameObject* other);
    void onCollisionExit(GameObject* other);
    void onCollisionStay(GameObject* other);
};
}