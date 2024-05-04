#include "PhysicsManager.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "Utilities/Vector3.h"
#include "Utilities/Quaternion.h"
#include "Components/RigidBody.h"
#include "checkML.h"
#include "Structure/MainLoop.h"
#undef new
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

    if (obj1 != nullptr && obj2 != nullptr) {
        RigidBody* col1 = static_cast<RigidBody*>(obj1->getUserPointer());
        RigidBody* col2 = static_cast<RigidBody*>(obj2->getUserPointer());
        if (col2->getActiveRB() && col1->getActiveRB()) {
            col1->onCollisionEnter(col2->getObject());
            col2->onCollisionEnter(col1->getObject());
        }
    }
}

void onCollisionExit(btPersistentManifold* const& manifold) {
    const btCollisionObject* obj1 = manifold->getBody0();
    const btCollisionObject* obj2 = manifold->getBody1();

    if (obj1 != nullptr && obj2 != nullptr) {
        RigidBody* col1 = static_cast<RigidBody*>(obj1->getUserPointer());
        RigidBody* col2 = static_cast<RigidBody*>(obj2->getUserPointer());
        if (col2->getActiveRB() && col1->getActiveRB()) {
            col1->onCollisionExit(col2->getObject());
            col2->onCollisionExit(col1->getObject());
        }
    }
}

bool onCollisionStay(btManifoldPoint& manifold, void* obj1, void* obj2) {
    const btCollisionObject* body1 = static_cast<btCollisionObject*>(obj1);
    const btCollisionObject* body2 = static_cast<btCollisionObject*>(obj2);

    if (body1 != nullptr && body2 != nullptr) {

        RigidBody* col1 = static_cast<RigidBody*>(body1->getUserPointer());
        RigidBody* col2 = static_cast<RigidBody*>(body2->getUserPointer());

        if (col2->getActiveRB() && col1->getActiveRB()) {
            col1->onCollisionStay(col2->getObject());
            col2->onCollisionStay(col1->getObject());
        }
    }
    return true;
}


PhysicsManager::PhysicsManager(bool debug)
    : colConfig(nullptr), colDispatch(nullptr), broadphase(nullptr), constraintSolver(nullptr), dynamicsWorld(nullptr)
#ifdef _DEBUG
      ,
      pdd(nullptr), debug(debug)
#else
      ,
      debug(false)
#endif
{
}

PhysicsManager::~PhysicsManager() { destroy(); }

void PhysicsManager::start() {
    logInfo("PhysicsManager: Object:");

    colConfig = new btDefaultCollisionConfiguration();
    colDispatch = new btCollisionDispatcher(colConfig);
    broadphase = new btDbvtBroadphase();
    constraintSolver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(colDispatch, broadphase, constraintSolver, colConfig);

    dynamicsWorld->setGravity(btVector3(0, -10.0, 0));

    gContactStartedCallback = onCollisionEnter;
    gContactEndedCallback = onCollisionExit;
    gContactProcessedCallback = onCollisionStay;

#ifdef _DEBUG
    pdd = new PhysicsDebugDrawer();
    dynamicsWorld->setDebugDrawer(pdd);
#endif
}

bool PhysicsManager::loadObj(const std::string& filename, btTriangleMesh* triangleMesh) { 
       std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        logError("[Motor]: Error al abrir el archivo.obj ");
        return false;
    }

    std::vector<btVector3> vertices;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            btScalar x, y, z;
            iss >> x >> y >> z;
            vertices.push_back(btVector3(x, y, z));
        }
        else if (prefix == "f") {
            std::string v1, v2, v3;
            iss >> v1 >> v2 >> v3;
            int vi1, vi2, vi3;   // Índices de los vértices
            sscanf(v1.c_str(), "%d", &vi1);
            sscanf(v2.c_str(), "%d", &vi2);
            sscanf(v3.c_str(), "%d", &vi3);
            triangleMesh->addTriangle(vertices[vi1 - 1], vertices[vi2 - 1], vertices[vi3 - 1]);
        }
    }
    return true; 
}

btBvhTriangleMeshShape* PhysicsManager::createMeshCollision(const std::string& name) { 

    btTriangleMesh* objTriangleMesh;
    if (meshInterfaces.count(name)) {
        objTriangleMesh = meshInterfaces.at(name);
    }
    else {
        objTriangleMesh = new btTriangleMesh();
        if (!loadObj(name, objTriangleMesh)) {
            delete objTriangleMesh;
            return nullptr;
        }
        else {
            meshInterfaces[name] = objTriangleMesh;
        }
    }
    // Crea la forma de colisión basada en malla
    btBvhTriangleMeshShape* groundShape = new btBvhTriangleMeshShape(objTriangleMesh, true);
    return groundShape;
}

void PhysicsManager::update(const uint64_t deltaTime) {
#ifdef _DEBUG
    if (debug) dynamicsWorld->debugDrawWorld();
#endif
}

void PhysicsManager::fixedUpdate() { dynamicsWorld->stepSimulation(MainLoop::FIXED_DELTA_TIME, 1); }

btRigidBody* PhysicsManager::createRigidBody(const Vector3 position, const Quaternion rotation,
                                             const Vector3 shapeScale, const ColliderShape colliderShape,
                                             const MovementType type, float mass, const float friction,
                                             const float damping, const float bounciness, const bool isTrigger,
                                             const int group, const int mask, const std::string file) {
    btVector3 scale = toBtVector3(shapeScale);
    btVector3 pos = toBtVector3(position);
    btQuaternion rot = btQuaternion(rotation.vector.x, rotation.vector.y, rotation.vector.z, rotation.scalar);
    btCollisionShape* shape;

    switch (colliderShape) {
    case BOX_SHAPE: shape = new btBoxShape(scale); break;
    case SPHERE_SHAPE: shape = new btSphereShape(scale.getX()); break;
    case PLANE_SHAPE: shape = new btStaticPlaneShape(scale, 0); break;
    case CAPSULE_SHAPE: shape = new btCapsuleShape(scale.getX(), scale.getY()); break;
    case MESH_SHAPE: shape = createMeshCollision(file); break;
    default: shape = new btBoxShape(scale); break;
    }

    btVector3 inertia;
    inertia.setZero();

    // El rigidbody es dinamico si la masa !=0, de lo contrario es estatico
    if (type == DYNAMIC_OBJECT) shape->calculateLocalInertia(mass, inertia);
    else
        mass = 0;

    // Establece el Transform (posicion y rotacion)
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(pos);
    transform.setRotation(rot);

    // Para sincronizar el transform con el grafico
    btMotionState* motionState = new btDefaultMotionState(transform);

    btRigidBody* rb = new btRigidBody(mass, motionState, shape, inertia);
    rigidBodies.insert(rb);

    // Si es un cuerpo dinamico, tiene que estar siempre activo para actualizar su movimiento y detectar colision
    if (type != STATIC_OBJECT) rb->setActivationState(DISABLE_DEACTIVATION);

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

    for (auto m : meshInterfaces) {
        delete m.second;
    }
    meshInterfaces.clear();

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

void PhysicsManager::activateDebug(bool d) {
#ifdef _DEBUG
    debug = d;
#else
    logWarn("PhysicsManager: se intento activar debug en modo release\n");
#endif
}

void PhysicsManager::destroyRigidBody(btRigidBody* const rb) {
    rigidBodies.erase(rb);
    if (rb && rb->getMotionState()) delete rb->getMotionState();
    delete rb->getCollisionShape();
    dynamicsWorld->removeCollisionObject(rb);
    delete rb;
}

void PhysicsManager::removeRigidBody(btRigidBody* const rb) { dynamicsWorld->removeCollisionObject(rb); }

void PhysicsManager::addRigidBody(btRigidBody* const rb) { dynamicsWorld->addRigidBody(rb); }
}