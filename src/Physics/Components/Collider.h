#pragma once
#include "Structure/Component.h"

namespace Tapioca {
class ColliderBuilder;
class GameObject;

class TAPIOCA_API Collider : public Component {
    friend ColliderBuilder;

public:
    COMPONENT_ID("Collider");
    Collider();
    ~Collider() {};

    bool initComponent(const CompMap& variables) override { return true; };

    void onCollisionEnter(GameObject* const other);
    void onCollisionExit(GameObject* const other);
    void onCollisionStay(GameObject* const other);
};
}