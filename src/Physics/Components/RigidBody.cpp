#include "RigidBody.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "PhysicsManager.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
//#include "Collider.h"


namespace Tapioca {
RigidBody::RigidBody()
    : transform(nullptr), rigidBody(nullptr), mass(0), isTrigger(false), mask(-1), group(1), friction(0),
      colShape(BOX_SHAPE), movementType(STATIC_OBJECT), damping(0), bounciness(0), colliderScale(Vector3(1)),
      activeRigidBody(true) { }

RigidBody::~RigidBody() {
    if (rigidBody != nullptr) {
        activeRigidBody = false;
        PhysicsManager::instance()->destroyRigidBody(rigidBody);
        rigidBody = nullptr;
    }
}

bool RigidBody::initComponent(const CompMap& variables) {

    int colShapeAux;
    if (!setValueFromMap(colShapeAux, "colShape", variables)) {
        logError("RigidBody: No se pudo inicializar colShape.");
        return false;
    }
    colShape = (ColliderShape)colShapeAux;

    bool colliderScaleSet = setValueFromMap(colliderScale.x, "colliderScaleX", variables) &&
        setValueFromMap(colliderScale.y, "colliderScaleY", variables) &&
        setValueFromMap(colliderScale.z, "colliderScaleZ", variables);
    if (!colliderScaleSet) {
        logError("RigidBody: No se pudo inicializar colliderScale.");
        return false;
    }

    if (!setValueFromMap(isTrigger, "isTrigger", variables)) {
        logError("RigidBody: No se pudo inicializar isTrigger.");
        return false;
    }

    int movementTypeAux;
    if (!setValueFromMap(movementTypeAux, "movementType", variables)) {
        logError("RigidBody: No se pudo inicializar movementType.");
        return false;
    }
    movementType = (MovementType)movementTypeAux;

    if (!setValueFromMap(mass, "mass", variables)) {
        logError("RigidBody: No se pudo inicializar mass.");
        return false;
    }

    if (!setValueFromMap(friction, "friction", variables)) {
        logError("RigidBody: No se pudo inicializar friction.");
        return false;
    }

    if (!setValueFromMap(damping, "damping", variables)) {
        logError("RigidBody: No se pudo inicializar damping.");
        return false;
    }

    if (!setValueFromMap(bounciness, "bounciness", variables)) {
        logError("RigidBody: No se pudo inicializar bounciness.");
        return false;
    }

    int bitmask;
    if (!setValueFromMap(bitmask, "mask", variables)) {
        logInfo("Rigidbody: Mask por defecto.");
    }
    else {
        for (int i = 0; i < std::to_string(bitmask).length(); ++i) {
            if (bitmask & (1 << i)) mask += (1 << i);
        }
    }


    if (!setValueFromMap(group, "group", variables)) {
        logInfo("Rigidbody: Group por defecto.");
    }

    return true;
}


void RigidBody::update(const uint64_t deltaTime) {
    /* if (movementType == KINEMATIC_OBJECT) {

        btTransform& btTr = rigidBody->getWorldTransform();
        btTr.setOrigin(toBtVector3(transform->getGlobalPosition()));
        btTr.setRotation(toBtQuaternion(transform->getGlobalRotation()));
        rigidBody->setWorldTransform(btTr);
    }*/
}
void RigidBody::fixedUpdate() {
    if (movementType == DYNAMIC_OBJECT) {
        transform->setPosition(toVector3(rigidBody->getWorldTransform().getOrigin()), true);
        transform->setRotation(toEuler(rigidBody->getWorldTransform().getRotation()), true);
    }
}
void RigidBody::handleEvent(std::string const& id, void* info) {
    if (id == "transformChanged") {
        bool b = *((bool*)info);
        if (!b) {
            if (movementType == DYNAMIC_OBJECT) {
                btTransform& btTr = rigidBody->getWorldTransform();
                btTr.setOrigin(toBtVector3(transform->getGlobalPosition()));

                Quaternion q = transform->getGlobalRotation();
                btQuaternion btQ = btQuaternion(q.vector.x, q.vector.y, q.vector.z, q.scalar);
                btTr.setRotation(btQ);
                //btTr.setRotation(toBtQuaternion(transform->getGlobalRotation()));
            }
            else if (movementType == KINEMATIC_OBJECT) {
                btTransform btTr;
                rigidBody->getMotionState()->getWorldTransform(btTr);
                btTr.setOrigin(toBtVector3(transform->getGlobalPosition()));

                Quaternion q = transform->getGlobalRotation();
                btQuaternion btQ = btQuaternion(q.vector.x, q.vector.y, q.vector.z, q.scalar);
                btTr.setRotation(btQ);
                //btTr.setRotation(toBtQuaternion(transform->getGlobalRotation()));
                rigidBody->getMotionState()->setWorldTransform(btTr);
            }
        }
    }
}
void RigidBody::onCollisionEnter(GameObject* const other) { pushEvent("onCollisionEnter", other, false); }

void RigidBody::onCollisionExit(GameObject* const other) { pushEvent("onCollisionExit", other, false); }

void RigidBody::onCollisionStay(GameObject* const other) { pushEvent("onCollisionStay", other, false); }

void RigidBody::awake() {

    transform = object->getComponent<Transform>();

    rigidBody = PhysicsManager::instance()->createRigidBody(
        transform->getGlobalPosition(), transform->getGlobalRotation(), colliderScale, colShape, movementType, mass,
        friction, damping, bounciness, isTrigger, group, mask);

    rigidBody->setUserPointer(this);
}

void RigidBody::setActive(const bool b) {
    Component::setActive(b);
    rigidBody->activate(b);
}

void RigidBody::setMomeventType(const MovementType t) {
    movementType = t;
    if (rigidBody == nullptr) return;
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() & ~movementType);
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | t);
    movementType = t;
}

void RigidBody::setTrigger(const bool t) {
    isTrigger = t;
    if (rigidBody == nullptr) return;
    if (isTrigger)
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    else
        rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

void RigidBody::setColliderShape(const ColliderShape s) { colShape = s; }

void RigidBody::setColliderScale(const Vector3 s) {
    colliderScale = s;
    if (rigidBody == nullptr) return;
    rigidBody->getCollisionShape()->setLocalScaling(toBtVector3(s));
}


void RigidBody::setMass(const float m) { mass = m; }

void RigidBody::setTensor(const Vector3 t) {

    PhysicsManager::instance()->removeRigidBody(rigidBody);
    btVector3 inertia;
    btVector3 tensor = toBtVector3(t);
   
    rigidBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
    rigidBody->setMassProps(mass, inertia * tensor);
    PhysicsManager::instance()->addRigidBody(rigidBody);
};


void RigidBody::addHingeConstraint(const Vector3 a) {

    /* btTransform frameInA(btQuaternion::getIdentity(), btVector3(0, 0, 0));   
    btTransform frameInB( btQuaternion::getIdentity(),btVector3(0, 0, 0));  
    btHingeConstraint* hingeConstraint = new btHingeConstraint(*rigidBody, frameInA,toBtVector3(a));
    hingeConstraint->setFrames(frameInA, frameInB);     
    PhysicsManager::instance()->getWorld()->addConstraint(hingeConstraint, true);  */
};


void RigidBody::setFriction(const float f) {
    friction = f;
    if (rigidBody == nullptr) return;
    rigidBody->setFriction(f);
}

void RigidBody::setDamping(const float d) {
    damping = d;
    if (rigidBody == nullptr) return;
    rigidBody->setDamping(d, d);
}

void RigidBody::setBounciness(const float b) {
    rigidBody->setRestitution(b);
    if (rigidBody == nullptr) return;
    bounciness = b;
}

void RigidBody::setVelocity(const Vector3 v) {
    //rigidBody->activate(true);
    if (rigidBody == nullptr) return;
    rigidBody->setLinearVelocity(toBtVector3(v));
}

void RigidBody::setGravity(const Vector3 g) {
    if (rigidBody == nullptr) return;
    rigidBody->setGravity(toBtVector3(g));
}

void RigidBody::addForce(const Vector3 f) {
    if (rigidBody == nullptr) return;
    rigidBody->applyCentralForce(toBtVector3(f));
}

void RigidBody::addImpulse(const Vector3 f) {
    if (rigidBody == nullptr) return;
    rigidBody->applyCentralImpulse(toBtVector3(f));
}

void RigidBody::setMask(const int m) {
    mask = m;
    if (rigidBody == nullptr) return;
    btBroadphaseProxy* bdProxy = rigidBody->getBroadphaseProxy();
    bdProxy->m_collisionFilterMask = m;
}

void RigidBody::setGroup(const int g) {
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

bool RigidBody::getActiveRB() const { return activeRigidBody; }

Vector3 RigidBody::getTensor() const { return toVector3(rigidBody->getLocalInertia()); }

float RigidBody::getFriction() const { return friction; }

float RigidBody::getDamping() const { return damping; }

float RigidBody::getBounciness() const { return bounciness; }

Vector3 RigidBody::getVelocity() const { return toVector3(rigidBody->getLinearVelocity()); }

Vector3 RigidBody::getAngularVelocity() const { return toVector3(rigidBody->getAngularVelocity()); }

Vector3 RigidBody::getGravity() const { return toVector3(rigidBody->getGravity()); }

Vector3 RigidBody::getTotalForce() const { return toVector3(rigidBody->getTotalForce());
}
Vector3 RigidBody::getPushVelocity() const { return toVector3(rigidBody->getPushVelocity()); }


int RigidBody::getMask() const { return mask; }

int RigidBody::getGroup() const { return group; }


}
