#pragma once
#include "defs.h"
#include <string>
namespace irrklang{
class ISoundSource;

}

namespace Tapioca {
class AudioSource;
class SoundEngine;
class TAPIOCA_API Sound {
private:
    friend AudioSource;
    friend SoundEngine;

protected:
   // irrklang::ISound* mysound;
    irrklang::ISoundSource* mysource;
   std:: string sourceFile;
   
    float length;// en milisegundos
   

public:
    Sound(std::string source);
    
    

};
}