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
    float bounciness;


public:
    COMPONENT_ID("RigidBody");
    RigidBody();
    ~RigidBody();

    bool initComponent(const CompMap& variables) override;
    void update(const uint64_t deltaTime) override;
    void start() override;

    void setActive(bool b) override;

    void setMomeventType(MovementType t);
    void setTrigger(bool t);
    void setColliderScale(Vector3 s);
    void setColliderShape(ColliderShape s);
    void setMask(int m);
    void setGroup(int g);

    void setMass(float m);
    void setFriction(float f);
    void setBounciness(float b);
    void setVelocity(Vector3 v);
    void setGravity(Vector3 g);

    //fuerza gradual
    void addForce(Vector3 f);
    //aplica solo una fuerza instantanea (golpes,explosiones)
    void addImpulse(Vector3 f);


    bool getTrigger() const;
    int getColliderShape() const;
    Vector3 getColliderScale() const;
    int getMovementType() const;
    int getMask() const;
    int getGroup() const;

    float getMass() const;
    float getFriction() const;
    float getBounciness() const;
    Vector3 getVelocity() const;
    Vector3 getGravity() const;
};


}