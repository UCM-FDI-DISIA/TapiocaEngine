#include "RigidBody.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "../../Physics/PhysicsManager.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "Collider.h"


namespace Tapioca {
RigidBody::RigidBody()
    : transform(nullptr)
    , collider(nullptr)
    , rigidBody(nullptr)
    , mass(0)
    , isTrigger(false)
    , mask(-1)
    , group(1)
    , friction(0)
    , colShape(BOX_SHAPE)
    , movementType(STATIC_OBJECT)
    , bounciness(0) { }

RigidBody::~RigidBody() {
    if (rigidBody != nullptr) {
        PhysicsManager::instance()->destroyRigidBody(rigidBody);
        rigidBody = nullptr;
    }
}

void RigidBody::initComponent() {

    //transform = parent->getComponent<Transform>();// DESCOMENTAR

    rigidBody = PhysicsManager::instance()->createRigidBody(transform->getPosition(), transform->getRotation(),
        transform->getScale(), colShape, movementType, mass, friction, bounciness, isTrigger, group, mask);

    //collider = parent->getComponent<Collider>();//DESCOMENTAR

    rigidBody->setUserPointer(collider);
}

void RigidBody::update() {

    if (movementType == KINEMATIC_OBJECT) {

        btTransform btTr = rigidBody->getWorldTransform();
        btTr.setOrigin(toBtVector3(transform->getPosition()));
        btTr.setRotation(toBtQuaternion(transform->getRotation()));
        rigidBody->setWorldTransform(btTr);
    }

    else if (movementType == DYNAMIC_OBJECT) {
        transform->setPosition(toVector3(rigidBody->getWorldTransform().getOrigin()));
        transform->setRotation(toEuler(rigidBody->getWorldTransform().getRotation()));
        // btVector3 vel = rigidBody->getLinearVelocity();
        //transform->setVelocity(rigidBody->getLinearVelocity());
    }
}

void RigidBody::setActive(bool b) {
    Component::setActive(b);
    rigidBody->activate(b);
}

void RigidBody::setMomeventType(MovementType t) {
    if (rigidBody == nullptr) return;
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() & ~movementType);
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | t);
    movementType = t;
}

void RigidBody::setTrigger(bool t) {
    isTrigger = t;
    if (isTrigger) {
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    } else
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void RigidBody::setColliderShape(ColliderShape s) { colShape = s; }

void RigidBody::setColliderScale(Vector3 s) {
    colliderScale = s;
    if (rigidBody == nullptr) return;
    rigidBody->getCollisionShape()->setLocalScaling(toBtVector3(s));
}


void RigidBody::setMass(float m) { mass = m; }

void RigidBody::setFriction(float f) {
    friction = f;
    if (rigidBody == nullptr) return;
    rigidBody->setFriction(f);
}

void RigidBody::setBounciness(float b) {
    rigidBody->setRestitution(b);
    if (rigidBody == nullptr) return;
    bounciness = b;
}

void RigidBody::setVelocity(Vector3 v) {
    //rigidBody->activate(true);
    if (rigidBody == nullptr) return;
    rigidBody->setLinearVelocity(toBtVector3(v));
}

void RigidBody::setGravity(Vector3 g) {
    if (rigidBody == nullptr) return;
    rigidBody->setGravity(toBtVector3(g));
}

void RigidBody::addForce(Vector3 f) {
    if (rigidBody == nullptr) return;
    rigidBody->applyCentralForce(toBtVector3(f));
}

void RigidBody::addImpulse(Vector3 f) {
    if (rigidBody == nullptr) return;
    rigidBody->applyCentralImpulse(toBtVector3(f));
}

void RigidBody::setMask(int m) {
    mask = m;
    if (rigidBody == nullptr) return;
    btBroadphaseProxy* bdProxy = rigidBody->getBroadphaseProxy();
    bdProxy->m_collisionFilterMask = m;
}

void RigidBody::setGroup(int g) {
    group = g;
    if (rigidBody == nullptr) return;
    btBroadphaseProxy* bdProxy = rigidBody->getBroadphaseProxy();
    bdProxy->m_collisionFilterGroup = g;
}


int RigidBody::getMovementType() const { return movementType; }

bool RigidBody::getTrigger() const { return isTrigger; }

int RigidBody::getColliderShape() const { return colShape; }

Vector3 RigidBody::getColliderScale() const { return colliderScale; }


float RigidBody::getMass() const { return mass; }

float RigidBody::getFriction() const { return friction; }

float RigidBody::getBounciness() const { return bounciness; }

Vector3 RigidBody::getVelocity() const { return toVector3(rigidBody->getLinearVelocity()); }

Vector3 RigidBody::getGravity() const { return toVector3(rigidBody->getGravity()); }


int RigidBody::getMask() const { return mask; }

int RigidBody::getGroup() const { return group; }


}