#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class AudioSource;
class Sound;
class Transform;

/**
* @brief Componente que se encarga de reproducir sonidos en el juego
*/
class TAPIOCA_API AudioSourceComponent: public Component {
private:
    AudioSource* source;      // Fuente de audio
    Sound* sound;             // Sonido a reproducir
    Transform* transform;     // Transform del objeto
    Vector3 position;         // Posicion de la fuente de audio
    
    // warning C4251 'Tapioca::AudioSourceComponent::sourcepath' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita tener una interfaz DLL
    // para que la utilicen los clientes de class 'Tapioca::AudioSourceComponent'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string sourcePath;         //ruta archivo de sonido
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif

    bool is3D; // Indica si queremos una fuente de sonido espacializada o no
    bool isLooping;
    bool isPaused;
    float volumen;
    float playSpeed;

public:
    COMPONENT_ID("AudioSourceComponent")

    /**
    * @brief Constructor por defecto
    */
    AudioSourceComponent();

    /**
    * @brief Destructor por defecto
    */
    ~AudioSourceComponent();

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
    * @brief Reproduce el sonido una vez
    */
    void playOnce();
    /**
    * @brief Reproduce el sonido en bucle
    */
    void playLooped();
    /**
    * @brief Para la reproduccion del sonido
    */
    void stop();
    /**
    * @brief Pausa o reanuda la reproduccion del sonido
    * @param p Indica si se pausa o reanuda la reproduccion
    */
    void pause(bool p);
    /**
    * @brief Indica si el sonido se reproduce en bucle o no
    * @param l Indica si el sonido se reproduce en bucle o no
    */
    void loop(bool l);
    /**
    * @brief Establece el volumen de reproduccion del sonido
    * @param v Volumen de reproduccion del sonido
    */
    void setVolume(float v);
};
}
