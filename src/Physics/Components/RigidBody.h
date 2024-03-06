#pragma once

#pragma region includes
#include "Structure/Component.h"
#include "Utilities/Vector3.h"
#include "../Physics_def.h"

class btRigidBody;

#pragma endregion

namespace Tapioca {
//class RigidBodyBuilder;
class Vector3;
class Transform;
class Collider;

class TAPIOCA_API RigidBody : public Component {
    //friend RigidBodyBuilder;

private:
    btRigidBody* rigidBody;
    Transform* transform;
    Collider* collider;
    ColliderShape colShape;
    Vector3 colliderScale;
    MovementType movementType;
    bool isTrigger;
    //indica con que grupo se puede colisionar (en binario)
    int mask;
    //mi grupo/filtro de colision
    int group;

    float mass;
    float friction;
    float damping;
    float bounciness;


public:
    COMPONENT_ID("RigidBody");
    RigidBody();
    ~RigidBody();

    bool initComponent(const CompMap& variables) override;
    void update(const uint64_t deltaTime) override;
    void start() override;

    void setActive(const bool b) override;

    void setMomeventType(const MovementType t);
    void setTrigger(const bool t);
    void setColliderScale(const Vector3 s);
    void setColliderShape(const ColliderShape s);
    void setMask(const int m);
    void setGroup(const int g);

    void setMass(const float m);
    void setFriction(const float f);
    void setDamping(const float f);
    void setBounciness(const float b);
    void setVelocity(const Vector3 v);
    void setGravity(const Vector3 g);

    //fuerza gradual
    void addForce(const Vector3 f);
    //aplica solo una fuerza instantanea (golpes,explosiones)
    void addImpulse(const Vector3 f);


    bool getTrigger() const;
    int getColliderShape() const;
    Vector3 getColliderScale() const;
    int getMovementType() const;
    int getMask() const;
    int getGroup() const;

    float getMass() const;
    float getFriction() const;
    float getDamping() const;
    float getBounciness() const;
    Vector3 getVelocity() const;
    Vector3 getGravity() const;
};


}