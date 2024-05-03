#pragma once
#include "Structure/Component.h"
#include <unordered_map>

namespace Tapioca {
class NodeAnimator;
class Vector3;

/**
* @brief Clase que se encarga de animar un objeto a traves de keyframes
*/
class TAPIOCA_API KeyFrameAnimator : public Component {
private:
    // Estructura con la informacion de los keyframes
    struct KeyFramesInfo {
        int size = 0;
        std::unordered_map<int, Vector3> pos;
        std::unordered_map<int, Vector3> scale;
        std::unordered_map<int, Vector3> rot;
    };
    KeyFramesInfo info;           // Informacion de los keyframes
    NodeAnimator* nodeAnimator;   // Nodo animador
    float speed;                  // Velocidad de la animacion
    float duration;               // Duracion de la animacion
    bool playByDefault;           // Indica si la animacion se reproduce por defecto
    bool loop;                    // Indica si la animacion se reproduce en bucle

public:
    COMPONENT_ID("KeyFrameAnimator")
    
    /**
    * @brief Constructor por defecto
    */
    KeyFrameAnimator();

    /**
    * @brief Destructor por defecto
    */
    ~KeyFrameAnimator();

    /**
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;
    /**
    * @brief Inicializa el componente
    * Garantiza que todos los componentes iniciales esten creados
    */
    void start() override;
    /**
    * @brief Actualiza la animacion
    * @param deltaTime Tiempo que ha pasado desde la ultima llamada
    */
    void update(uint64_t delt) override;

    /**
    * @brief Reproduce la animacion y se le indica si se reproduce en bucle
    * @param loop Indica si la animacion se reproduce en bucle
    */
    void play(const bool loop);

    /**
    * @brief Activa o desactiva la animacion
    * @param enabled Indica si se activa o desactiva la animacion
    */
    void enable(const bool enabled);

    /**
    * @brief Establece si se quiere que la animacion se reproduzca en bucle
    * @param loop Indica si la animacion se reproduce en bucle
    */
    void setLoop(const bool loop);
    /**
    * @brief Devuelve si la animacion se reproduce en bucle
    * @return true si la animacion se reproduce en bucle, false en caso contrario
    */
    bool isLooping() const;

    /**
    * @brief Devuelve si la animacion esta activada
    * @return true si la animacion esta activada, false en caso contrario
    */
    bool isEnabled() const;
    /**
    * @brief Devuelve si la animacion ha terminado
    * @return true si la animacion ha terminado, false en caso contrario
    */
    bool hasEnded() const;
};
}
