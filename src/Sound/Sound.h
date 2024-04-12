#pragma once
#include "defs.h"
#include <string>

namespace irrklang{
class ISoundSource;
}

namespace Tapioca {
class AudioSource;
class SoundManager;

/*
*/
class TAPIOCA_API Sound {
private:
    friend AudioSource;
    friend SoundManager;

protected:
   // irrklang::ISound* mysound;
    irrklang::ISoundSource* mysource;
    // warning C4251 'Tapioca::Sound::sourceFile' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita
    // tener una interfaz DLL para que la utilicen los clientes de class 'Tapioca::Sound'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string sourceFile;
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
   
    unsigned int length; // en milisegundos
   

public:
    Sound(std::string source);
    
    

};
}