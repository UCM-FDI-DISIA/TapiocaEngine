#pragma once
//includes para herencia
#include "Utilities/Singleton.h"
#include "Structure/Module.h"

//includes de IrrKlang
#include <irrKlang.h>
namespace Tapioca {
class TAPIOCA_API SoundEngine : public Singleton<SoundEngine>, public Module {
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
    ~SoundEngine();

    //para pruebas
    void testsample();
};
}