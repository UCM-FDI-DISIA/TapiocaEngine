#include "PhysicsManager.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "Utilities/Vector3.h"
#include "Components/Collider.h"
#include "checkML.h"
#include "Structure/Game.h"
#undef new DBG_NEW
#ifdef _DEBUG
#include "PhysicsDebugDrawer.h"
#endif

namespace Tapioca {
template class TAPIOCA_API Singleton<PhysicsManager>;
template<>
PhysicsManager* Singleton<PhysicsManager>::instance_ = nullptr;

void onCollisionEnter(btPersistentManifold* const& manifold) {

    const btCollisionObject* obj1 = manifold->getBody0();
    const btCollisionObject* obj2 = manifold->getBody1();
#ifdef _DEBUG
    std::cout << "Entro\n";
#endif

    if (obj1 != nullptr && obj2 != nullptr) {
        Collider* col1 = static_cast<Collider*>(obj1->getUserPointer());
        Collider* col2 = static_cast<Collider*>(obj2->getUserPointer());
        if (col2 != nullptr && col1 != nullptr) {
            col1->onCollisionEnter(col2->getObject());
            col2->onCollisionEnter(col1->getObject());
        }
    }
}

void onCollisionExit(btPersistentManifold* const& manifold) {

    const btCollisionObject* obj1 = manifold->getBody0();
    const btCollisionObject* obj2 = manifold->getBody1();
#ifdef _DEBUG
    std::cout << "Salgo\n";
#endif
    if (obj1 != nullptr && obj2 != nullptr) {
        Collider* col1 = static_cast<Collider*>(obj1->getUserPointer());
        Collider* col2 = static_cast<Collider*>(obj2->getUserPointer());
        if (col2 != nullptr && col1 != nullptr) {
            col1->onCollisionExit(col2->getObject());
            col2->onCollisionExit(col1->getObject());
        }
    }
}

bool onCollisionStay(btManifoldPoint& maniforlPoint, const btCollisionObjectWrapper* const colObj0Wrap,
                     const int partId0, int index0, const btCollisionObjectWrapper* const colObj1Wrap,
                     const int partId1, const int index1) {

    void* obj1 = colObj0Wrap->getCollisionObject()->getUserPointer();
    void* obj2 = colObj1Wrap->getCollisionObject()->getUserPointer();
#ifdef _DEBUG
    std::cout << "A\n";
#endif
    if (obj1 != nullptr && obj2 != nullptr) {
        Collider* col1 = static_cast<Collider*>(obj1);
        Collider* col2 = static_cast<Collider*>(obj2);
        if (col1 != nullptr && col2 != nullptr) {
            col1->onCollisionStay(col2->getObject());
            col2->onCollisionStay(col1->getObject());
        }
    }

    return false;
}
PhysicsManager::PhysicsManager()
    : colConfig(nullptr), colDispatch(nullptr), broadphase(nullptr), constraintSolver(nullptr), dynamicsWorld(nullptr)
#ifdef _DEBUG
      ,
      pdd(nullptr)
#endif   // _DEBUG
{
    //  init();
}

PhysicsManager::~PhysicsManager() { destroy(); }

bool PhysicsManager::init() {
    // CONTROLAR LOS POSIBLES ERRORES PARA DEVOLVER FALSE

#ifdef _DEBUG
    std::cout << "Object: ";
#endif

    colConfig = new btDefaultCollisionConfiguration();
    colDispatch = new btCollisionDispatcher(colConfig);
    broadphase = new btDbvtBroadphase();
    constraintSolver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(colDispatch, broadphase, constraintSolver, colConfig);

    dynamicsWorld->setGravity(btVector3(0, -10.0, 0));

    gContactStartedCallback = onCollisionEnter;
    gContactAddedCallback = onCollisionStay;
    gContactEndedCallback = onCollisionExit;

    //createRigidBody(Vector3(-5, 0, 0), Vector3(0), Vector3(5.f), SPHERE_SHAPE, DYNAMIC_OBJECT, 1, 1, 0.4, 10, 0, 1,
    //                (1 << 2) | (0 << 1) | (1 << 0));   //PRUEBA
    //createRigidBody(Vector3(0, 0, 0), Vector3(0), Vector3(2.f), BOX_SHAPE, DYNAMIC_OBJECT, 1, 1, 0.4, 10, 0, 4,
    //                1);   //PRUEBA

#ifdef _DEBUG
    pdd = new PhysicsDebugDrawer();
    dynamicsWorld->setDebugDrawer(pdd);
#endif   // _DEBUG

    return true;
}


void PhysicsManager::update(const uint64_t deltaTime) {
  // dynamicsWorld->stepSimulation(Game::FIXED_DELTA_TIME, 10);

#ifdef _DEBUG
    dynamicsWorld->debugDrawWorld();

#endif }
}
void PhysicsManager::fixedUpdate() {
   dynamicsWorld->stepSimulation(Game::FIXED_DELTA_TIME, 1);
}
btRigidBody* PhysicsManager::createRigidBody(const Vector3 position, const Vector3 rotation, const Vector3 shapeScale,
                                             const ColliderShape colliderShape, const MovementType type, float mass,
                                             const float friction, const float damping, const float bounciness,
                                             const bool isTrigger, const int group, const int mask) {
    btVector3 scale = toBtVector3(shapeScale);
    btVector3 pos = toBtVector3(position);
    btVector3 rot = toBtVector3(rotation);

    btCollisionShape* shape;

    switch (colliderShape) {
        case BOX_SHAPE: shape = new btBoxShape(scale); break;
        case SPHERE_SHAPE: shape = new btSphereShape(scale.getX()); break;
        case PLANE_SHAPE: shape = new btStaticPlaneShape(scale, 0); break;
        case CAPSULE_SHAPE: shape = new btCapsuleShape(scale.getX(), scale.getY()); break;
        default: shape = new btBoxShape(scale); break;
    }

    btVector3 inertia;
    inertia.setZero();

    // El rigidbody es dinamico si la masa !=0, de lo contrario es estatico
    if (type == DYNAMIC_OBJECT) shape->calculateLocalInertia(mass, inertia);
    else if (type == STATIC_OBJECT)
        mass = 0;

    //settear Transform (posicion y rotacion)
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(pos);

    btQuaternion quaternion;
    quaternion.setEuler(rot.getY(), rot.getX(), rot.getZ());
    transform.setRotation(quaternion);

    //para sincronizar el transform con el grafico
    btMotionState* motionState = new btDefaultMotionState(transform);

    btRigidBody* rb = new btRigidBody(mass, motionState, shape, inertia);
    rigidBodies.insert(rb);

    //si es un cuerpo dinamico, tiene que estar siempre activo para actualizar su movimiento y detectar colision
    if (type == DYNAMIC_OBJECT) rb->setActivationState(DISABLE_DEACTIVATION);

    rb->setCollisionFlags(rb->getCollisionFlags() | type);

    if (isTrigger) rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    rb->setFriction(friction);
    rb->setDamping(damping, damping);
    rb->setRestitution(bounciness);
    dynamicsWorld->addRigidBody(rb, group, mask);

    return rb;
}


void PhysicsManager::destroy() {

    auto it = rigidBodies.begin();
    while (it != rigidBodies.end()) {
        auto itAux = it;
        ++it;
        destroyRigidBody(*itAux);
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

#ifdef _DEBUG
    delete pdd;
    pdd = nullptr;
#endif
}

void PhysicsManager::destroyRigidBody(btRigidBody* const rb) {
    rigidBodies.erase(rb);
    if (rb && rb->getMotionState()) delete rb->getMotionState();
    delete rb->getCollisionShape();
    dynamicsWorld->removeCollisionObject(rb);
    delete rb;
}


}