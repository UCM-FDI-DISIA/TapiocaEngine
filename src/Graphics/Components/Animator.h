#pragma once
#include <string>
#include "Structure/Component.h"

namespace Tapioca {
class MeshRenderer;
class AnimationHelper;

class TAPIOCA_API Animator : public Component {

private:
    MeshRenderer* meshRenderer;
    AnimationHelper* anim;

    float speed;   //Velocidad a la que se actualiza la animacion

public:
    COMPONENT_ID("Animator")
    /*
    * @brief Constructor de la clase Animator
    */
    Animator();
    /*
    * @brief Destructor de la clase Animator
    */
    ~Animator();

    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;
    /*
    * @brief Metodo que se usa para inicializar el componente.
    * Garantiza que todos los componentes iniciales esten creados
    */
    void start() override;
    /*
    * @brief Metodo que se usa para actualizar el estado de un componente. Se llama todas las veces posibles
    * @param deltaTime Tiempo que ha pasado desde la ultima llamada
    */
    void update(uint64_t delt) override;

    /*
    * @brief Cambia la animacion actual
    * @param animName Animacion que se quiere comenzar a reproducir
    */
    void playAnim(std::string const& animName);

    /*
    * @brief Cambia la velocidad a la que se reproduce la animacion. Valor por defecto a 1
    * @param s Velocidad a la que se quiere cambiar
    */
    void setSpeed(const float s);
    /*
    * @brief Cambia si la animacion se repite al terminar. Valor por defecto a true
    * @param l true si se quiere repetir, false en caso contrario
    */
    void setLoop(const bool l);
    /*
    * @brief Cambia si se reproduce la animacion
    * @param l true si se quiere reproducir, false en caso contrario
    */
    void setPlaying(const bool p);

    /*
    * @brief Devuelve la velocidad a la que se reproduce la animacion
    * @return Velocidad a la que se reproduce la animacion
    */
    float getSpeed() const;
    /*
    * @brief Devuelve si la animacion esta en bucle
    * @return true si la animacion esta en bucle, false en caso contrario
    */
    bool getLoop() const;
    /*
    * @brief Devuelve si la animacion se esta reproduciendo
    * @return true si la animacion se esta reproduciendo, false en caso contrario
    */
    bool getPlaying() const;
};
}