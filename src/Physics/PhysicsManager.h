#pragma once

#pragma region includes
#include <unordered_set>
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
#include "Physics_def.h"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btBroadphaseInterface;
class btConstraintSolver;
class btDynamicsWorld;
class btCollisionObject;
class btManifoldPoint;
class btVector3;
class btCollisionShape;
class btDiscreteDynamicsWorld;
class btRigidBody;
class Collider;
#pragma endregion

using namespace std;
namespace Tapioca {
class Vector3;
class PhysicsManager : public Singleton<PhysicsManager> /*, public Module*/ {

    friend Singleton<PhysicsManager>;

private:
#pragma region métodos

    PhysicsManager();

    //inicialización del mundo
    void init() /*override*/;

#pragma endregion

#pragma region parámetros
    //configuración predeterminada para la detección de colisiones
    btDefaultCollisionConfiguration* colConfig;
    //detectar pares de objetos de la misma región
    btBroadphaseInterface* broadphase;
    //confirmar la colisión, notificar a los objetos que se colisionan y callbacks
    btCollisionDispatcher* colDispatch;
    //resolver la interacción de colisiones y cálculos de fuerzas resultantes
    btSequentialImpulseConstraintSolver* constraintSolver;
    //el mundo
    btDiscreteDynamicsWorld* dynamicsWorld;
    //almacenado todos los rigidbodies del mundo
    unordered_set<btRigidBody*> rigidBodies;

#pragma endregion

public:
#pragma region métodos

    ~PhysicsManager();

    // Para evitar copiar/mover la instancia
    PhysicsManager(PhysicsManager&) = delete;
    PhysicsManager(PhysicsManager&&) = delete;
    PhysicsManager& operator=(PhysicsManager&) = delete;
    PhysicsManager& operator=(PhysicsManager&&) = delete;

    void update(float frameRate);

    void fixedUpdate(float deltaTime);


    /*
    /// @brief  crear un rigidbody
    /// @params position posición inicial del rb
    /// @params rotation inicial del rb
    /// @params shapeScale tamaño del shape del collider
    /// @params colliderShape tipo de shape del collider
    /// @params mass masa del rb
    /// @return puntero al rb creado
    */
    btRigidBody* createRigidBody(Vector3 position, Vector3 rotation, Vector3 shapeScale,
        ColliderShape colliderShape = BOX_SHAPE, MovementType type = STATIC_OBJECT, float mass = 0, float friction = 0,
        float bounciness = 0, bool isTrigger = false, int group = 1, int mask = -1);

    /*
    /// @brief  destruir rigidBody
    /// @params rb puntero al rb que se quiere destruir
    */
    void destroyRigidBody(btRigidBody* rb);

    //destruir el mundo
    void destroy();

#pragma endregion
};
}