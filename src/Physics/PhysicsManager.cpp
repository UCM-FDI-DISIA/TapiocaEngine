#include "PhysicsManager.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "../Core/Utilities/Vector3.h"
#include "Components/Collider.h"

#include <iostream>   //PRUEBA
namespace Tapioca {
void onCollisionEnter(btPersistentManifold* const& manifold) {

    const btCollisionObject* obj1 = manifold->getBody0();
    const btCollisionObject* obj2 = manifold->getBody1();

    cout << "Entro\n";
    if (obj1 != nullptr && obj2 != nullptr) {
        Collider* col1 = static_cast<Collider*>(obj1->getUserPointer());
        Collider* col2 = static_cast<Collider*>(obj2->getUserPointer());
        if (col2 != nullptr && col1 != nullptr) {
            col1->onCollisionEnter(col2->getParent());
            col2->onCollisionEnter(col1->getParent());
        }
    }
}

void onCollisionExit(btPersistentManifold* const& manifold) {

    const btCollisionObject* obj1 = manifold->getBody0();
    const btCollisionObject* obj2 = manifold->getBody1();
    cout << "Salgo\n";
    if (obj1 != nullptr && obj2 != nullptr) {
        Collider* col1 = static_cast<Collider*>(obj1->getUserPointer());
        Collider* col2 = static_cast<Collider*>(obj2->getUserPointer());
        if (col2 != nullptr && col1 != nullptr) {
            col1->onCollisionExit(col2->getParent());
            col2->onCollisionExit(col1->getParent());
        }
    }
}

bool onCollisionStay(btManifoldPoint& maniforlPoint, const btCollisionObjectWrapper* colObj0Wrap, int partId0,
    int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) {

    void* obj1 = colObj0Wrap->getCollisionObject()->getUserPointer();
    void* obj2 = colObj1Wrap->getCollisionObject()->getUserPointer();
    cout << "A\n";
    if (obj1 != nullptr && obj2 != nullptr) {
        Collider* col1 = static_cast<Collider*>(obj1);
        Collider* col2 = static_cast<Collider*>(obj2);
        if (col1 != nullptr && col2 != nullptr) {
            col1->onCollisionStay(col2->getParent());
            col2->onCollisionStay(col1->getParent());
        }
    }

    return false;
}
PhysicsManager::PhysicsManager()
    : colConfig(nullptr)
    , colDispatch(nullptr)
    , broadphase(nullptr)
    , constraintSolver(nullptr)
    , dynamicsWorld(nullptr) {
    init();   //DEBUG
}

PhysicsManager::~PhysicsManager() { destroy(); }


void PhysicsManager::init() {

    colConfig = new btDefaultCollisionConfiguration();

    colDispatch = new btCollisionDispatcher(colConfig);

    broadphase = new btDbvtBroadphase();

    constraintSolver = new btSequentialImpulseConstraintSolver();

    dynamicsWorld = new btDiscreteDynamicsWorld(colDispatch, broadphase, constraintSolver, colConfig);

    dynamicsWorld->setGravity(btVector3(0, 0, 0));


    gContactStartedCallback = onCollisionEnter;
    gContactAddedCallback = onCollisionStay;
    gContactEndedCallback = onCollisionExit;

    createRigidBody(Vector3(0, 0, 0), Vector3(0), Vector3(5.f), SPHERE_SHAPE, DYNAMIC_OBJECT, 8, 0, 0, 0, 1,
        (1 << 4) | (1 << 1) | (1 << 2));                                                                        //PRUEBA
    createRigidBody(Vector3(0, 0, 0), Vector3(0), Vector3(4.f), BOX_SHAPE, DYNAMIC_OBJECT, 6, 0, 0, 0, 4, 1);   //PRUEBA
}

void PhysicsManager::update(float frameRate) {
    //simulación física y detección de colisión
    dynamicsWorld->stepSimulation(frameRate, 10);

    //PRUEBA: printear las pos de los rb
    for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform tr;
        if (body && body->getMotionState()) {
            body->getMotionState()->getWorldTransform(tr);
        } else
            tr = obj->getWorldTransform();

        //std::cout << "Object: " << i << " Transform: " << tr.getOrigin().getX() << " " << tr.getOrigin().getY() << " "
        //          << tr.getOrigin().getZ() << "\n";
    }
    //...........................................
}

void PhysicsManager::fixedUpdate(float deltaTime) { dynamicsWorld->stepSimulation(deltaTime); }


btRigidBody* PhysicsManager::createRigidBody(Vector3 position, Vector3 rotation, Vector3 shapeScale,
    ColliderShape colliderShape, MovementType type, float mass, float friction, float bounciness, bool isTrigger,
    int group, int mask) {

    btVector3 scale = toBtVector3(shapeScale);
    btVector3 pos = toBtVector3(position);
    btVector3 rot = toBtVector3(rotation);

    //collider shape
    btCollisionShape* shape;

    switch (colliderShape) {
    case BOX_SHAPE:
        shape = new btBoxShape(scale);
        break;
    case SPHERE_SHAPE:
        shape = new btSphereShape(scale.getX());
        break;
    case PLANE_SHAPE:
        shape = new btStaticPlaneShape(scale, 0);
        break;
    case CAPSULE_SHAPE:
        shape = new btCapsuleShape(scale.getX(), scale.getY());
        break;
    default:
        shape = new btBoxShape(scale);
        break;
    }

    btVector3 inertia;
    inertia.setZero();

    // El rigidbody es dinámico si la masa !=0, de lo contrario es estático
    if (type == DYNAMIC_OBJECT) shape->calculateLocalInertia(mass, inertia);
    else if (type == STATIC_OBJECT)
        mass = 0;


    //settear Transform (posición y rotación)
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(pos);

    btQuaternion quaternion;
    quaternion.setEuler(rot.getY(), rot.getX(), rot.getZ());
    transform.setRotation(quaternion);

    //para sincronizar el transform con el gráfico
    btMotionState* motionState = new btDefaultMotionState(transform);

    btRigidBody* rb = new btRigidBody(mass, motionState, shape, inertia);
    rigidBodies.insert(rb);

    //si es un cuerpo dinámico, tiene que estar siempre activo para actualizar su movimiento y detectar colisión
    if (type == DYNAMIC_OBJECT) rb->setActivationState(DISABLE_DEACTIVATION);

    rb->setCollisionFlags(rb->getCollisionFlags() | type);

    if (isTrigger) rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    rb->setFriction(friction);
    rb->setRestitution(bounciness);
    dynamicsWorld->addRigidBody(rb, group, mask);

    return rb;
}
void PhysicsManager::destroy() {

    auto it = rigidBodies.begin();
    while (it != rigidBodies.end()) {
        destroyRigidBody(*it);
        it = rigidBodies.erase(it);
    }

    delete colConfig;
    colConfig = nullptr;

    delete colDispatch;
    colDispatch = nullptr;

    delete broadphase;
    broadphase = nullptr;

    delete constraintSolver;
    constraintSolver = nullptr;

    delete dynamicsWorld;
    dynamicsWorld = nullptr;
}

void PhysicsManager::destroyRigidBody(btRigidBody* rb) {

    if (rb && rb->getMotionState()) delete rb->getMotionState();
    delete rb->getCollisionShape();
    dynamicsWorld->removeCollisionObject(rb);
    delete rb;
}
}