#pragma once
#include "defs.h"
#include "Utilities/Vector3.h"
namespace irrklang { 
class ISound;
}
namespace Tapioca {
class Sound;
//class Vector3;
class TAPIOCA_API AudioSource { 
   

private:
	//Tapioca::Sound* mysoundsource;
    irrklang ::ISound* mysound;
    //bool is3D;//esto se marca al crearlo y luego no se puede cambiar  creo que ira al audiosource
    bool paused;
    //los proximos 2 campos son solo aplicables a audio 3D
    /*
   * @brief distancia a partir de la cual el sonido ya no se atenua 
   */
    float maxDistance=100000;

    /*
    * @brief distancia a partir de la cual el sonido empieza a atenuarse
    */
    float minDistance=1;
    /*
    * @brief 0 es el centro , -1 la derecha  y 1 la izquierda , solo aplicable a fuentes 2D
    */
    float pan=0;
    /*
    * @brief velocidad a la que se reproduce el sound , variarla afecta al pitch del sonido
    */
    float playbackSpeed=1;
    /*
    * @brief el segundo por el que va el sonido
    */
    float currentTimeStamp=0;

    //efectos podria ser una clase lo hago luego
    //no son prioritarios
    //ISoundEffectControl

    //Sound source es supropia clase en IrrKlang

    bool looped;
    /*
    * @brief valor entre 0 (muteado) y 1 el maximo 
    */
    float volume=1;

    Vector3 position;//solo util para sonidos 3D

public:
   // AudioSource(Sound sound, bool ispaused = true, bool islooped = false, bool load3D = false);

    /*
    * @brief  Constructora para una fuente de sonido espacializada en 3 dimensiones
    */
    AudioSource(Sound sound,  Vector3 pos, bool ispaused = true, bool islooped = false);   //construye una fuente 3D

    /*
    * @brief Constructora para una fuente de audio 2D 
    */
    AudioSource(Sound sound, bool ispaused = true, bool islooped = false);//Construye una fuente 2D
    ~AudioSource() {};
    void setPaused(bool p);
    void setAtenuation(float minDist, float maxDist);
    void setPan(float p);
    void setSpeed(float s);
    void setLooped(bool l);
    void setVolume(float v);
    void setPosition(Vector3 p);
    /*
    * @brief Reproduce el sonido una vez desde el principio.
    */
    void playOnce(bool spatialised);
    /*
    * @brief Reproduce el sonido en bucle desde el principio.
    */
    void playLooped(bool spatialised);
    /*
    * @brief detiene  el sonido 
    */
    void stop();
};
}