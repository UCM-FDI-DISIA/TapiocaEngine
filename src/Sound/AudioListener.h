#pragma once
#include "defs.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class SoundManager;

/**
* @brief Clase que representa un objeto que escucha sonidos
*/
class TAPIOCA_API AudioListener {
private:
    friend SoundManager;

    Vector3 position;   // Posicion de la fuente de sonido
    Vector3 look;       // Direccion de la fuente de sonido
    Vector3 velocity;   // Velocidad de la fuente de sonido en m/s
    Vector3 up;         // Vector que indica la direccion de arriba

public:
    /**
    * @brief Crea un objeto AudioListener con los parametros dados
    * @param pos Posicion de la fuente de sonido
    * @param dir Direccion de la fuente de sonido
    * @param vel Velocidad de la fuente de sonido en m/s
    * @param up Vector que indica la direccion de arriba
    */
    AudioListener(Vector3 pos, Vector3 dir, Vector3 vel, Vector3 up);

    /**
    * @brief Establece la posicion de la fuente de sonido
    * @param p Posicion de la fuente de sonido
    */
    inline void setPosition(Vector3 p) { position = p; };

    /**
    * @brief Establece la direccion de la fuente de sonido
    * @param d Direccion de la fuente de sonido
    */
    inline void setDirection(Vector3 d) { look = d; };

    /**
    * @brief Establece la velocidad de la fuente de sonido
    * @param v Velocidad de la fuente de sonido en m/s
    */
    inline void setVelocity(Vector3 v) { velocity = v; };

    /**
    * @brief Establece el vector que indica la direccion de arriba
    * @param u Vector que indica la direccion de arriba
    */
    inline void setUp(Vector3 u) { up = u; };
};
}
