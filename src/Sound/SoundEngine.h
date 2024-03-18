#pragma once
//includes para herencia
#include "../Core/Utilities/Singleton.h"
#include "../Core/Structure/Module.h"

//includes de IrrKlang
#include "../../Dependencies/IrrKlang/include/irrKlang.h"

namespace Tapioca {
class Sound;
class AudioSource;
class AudioListener;
class TAPIOCA_API SoundEngine : public Singleton<SoundEngine>, public Module {

private:
    friend Sound;
    friend AudioSource;
    friend AudioListener;
    Sound* snd;//para probar en testsample
    AudioSource* au; //para probar en testsample
    AudioListener* al;
    irrklang::ISoundEngine* soundEngine;

public:
    /*
    * @brief Constructora de la clase SoundEngine
    */
    SoundEngine();
    /*
    * @brief crea un irrKlang device para gestionarlo todo
    */
    bool init() override;
    /*
    * @brief Destructora de la clase SoundEngine
    */

    void setListener(AudioListener al);
    ~SoundEngine();



    //para pruebas
    void testsample();
};
}