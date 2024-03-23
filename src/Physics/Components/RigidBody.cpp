#include "RigidBody.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "../../Physics/PhysicsManager.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
//#include "Collider.h"


namespace Tapioca {
RigidBody::RigidBody()
    : transform(nullptr), rigidBody(nullptr), mass(0), isTrigger(false), mask(-1), group(1), friction(0),
      colShape(BOX_SHAPE), movementType(STATIC_OBJECT), bounciness(0), colliderScale(Vector3(1)),activeRigidBody(true) { }

RigidBody::~RigidBody() {
    if (rigidBody != nullptr) {
        activeRigidBody = false;
        PhysicsManager::instance()->destroyRigidBody(rigidBody);
        rigidBody = nullptr;
    }
}

bool RigidBody::initComponent(const CompMap& variables) {

    int colShapeAux;

    bool colShapeSet = setValueFromMap(colShapeAux, "colShape", variables);
    if (!colShapeSet) {
#ifdef _DEBUG
        std::cerr << "Error: RigidBody: no se pudo inicializar colShape.\n";
#endif
        return false;
    }
    colShape = (ColliderShape)colShapeAux;

    bool colliderScaleSet = setValueFromMap(colliderScale.x, "colliderScaleX", variables) &&
        setValueFromMap(colliderScale.y, "colliderScaleY", variables) &&
        setValueFromMap(colliderScale.z, "colliderScaleZ", variables);
    if (!colliderScaleSet) {
#ifdef _DEBUG
        std::cerr << "Error: Transform: no se pudo inicializar colliderScale.\n";
#endif
        return false;
    }

    bool isTriggerSet = setValueFromMap(isTrigger, "isTrigger", variables);
    if (!isTriggerSet) {
#ifdef _DEBUG
        std::cerr << "Error: RigidBody: no se pudo inicializar isTrigger.\n";
#endif
        return false;
    }

    int movementTypeAux;
    bool movementTypeSet = setValueFromMap(movementTypeAux, "movementType", variables);
    if (!movementTypeSet) {
#ifdef _DEBUG
        std::cerr << "Error: RigidBody: no se pudo inicializar movementType.\n";
#endif
        return false;
    }

    movementType = (MovementType)movementTypeAux;

    bool massSet = setValueFromMap(mass, "mass", variables);
    if (!massSet) {
#ifdef _DEBUG
        std::cerr << "Error: RigidBody: no se pudo inicializar mass.\n";
#endif
        return false;
    }

    bool frictionSet = setValueFromMap(friction, "friction", variables);
    if (!frictionSet) {
#ifdef _DEBUG
        std::cerr << "Error: RigidBody: no se pudo inicializar friction.\n";
#endif
        return false;
    }

    bool dampingSet = setValueFromMap(damping, "damping", variables);
    if (!dampingSet) {
#ifdef _DEBUG
        std::cerr << "Error: RigidBody: no se pudo inicializar damping.\n";
#endif
        return false;
    }

    bool bouncinessSet = setValueFromMap(bounciness, "bounciness", variables);
    if (!bouncinessSet) {
#ifdef _DEBUG
        std::cerr << "Error: RigidBody: no se pudo inicializar bounciness.\n";
#endif
        return false;
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
        transform->setPosition(toVector3(rigidBody->getWorldTransform().getOrigin()),true);
        transform->setRotation(toEuler(rigidBody->getWorldTransform().getRotation()), true);
    }
}
void RigidBody::handleEvent(std::string const& id, void* info) {

    if (id == "transformChanged" &&  !(*(bool*)info)) {

        if (movementType == DYNAMIC_OBJECT) {
            btTransform& btTr = rigidBody->getWorldTransform();
            btTr.setOrigin(toBtVector3(transform->getGlobalPosition()));
            btTr.setRotation(toBtQuaternion(transform->getGlobalRotation()));
        }
        else if (movementType == KINEMATIC_OBJECT) {
            btTransform btTr;
            rigidBody->getMotionState()->getWorldTransform(btTr);
            btTr.setOrigin(toBtVector3(transform->getGlobalPosition()));
            btTr.setRotation(toBtQuaternion(transform->getGlobalRotation()));
            rigidBody->getMotionState()->setWorldTransform(btTr);
        }
    }
}
void RigidBody::onCollisionEnter(GameObject* const other) {
#ifdef _DEBUG
    std::cout << "Entro en collision\n";
#endif
    pushEvent("onCollisionEnter", other, false);
}

void RigidBody::onCollisionExit(GameObject* const other) {
#ifdef _DEBUG
    std::cout << "Salgo de collision\n";
#endif
    pushEvent("onCollisionExit", other, false);
}

void RigidBody::onCollisionStay(GameObject* const other) {
#ifdef _DEBUG
    std::cout << "Estoy en collision\n";
#endif
    pushEvent("onCollisionStay", other, false);
}

void RigidBody::awake() {

    transform = object->getComponent<Transform>();

    rigidBody = PhysicsManager::instance()->createRigidBody(
        transform->getGlobalPosition(), transform->getGlobalRotation(), colliderScale, colShape, movementType, mass,
        friction, damping, bounciness, isTrigger, group, mask);

    // collider = object->getComponent<Collider>();

    rigidBody->setUserPointer(this);
}

void RigidBody::setActive(const bool b) {
    Component::setActive(b);
    rigidBody->activate(b);
}

void RigidBody::setMomeventType(const MovementType t) {
    if (rigidBody == nullptr) return;
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() & ~movementType);
    rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | t);
    movementType = t;
}

void RigidBody::setTrigger(const bool t) {
    isTrigger = t;
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

void RigidBody::setTensor(const Vector3 t) { rigidBody->setMassProps(mass, toBtVector3(t)); };

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

Vector3 RigidBody::getGravity() const { return toVector3(rigidBody->getGravity()); }


int RigidBody::getMask() const { return mask; }

int RigidBody::getGroup() const { return group; }


}
