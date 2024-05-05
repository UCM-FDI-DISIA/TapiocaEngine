#pragma once
#include "defs.h"
#include <string>

namespace irrklang {
class ISoundSource;
class ISound;
}

namespace Tapioca {
class AudioSource;
class SoundManager;

/**
* @brief Clase que representa un recurso de sonido
*/
class TAPIOCA_API Sound {
private:
    friend AudioSource;
    friend SoundManager;

protected:
    SoundManager* soundManager;       // Puntero al SoundManager
    irrklang::ISoundSource* source;   // Fuente de sonido

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

    unsigned int length;   // Duracion del sonido en milisegundos

public:
    /**
    * @brief Constructor por defecto
    * @param sourceFile Ruta del archivo de sonido
    */
    Sound(std::string sourceFile);
};
}
