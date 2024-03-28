#pragma once
#include "Utilities/Singleton.h"
#include "Structure/Module.h"

namespace irrklang {
class ISoundEngine;
}

namespace Tapioca {
class Sound;
class AudioSource;
class AudioListener;

/*
* @brief Clase que gestiona el audio del juego
*/
class TAPIOCA_API SoundManager : public Singleton<SoundManager>, public Module {
    friend Singleton<SoundManager>;
    friend Sound;
    friend AudioSource;
    friend AudioListener;

private:
    AudioListener* al;
    irrklang::ISoundEngine* soundEngine;

    // TEMPORAL
    Sound* snd;
    AudioSource* au;

    /*
    * @brief Constructora de la clase SoundManager
    */
    SoundManager();

public:
    /*
    * @brief Destructora de la clase SoundManager
    */
    ~SoundManager();

    /*
    * @brief crea un irrKlang device para gestionarlo todo
    */
    bool init() override;

    /*
    * @brief Actualiza el audioListener
    * @params al AudioListener al que se quiere actualizar
    */
    void setListener(AudioListener al);

    // TEMPORAL
    void testsample();
};
}
