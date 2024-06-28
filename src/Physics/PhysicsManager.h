#pragma once
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
#include "physicsDefs.h"

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
class btBvhTriangleMeshShape;
class btTriangleMesh;

namespace Tapioca {
class Vector3;
class Quaternion;
class PhysicsDebugDrawer;
class RigidBody;

/**
* @brief Clase que se encarga de gestionar las fisicas del juego
*/
class TAPIOCA_API PhysicsManager : public Singleton<PhysicsManager>, public Module {
private:
    friend Singleton<PhysicsManager>;

    btDefaultCollisionConfiguration* colConfig;   // Configuracion predeterminada para la deteccion de colisiones
    btBroadphaseInterface* broadphase;            // Detecta pares de objetos de la misma region
    btCollisionDispatcher* colDispatch;   // Confirma la colision, notificar a los objetos que se colisionan y callbacks

    // Resuelve la interaccion de colisiones y calculos de fuerzas resultantes
    btSequentialImpulseConstraintSolver* constraintSolver;
    btDiscreteDynamicsWorld* dynamicsWorld;   // Mundo de fisicas

// warning C4251 'Tapioca::PhysicsManager::meshInterfaces' : class
// 'std::unordered_map<std::string,btTriangleMesh *,std::hash<std::string>,std::equal_to<std::string>,std::allocator<std::pair<const std::string,btTriangleMesh *>>>' necesita
// tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::PhysicsManager'
// warning C4251: 'Tapioca::PhysicsManager::rigidBodies':
// class ' std::unordered_set<btRigidBody*, std::hash<btRigidBody*>, std::equal_to<btRigidBody*>,
// std::allocator<btRigidBody*>>' necesita tener una interfaz DLL para que la
// utilicen los clientes de class 'Tapioca::PhysicsManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::unordered_map<std::string, btTriangleMesh*> meshInterfaces;   // Dados para mesh collider
    std::unordered_set<btRigidBody*> rigidBodies;                      // Almacena todos los rigidbodies
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    bool debug;   // Indica si se quiere debugear las fisicas

#ifdef _DEBUG
    PhysicsDebugDrawer* pdd;   // Si debug es true, se crea un drawer para debugear
#endif

    /**
    * @brief Constructor por defecto
    */
    PhysicsManager(bool debug = false);

    /**
    * @brief Inicializa el manager
    */
    void start() override;

    /**
    * @brief Crea un mesh de colision
    * @param name Nombre del mesh
    */
    btBvhTriangleMeshShape* createMeshCollision(const std::string& name);

    /**
    * @brief Carga un objeto a partir de un nombre de archivo y la forma de colision
    * @param filename Nombre del archivo
    * @param shape Puntero al mesh
    */
    bool loadObj(const std::string& filename, btTriangleMesh* shape);

public:
    /**
    * @brief Destructor por defecto
    */
    ~PhysicsManager();

    PhysicsManager(PhysicsManager&) = delete;
    PhysicsManager(PhysicsManager&&) = delete;
    PhysicsManager& operator=(PhysicsManager&) = delete;
    PhysicsManager& operator=(PhysicsManager&&) = delete;

    /**
    * @brief Dibuja las cajas de colision de los objetos
    * @param deltaTime Tiempo transcurrido desde el ultimo frame
    */
    void update(const uint64_t deltaTime);
    /**
    * @brief Actualiza el mundo de fisicas
    */
    void fixedUpdate() override;

    /**
    * @brief  Crea un rigidbody
    * @param  position Posicion inicial del rb
    * @param  rotation Rotacion inicial del rb
    * @param  shapeScale Escala del shape del collider
    * @param  colliderShape Tipo de shape del collider
    * @param  type Tipo de movimiento del rb
    * @param  mass Masa del rb
    * @param  friction Coeficiente de friccion del rb
    * @param  damping Damping del rb
    * @param  bounciness Lo que rebota el rb
    * @param  isTrigger True para declararlo como trigger, false en caso contrario
    * @param  group Grupo al que pertenece el rb
    * @param  mask Mascara de colisiones del rb
    * @param  file Nombre del archivo que contiene el mesh
    * @return Puntero al rb creado
    */
    btRigidBody* createRigidBody(const Vector3 position, const Quaternion rotation, const Vector3 shapeScale,
                                 RigidBody* rigidBody,
                                 const ColliderShape colliderShape = BOX_SHAPE, const MovementType type = STATIC_OBJECT,
                                 float mass = 0, const float friction = 0, const float damping = 0,
                                 const float bounciness = 0, const bool isTrigger = false, const int group = 1,
                                 const int mask = -1, const std::string file = "");

    /**
    * @brief Destruye un rigidBody
    * @param rb puntero al rb que se quiere destruir
    */
    void destroyRigidBody(btRigidBody* const rb);

    /**
    * @brief Elimina un rigidBody del mundo
    * @param rb puntero al rb que se quiere quitar
    */
    void removeRigidBody(btRigidBody* const rb);

    /**
    * @brief Anade un rigibody al mundo
    * @param rb puntero al rb que se quiere anadir
    */
    void addRigidBody(btRigidBody* const rb);

    /**
    * @brief Destruye el mundo de fisicas
    */
    void destroy();

    /**
    * @brief Devuelve el mundo de fisicas
    * @return Puntero al mundo de fisicas
    */
    inline btDiscreteDynamicsWorld* getWorld() const { return dynamicsWorld; }

    /**
    * @brief Establece si se quiere debugear las fisicas
    * @param d True para activar el debug, false en caso contrario
    */
    void activateDebug(bool d);

    /**
    * @brief Evalua si en el rango del raycast se detecta alguna colisión (solo tiene en cuenta la primera)
    * @param start punto en el que comienza el raycast
    * @param end punto en el que termina el raycast (devuelve el punto donde colisiona)
    * @param normal dirección del raycast
    * @return true si el raycast detecta algún objeto false si no detecta ninguno
    */
    bool Raycast(const Vector3& start, Vector3& end, Vector3& normal, RigidBody*& rigidBody, const int mask = -1);

    /**
    * @brief Evalua si en el rango del raycast se detecta alguna colisión (tiene en cuenta todas las colisiones entre start y end)
    * @param start punto en el que comienza el raycast
    * @param end punto en el que termina el raycast
    * @param normal dirección del raycast
    * @return true si el raycast detecta algún objeto false si no detecta ninguno
    */
    bool RaycastAll(const Vector3& start, Vector3& end, Vector3& normal, std::vector<RigidBody*>& rigidBodies,
                    const int mask = -1);
};
}