#pragma once
#include "Structure/Component.h"

namespace Tapioca {
class ColliderBuilder;
class GameObject;

class TAPIOCA_API Collider : public Component {
    friend ColliderBuilder;

public:
    COMPONENT_ID("Collider");
    /*
    * @brief Constructora de la clase Collider.
    */
    Collider();
    /*
    * @brief Destructora de la clase Collider.
    */
    ~Collider() {};

    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override { return true; };

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
};
}