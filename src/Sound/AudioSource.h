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
    float maxDistance;          // Distancia a partir de la cual el sonido ya no se atenua
    float minDistance;          // Distancia a partir de la cual el sonido empieza a atenuarse
    float pan;                  // 0 es el centro , -1 la derecha  y 1 la izquierda , solo aplicable a fuentes 2D
    float playbackSpeed;        // Velocidad a la que se reproduce el sound, variarla afecta al pitch del sonido
    float currentTimeStamp;     // Segundo por el que va el sonido
    bool isLooped;              // Indica si el sonido se reproduce en bucle o no
    float volume;               // Volumen del sonido. Valor entre 0 (muteado) y 1 el maximo
    Vector3 position;           // Position del sonido (solo util para sonidos 3D)

public:
    /**
    * @brief Constructor para una fuente de sonido espacializada en 3D
    * @param sound Sonido que se va a reproducir
    * @param pos Posicion de la fuente de sonido
    * @param ispaused Indica si el sonido se reproduce pausado o no
    * @param islooped Indica si el sonido se reproduce en bucle o no
    */
    AudioSource(Sound sound, Vector3 pos, bool ispaused = true, bool islooped = false);

    /**
    * @brief Constructor para una fuente de audio 2D
    * @param sound Sonido que se va a reproducir
    * @param ispaused Indica si el sonido se reproduce pausado o no
    * @param islooped Indica si el sonido se reproduce en bucle o no
    */
    AudioSource(Sound sound, bool ispaused = true, bool islooped = false);

    /**
    * @brief Destructor vacio
    */
    ~AudioSource() {};

    /**
    * @brief Devuelve si el sonido esta pausado o no
    * @return Si el sonido esta pausado o no
    */
    bool isPaused();
    /**
    * @brief Devuelve si el sonido ha terminado de reproducirse
    * @return Si el sonido ha terminado de reproducirse
    */
    bool hasFinished();
    /**
    * @brief Devuelve si el sonido se ha detenido
    * @return Si el sonido se ha detenido
    */
    bool hasStopped();
    /**
    * @brief Establece si el sonido esta pausado o no
    * @param p Indica si el sonido esta pausado o no
    */
    void setPaused(bool p);
    /**
    * @brief Establece la distancia a partir de la cual el sonido ya no se atenua
    * @param minDist Distancia a partir de la cual el sonido empieza a atenuarse
    * @param maxDist Distancia a partir de la cual el sonido ya no se atenua
	*/
    void setAtenuation(float minDist, float maxDist);
    /**
    * @brief Establece el pan del sonido
    * @param p Pan del sonido
    */
    void setPan(float p);
    /**
    * @brief Establece la velocidad a la que se reproduce el sonido
    * @param s Velocidad a la que se reproduce el sonido
    */
    void setSpeed(float s);
    /**
    * @brief Establece si el sonido se reproduce en bucle o no
    * @param l Indica si el sonido se reproduce en bucle o no
    */
    void setLooped(bool l);
    /**
    * @brief Establece el volumen del sonido
    * @param v Volumen del sonido
    */
    void setVolume(float v);
    /**
    * @brief Establece la posicion del sonido
    * @param p Posicion del sonido
	*/
    void setPosition(Vector3 p);
    /**
    * @brief Reproduce el sonido una vez desde el principio
    * @param spatialised Indica si el sonido es 2D o 3D
    */
    void playOnce(bool spatialised);
    /**
    * @brief Reproduce el sonido en bucle desde el principio
    * @param spatialised Indica si el sonido es 2D o 3D
    */
    void playLooped(bool spatialised);
    /**
    * @brief Detiene el sonido 
    */
    void stop();
};
}
