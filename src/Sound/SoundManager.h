#pragma once
//includes para herencia
#include "Utilities/Singleton.h"
#include "Structure/Module.h"

//includes de IrrKlang
#include "irrKlang.h"

namespace Tapioca {
class Sound;
class AudioSource;
class AudioListener;
class TAPIOCA_API SoundManager : public Singleton<SoundManager>, public Module {
    friend Singleton<SoundManager>;
    friend Sound;
    friend AudioSource;
    friend AudioListener;

private:
    Sound* snd;//para probar en testsample
    AudioSource* au; //para probar en testsample
    AudioListener* al;
    irrklang::ISoundEngine* soundEngine;

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



    //para pruebas
    void testsample();
};
}