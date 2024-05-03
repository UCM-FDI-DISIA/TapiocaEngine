#pragma once
#include "defs.h"
#include "Utilities/Vector3.h"

namespace irrklang {
class ISound;
}

namespace Tapioca {
class Sound;

/**
* @brief Clase que representa una fuente de audio, ya sea 2D o 3D
*/
class TAPIOCA_API AudioSource {
private:
    irrklang::ISound* iSound;   // Puntero al sonido que se reproduce
    bool isPaused;             // Indica si el sonido esta pausado o no
    float maxDistance;         // Distancia a partir de la cual el sonido ya no se atenua
    float minDistance;         // Distancia a partir de la cual el sonido empieza a atenuarse
    float pan;                 // 0 es el centro , -1 la derecha  y 1 la izquierda , solo aplicable a fuentes 2D
    float playbackSpeed;       // Velocidad a la que se reproduce el sound, variarla afecta al pitch del sonido
    float currentTimeStamp;    // Segundo por el que va el sonido
    bool isLooped;             // Indica si el sonido se reproduce en bucle o no
    float volume;              // Volumen del sonido. Valor entre 0 (muteado) y 1 el maximo
    Vector3 position;          // Position del sonido (solo util para sonidos 3D)

public:
    /**
    * @brief Constructor para una fuente de sonido espacializada en 3D
    */
    AudioSource(Sound sound, Vector3 pos, bool ispaused = true, bool islooped = false);   //construye una fuente 3D

    /**
    * @brief Constructor para una fuente de audio 2D
    */
    AudioSource(Sound sound, bool ispaused = true, bool islooped = false);

    /**
    * @brief Destructor vacio
    */
    ~AudioSource() {};

    void setPaused(bool p);
    void setAtenuation(float minDist, float maxDist);
    void setPan(float p);
    void setSpeed(float s);
    void setLooped(bool l);
    void setVolume(float v);
    void setPosition(Vector3 p);
    /**
    * @brief Reproduce el sonido una vez desde el principio
    * @param spatialised indica si el sonido es 3D o 2D
    */
    void playOnce(bool spatialised);
    /**
    * @brief Reproduce el sonido en bucle desde el principio
    * @param spatialised indica si el sonido es 3D o 2D
    */
    void playLooped(bool spatialised);
    /**
    * @brief Detiene el sonido 
    */
    void stop();
};
}