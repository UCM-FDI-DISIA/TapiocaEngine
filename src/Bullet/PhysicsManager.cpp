#include "PhysicsManager.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "../Core/Utilities/Vector3.h"

#include <iostream>   //PRUEBA
namespace Tapioca {

PhysicsManager::PhysicsManager()
    : colConfig(nullptr)
    , colDispatch(nullptr)
    , broadphase(nullptr)
    , constraintSolver(nullptr)
    , dynamicsWorld(nullptr) {
    setup();
}

PhysicsManager::~PhysicsManager() { destroy(); }

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

void PhysicsManager::setup() {

    colConfig = new btDefaultCollisionConfiguration();

    colDispatch = new btCollisionDispatcher(colConfig);

    broadphase = new btDbvtBroadphase();

    constraintSolver = new btSequentialImpulseConstraintSolver();

    dynamicsWorld = new btDiscreteDynamicsWorld(colDispatch, broadphase, constraintSolver, colConfig);

    dynamicsWorld->setGravity(btVector3(0, -10, 0));

    createRigidBody(Vector3(0), Vector3(0), Vector3(1.f), Sphere, 6);   //PRUEBA
    createRigidBody(Vector3(-1), Vector3(0), Vector3(4.f), Box, 8);   //PRUEBA

    //gContactStartedCallback;
    //gContactAddedCallback ;
    //gContactEndedCallback;
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

        std::cout << "Object: " << i << " Transform: " << tr.getOrigin().getX() << " " << tr.getOrigin().getY() << " "
                  << tr.getOrigin().getZ() << "\n";
    }
    //...........................................
}

void PhysicsManager::fixedUpdate(float deltaTime) { dynamicsWorld->stepSimulation(deltaTime); }

btVector3 PhysicsManager::toBtVector3(Vector3 v) { return btVector3(v.x, v.y, v.z); };
Vector3 PhysicsManager::toVector3(btVector3 v) { return Vector3(v.getX(), v.getY(), v.getZ()); }


btRigidBody* PhysicsManager::createRigidBody(
    Vector3 position, Vector3 rotation, Vector3 shapeScale, ColliderShape colliderShape, float mass) {

    btVector3 scale = toBtVector3(shapeScale);
    btVector3 pos = toBtVector3(position);
    btVector3 rot = toBtVector3(rotation);

    //collider shape
    btCollisionShape* shape;

    switch (colliderShape) {
    case PhysicsManager::Box:
        shape = new btBoxShape(scale);
        break;
    case PhysicsManager::Sphere:
        shape = new btSphereShape(scale.getX());
        break;
    case PhysicsManager::Plane:
        shape = new btStaticPlaneShape(scale, 0);
        break;
    case PhysicsManager::Capsule:
        shape = new btCapsuleShape(scale.getX(), scale.getY());
        break;
    default:
        shape = new btBoxShape(scale);
        break;
    }


    btVector3 inertia;
    inertia.setZero();
    // El rigidbody es dinámico si la masa !=0, de lo contrario es estático
    if (!mass) shape->calculateLocalInertia(mass, inertia);

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

    //DUDA: no sé para qué es, pero creo q serán útiles
    //rb->forceActivationState(DISABLE_DEACTIVATION);
    //rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

    dynamicsWorld->addRigidBody(rb);
    //dynamicsWorld->addCollisionObject(rb, )

    return rb;
}
}