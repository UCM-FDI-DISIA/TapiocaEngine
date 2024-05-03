#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"
#include "physicsDefs.h"

class btRigidBody;

namespace Tapioca {
class Vector3;
class Transform;

/**
* @brief Clase que representa un rigidbody de bullet
*/
class TAPIOCA_API RigidBody : public Component {
private:
    btRigidBody* rigidBody;         // RigidBody de bullet
    Transform* transform;           // Transform del objeto
    ColliderShape colShape;         // Forma del collider
    Vector3 colliderInitialScale;   // Escala inicial del collider
    MovementType movementType;      // Tipo de movimiento
    bool isTrigger;                 // Si es un trigger o no

    int mask;    // Indica con que grupo se puede colisionar (en binario)
    int group;   // Filtro de colision

    float mass;         // Masa
    float friction;     // Friccion
    float damping;      // Damping
    float bounciness;   // Bote

    bool activeRigidBody;   // Indica si el rigidbody esta activo o no
    bool trackScale;        // Indica si el rigidbody escala con el transform

public:
    COMPONENT_ID("RigidBody");

    /**
    * @brief Constructor por defecto
    */
    RigidBody();
    /**
    * @brief Destructor por defecto
    */
    ~RigidBody();

    /**
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;
    /**
    * @brief Metodo que se usa para actualizar el estado de un componente
    * Se llama cada cierto tiempo fijo (Game::FIXED_DELTA_TIME),
    */
    void fixedUpdate() override;

    /**
    * @brief Metodo que se usa para recibir eventos.
    * Se llama nada mas se recibe el evento, si es un evento de ventana se recibe en el pushEvent.
    * @param Id indica el tipo de mensaje
    * @param info puntero a void para pasar parametros
    */
    void handleEvent(std::string const& id, void* info) override;
    /**
    * @brief Metodo que se usa para inicializar el componente.
    * Garantiza que todos los componentes iniciales esten creados
    */
    /**
    * @brief Se llama cuando otro objeto colisiona con este
    * @param other El objeto con el que ha colisionado
    */
    void onCollisionEnter(GameObject* const other);
    /**
    * @brief Se llama cuando otro objeto deja de colisionar con este
    * @param other El objeto con el que se ha dejado de colisionar
    */
    void onCollisionExit(GameObject* const other);
    /**
    * @brief Se llama mientras los objetos estan colisionando
    * @param other El objeto con el que se esta colisionando
    */
    void onCollisionStay(GameObject* const other);
    /**
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * Garantiza que todos los componentes iniciales esten creados
    */
    void awake() override;
    /**
    * @brief Activa o desactiva el componente (si es 'false' no se llama a ningun metodo excepto 'receiveEvent')
    * @param b true si se quiere activar, false en caso contrario
    */
    void setActive(const bool b) override;

    /**
    * @brief Cambia el tipo de movimiento entre dinamica, estatico o cinematico
    * @param t Tipo de movimiento
    */
    void setMomeventType(const MovementType t);
    /**
    * @brief Cambia si el rb es un trigger o no
    * @param t True si se quiere hacer un trigger, false en caso contrario
    */
    void setTrigger(const bool t);

    /**
    * @brief Cambia la forma del collider 
    * @param s Forma que se quiere dar al collider
    */
    inline void setColliderShape(const ColliderShape s) { colShape = s; }
    /**
    * @brief Cambia la mascara 
    * @param m Mascara a la que se quiere cambiar
    */
    inline void setMask(const int m);
    /**
    * @brief Cambia el grupo
    * @param g Grupo al que se quiere cambiar
    */
    void setGroup(const int g);

    /**
    * @brief Cambia la masa 
    * @param m Valor que se le quiere dar a la masa
    */
    inline void setMass(const float m) { mass = m; }
    /**
    * @brief Cambia el tensor
    * @param t Valor del tensor
    */
    void setTensor(const Vector3 t);

    /**
    * @brief Cambia la friccion
    * @param f Valor que se le quiere dar a la friccion
    */
    void setFriction(const float f);
    /**
    * @brief Cambia el damping
    * @param f Valor que se le quiere dar al damping
    */
    void setDamping(const float f);
    /**
    * @brief Cambia el bote
    * @param b Valor que se le quiere dar al bote
    */
    void setBounciness(const float b);
    /**
    * @brief Cambia la velocidad
    * @param v Valor que se le quiere dar a la velocidad
    */
    void setVelocity(const Vector3 v);
    /**
    * @brief Cambia la gravedad que se aplica sobre el objeto
    * @param g Valor que se le quiere dar a la gravedad
    */
    void setGravity(const Vector3 g);

    /**
    * @brief Si se escala junto con el transform
    */
    void setTrackScale(const bool b);

    /**
    * @brief Aplica fuerza de forma gradual
    * @param f Valor de la fuerza que se quiere aplicar 
    */
    void addForce(const Vector3 f);
    /**
    * @brief Aplica solo una fuerza instantanea (golpes, explosiones...)
    * @param f Valor de la fuerza que se quiere aplicar 
    */
    void addImpulse(const Vector3 f);

    /**
    * @brief Devuelve si el rb es un trigger o no
    * @return True si es un trigger, false en caso contrario
    */
    inline bool getTrigger() const { return isTrigger; }
    /**
    * @brief Devuelve la forma del collider 
    * @return Forma del collider perteneciente a este rigidbody
    */
    inline int getColliderShape() const { return colShape; }
    /**
    * @brief Devuelve la escala inicial del collider 
    * @return Escala del collider perteneciente a este rigidbody
    */
    inline Vector3 getColliderScale() const { return colliderInitialScale; }

    /**
    * @brief Devuelve la escala verdadera del collider en bullet
    * @return Escala del collider perteneciente a este rigidbody
    */
    Vector3 getColliderTrueScale() const;

    /**
    * @brief Devuelve el tipo de movimiento (dinamica, estatico o cinematico)
    * @return Tipo de movimiento
    */
    inline int getMovementType() const { return movementType; }
    /**
    * @brief Devuelve la mascara 
    * @return Mascara de este rigidbody
    */
    inline int getMask() const { return mask; }
    /**
    * @brief Devuelve el grupo 
    * @return Grupo de este rigidbody
    */
    inline int getGroup() const { return group; }

    /**
    * @brief Devuelve la masa 
    * @return Masa del rigidbody
    */
    inline float getMass() const { return mass; }
    /**
    * @brief Devuelve el tensor de inercia 
    * @return tensor de inercia 
    */
    Vector3 getTensor() const;
    /**
    * @brief Devuelve la friccion
    * @return Friccion del rigidbody
    */
    inline float getFriction() const { return friction; }
    /**
    * @brief Devuelve el damping
    * @return Damping del rigidbody
    */
    inline float getDamping() const { return damping; }
    /**
    * @brief Devuelve el bote
    * @return Bote del rigidbody
    */
    inline float getBounciness() const { return bounciness; }
    /**
    * @brief Devuelve la velocidad linear
    * @return Velocidad del rigidbody
    */
    Vector3 getVelocity() const;

    /**
    * @brief Devuelve la velocidad angular
    * @return Velocidad angular del rigidbody
    */
    Vector3 getAngularVelocity() const;

    /**
    * @brief Devuelve la gravedad que se aplica a este objeto
    * @return Gravedad que se aplcia al rigidbody
    */
    Vector3 getGravity() const;
    /**
    * @brief Devuelve la fuerza que se aplica a este objeto
    * @return Fuerza que se aplica al rigidbody
    */
    Vector3 getTotalForce() const;
    /**
    * @brief Devuelve la velocidad de empuje
    * @return Velocidad de empuje
    */
    Vector3 getPushVelocity() const;
    /**
    * @brief Devuelve si el rigidbody esta activo
    * @return true si esta activo, false en caso contrario
    */
    inline bool getActiveRB() const { return activeRigidBody; }
};
}
