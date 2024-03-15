#pragma once
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

namespace Tapioca {
class Vector3;
class PhysicsDebugDrawer;

class TAPIOCA_API PhysicsManager : public Singleton<PhysicsManager> , public Module {
    friend Singleton<PhysicsManager>;

private:
    btDefaultCollisionConfiguration* colConfig;                 // configuracion predeterminada para la deteccion de colisiones
    btBroadphaseInterface* broadphase;                          // detectar pares de objetos de la misma region
    btCollisionDispatcher* colDispatch;                         // confirmar la colision, notificar a los objetos que se colisionan y callbacks
    btSequentialImpulseConstraintSolver* constraintSolver;      // resolver la interaccion de colisiones y calculos de fuerzas resultantes
    btDiscreteDynamicsWorld* dynamicsWorld;                     // el mundo

// warning C4251: 'Tapioca::PhysicsManager::rigidBodies':
// class ' std::unordered_set<btRigidBody*, std::hash<btRigidBody*>, std::equal_to<btRigidBody*>,
// std::allocator<btRigidBody*>>' necesita tener una interfaz DLL para que la
// utilicen los clientes de class 'Tapioca::PhysicsManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::unordered_set<btRigidBody*> rigidBodies;   // almacenado todos los rigidbodies del mundo
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

#ifdef _DEBUG                                                      
    PhysicsDebugDrawer* pdd;                                    // para dibujar las fisicas
#endif

    /*
    * @brief Constructora de la clase PhysicsManager.
    */
    PhysicsManager();

    /*
    * @brief Inicializa el manager
    */
    bool init() override;

public:
    /*
    * @brief Destructora de la clase InputManager.
    */
    ~PhysicsManager();

    // Para evitar copiar/mover la instancia
    PhysicsManager(PhysicsManager&) = delete;
    PhysicsManager(PhysicsManager&&) = delete;
    PhysicsManager& operator=(PhysicsManager&) = delete;
    PhysicsManager& operator=(PhysicsManager&&) = delete;


    void fixedUpdate() override;
    void update(const uint64_t deltaTime);

    /*
    * @brief  Crea un rigidbody
    * @param  position Posicion inicial del rb
    * @param  rotation Rotacion inicial del rb
    * @param  shapeScale Escala del shape del collider
    * @param  colliderShape Tipo de shape del collider
    * @param  mass Masa del rb
    * @param  friction Coeficiente de friccion del rb
    * @param  damping Damping del rb
    * @param  bonciness Lo que rebota el rb
    * @param  isTrigger True para declararlo como trigger, false en caso contrario
    * @param  group Grupo al que pertenece el rb
    * @return Puntero al rb creado
    */
    btRigidBody* createRigidBody(const Vector3 position, const Vector3 rotation, const Vector3 shapeScale,
                                 const ColliderShape colliderShape = BOX_SHAPE, 
                                 const MovementType type = STATIC_OBJECT,
                                 float mass = 0, const float friction = 0,
                                 const float damping = 0,const float bounciness = 0, 
                                 const bool isTrigger = false, const int group = 1, const int mask = -1);

    /*
    * @brief  destruir rigidBody
    * @param rb puntero al rb que se quiere destruir
    */
    void destroyRigidBody(btRigidBody* const rb);

    /*
    * @brief Destruye el mundo
    */
    void destroy();
};
}