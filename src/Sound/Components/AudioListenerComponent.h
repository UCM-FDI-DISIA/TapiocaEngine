#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class SoundManager;
class AudioListener;
class Transform;

/**
* @brief Componente que se encarga de manejar el listener de audio
*/
class TAPIOCA_API AudioListenerComponent : public Component {
private:
    SoundManager* soundManager;   // Puntero al SoundManager
    AudioListener* listener;      // Listener de audio
    Transform* transform;         // Transform del objeto
    Vector3 position;             // Posicion del transform
    Vector3 look;                 // Vector forward del transform
    Vector3 velocity;             // Velocidad en m/s
    Vector3 up;                   // Vector up del transform

public:
    COMPONENT_ID("AudioListenerComponent");

    /**
    * @brief Constructor por defecto
    */
    AudioListenerComponent();

    /**
    * @brief Destructor por defecto
    */
    ~AudioListenerComponent();

    /**
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;
    /**
    * @brief Inicializa el componente. Se ejecuta antes que el start
    * Garantiza que todos los componentes iniciales esten creados
    */
    void awake() override;
    /**
    * @brief Recibe eventos. Se llama nada mas se recibe el evento, si es un evento de ventana se recibe en el pushEvent
    * @param id Indica el tipo de mensaje
    * @param info Puntero a void para pasar parametros
    */
    void handleEvent(std::string const& id, void* info) override;
    /**
    * @brief Establece la velocidad a la que se mueve el listener para calcular el doppler
    */
    void setVelocity(Vector3 v);
};
}
