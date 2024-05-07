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

/**
* @brief Clase que gestiona el audio del juego
*/
class TAPIOCA_API SoundManager : public Singleton<SoundManager>, public Module {
    friend Singleton<SoundManager>;
    friend Sound;
    friend AudioSource;
    friend AudioListener;

private:
    AudioListener* al;                     // Puntero al AudioListener
    irrklang::ISoundEngine* soundEngine;   // Puntero al ISoundEngine de irrKlang

    // warning C4251 'Tapioca::SoundManager::audioFolderPath' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita tener una interfaz DLL
    // para que la utilicen los clientes de class 'Tapioca::SoundManager'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string audioFolderPath;   // Donde se buscan los sonidos
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

    /**
    * @brief Constructor por defecto
    */
    SoundManager();

public:
    /**
    * @brief Destructor por defecto
    */
    ~SoundManager();

    /**
    * @brief Crea un irrKlang device para gestionarlo todo
    */
    bool init() override;

    /**
    * @brief Actualiza el audioListener
    * @param al AudioListener al que se quiere actualizar
    */
    void setListener(AudioListener al);

    /*
    * @brief comprueba si existe la carpeta audio dentro d eassets
    */
    bool checkAudioFolder() const;

    /*
    * @brief crea la carpeta audio dentro de assets
    */
    void createAudioFolder();

    /*
    * @brief devuelve la carpeta a partir de la cual se buscan los sonidos
    */
    std::string getAudioPath();
};
}
