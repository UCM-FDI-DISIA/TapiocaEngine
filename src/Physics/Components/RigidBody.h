#pragma once

#pragma region includes
#include "Structure/Component.h"
#include "Utilities/Vector3.h"
#include "../Physics_def.h"

class btRigidBody;

#pragma endregion

namespace Tapioca {
//class RigidBodyBuilder;
class Vector3;
class Transform;
//class Collider;

class TAPIOCA_API RigidBody : public Component {
    //friend RigidBodyBuilder;

private:
    btRigidBody* rigidBody;
    Transform* transform;
    // Collider* collider;
    ColliderShape colShape;
    Vector3 colliderScale;
    MovementType movementType;
    bool isTrigger;
    //indica con que grupo se puede colisionar (en binario)
    int mask;
    //mi grupo/filtro de colision
    int group;

    float mass;
    float friction;
    float damping;
    float bounciness;

    bool activeRigidBody;

public:
    COMPONENT_ID("RigidBody");
    /*
    * @brief Constructora de la clase RigidBody.
    */
    RigidBody();
    /*
    * @brief Destructora de la clase RigidBody.
    */
    ~RigidBody();


    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;
    /*
    * @brief Metodo que se usa para actualizar el estado de un componente
    * Se llama cada cierto tiempo fijo (Game::FIXED_DELTA_TIME),
    */
    void fixedUpdate() override;
    /*
    * @brief Metodo que se usa para actualizar el estado de un componente
    * Se llama cada cierto tiempo ,
    */
    void update(const uint64_t deltaTime) override;
    /*
    * @brief Metodo que se usa para recibir eventos.
    * Se llama nada mas se recibe el evento, si es un evento de ventana se recibe en el pushEvent.
    * @param Id indica el tipo de mensaje
    * @param info puntero a void para pasar parametros
    */
    void handleEvent(std::string const& id, void* info) override;
    /*
    * @brief Metodo que se usa para inicializar el componente.
    * Garantiza que todos los componentes iniciales esten creados
    */
    /*
    * @brief Se llama cuando otro objeto colisiona con este
    * @param other El objeto con el que ha colisionado
    */
    void onCollisionEnter(GameObject* const other);
    /*
    * @brief Se llama cuando otro objeto deja de colisionar con este
    * @param other El objeto con el que se ha dejado de colisionar
    */
    void onCollisionExit(GameObject* const other);
    /*
    * @brief Se llama mientras los objetos estan colisionando
    * @param other El objeto con el que se esta colisionando
    */
    void onCollisionStay(GameObject* const other);

    void awake() override;
    /*
    * @brief Activa o desactiva el componente (si es 'false' no se llama a ningun metodo excepto 'receiveEvent')
    * @param b true si se quiere activar, false en caso contrario
    */
    void setActive(const bool b) override;

    /*
    * @brief Cambia el tipo de movimiento entre dinamica, estatico o cinematico
    * @param t Tipo de movimiento
    */
    void setMomeventType(const MovementType t);
    /*
    * @brief Cambia si el rb es un trigger o no
    * @param t True si se quiere hacer un trigger, false en caso contrario
    */
    void setTrigger(const bool t);
    /*
    * @brief Cambia la escala del collider 
    * @param s Escala que se quiere dar al collider
    */
    void setColliderScale(const Vector3 s);
    /*
    * @brief Cambia la forma del collider 
    * @param s Forma que se quiere dar al collider
    */
    void setColliderShape(const ColliderShape s);
    /*
    * @brief Cambia la mascara 
    * @param m Mascara a la que se quiere cambiar
    */
    void setMask(const int m);
    /*
    * @brief Cambia el grupo
    * @param g Grupo al que se quiere cambiar
    */
    void setGroup(const int g);

    /*
    * @brief Cambia la masa 
    * @param m Valor que se le quiere dar a la masa
    */
    void setMass(const float m);
    /*
    * @brief Cambia el tensor
    * @param t Valor del tensor
    */
    void setTensor(const Vector3 t);
    /*
    * @brief Cambia la friccion
    * @param f Valor que se le quiere dar a la friccion
    */
    void setFriction(const float f);
    /*
    * @brief Cambia el damping
    * @param f Valor que se le quiere dar al damping
    */
    void setDamping(const float f);
    /*
    * @brief Cambia el bote
    * @param b Valor que se le quiere dar al bote
    */
    void setBounciness(const float b);
    /*
    * @brief Cambia la velocidad
    * @param v Valor que se le quiere dar a la velocidad
    */
    void setVelocity(const Vector3 v);
    /*
    * @brief Cambia la gravedad que se aplica sobre el objeto
    * @param g Valor que se le quiere dar a la gravedad
    */
    void setGravity(const Vector3 g);
    /*
    * @brief Aplica fuerza de forma gradual
    * @param f Valor de la fuerza que se quiere aplicar 
    */
    void addForce(const Vector3 f);
    /*
    * @brief Aplica solo una fuerza instantanea (golpes, explosiones...)
    * @param f Valor de la fuerza que se quiere aplicar 
    */
    void addImpulse(const Vector3 f);

    /*
    * @brief Devuelve si el rb es un trigger o no
    * @return True si es un trigger, false en caso contrario
    */
    bool getTrigger() const;
    /*
    * @brief Devuelve la forma del collider 
    * @return Forma del collider perteneciente a este rigidbody
    */
    int getColliderShape() const;
    /*
    * @brief Devuelve la escala del collider 
    * @return Escala del collider perteneciente a este rigidbody
    */
    Vector3 getColliderScale() const;
    /*
    * @brief Devuelve el tipo de movimiento (dinamica, estatico o cinematico)
    * @return Tipo de movimiento
    */
    int getMovementType() const;
    /*
    * @brief Devuelve la mascara 
    * @return Mascara de este rigidbody
    */
    int getMask() const;
    /*
    * @brief Devuelve el grupo 
    * @return Grupo de este rigidbody
    */
    int getGroup() const;

    /*
    * @brief Devuelve la masa 
    * @return Masa del rigidbody
    */
    float getMass() const;
    /*
    * @brief Devuelve el tensor de inercia 
    * @return tensor de inercia 
    */
    Vector3 getTensor() const;
    /*
    * @brief Devuelve la friccion
    * @return Friccion del rigidbody
    */
    float getFriction() const;
    /*
    * @brief Devuelve el damping
    * @return Damping del rigidbody
    */
    float getDamping() const;
    /*
    * @brief Devuelve el bote
    * @return Bote del rigidbody
    */
    float getBounciness() const;
    /*
    * @brief Devuelve la velocidad
    * @return Velocidad del rigidbody
    */
    Vector3 getVelocity() const;
    /*
    * @brief Devuelve la gravedad que se aplica a este objeto
    * @return Gravedad que se aplcia al rigidbody
    */
    Vector3 getGravity() const;


    bool getActiveRB() const;
};


}