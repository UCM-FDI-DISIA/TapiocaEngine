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
    SoundEngine();
    /*
    * @brief crea un irrKlang device para gestionarlo todo
    */
    bool init() override;
    ~SoundEngine();

//para pruebas 
void   testsample();



};
}