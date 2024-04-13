#include "Sound.h"
#include "SoundManager.h"
#include "AudioSource.h"
#include "irrKlang.h"


Tapioca::AudioSource::AudioSource(Sound sound, Vector3 pos , bool ispaused, bool islooped) {

	
	paused = ispaused;
     looped = islooped;
     position = pos;
     irrklang ::vec3df p(pos.x, pos.y, pos.z);
     //tienes que crearlo pausado si luego quieres acceder al el
     mysound = Tapioca::SoundManager::instance()->soundEngine->play3D(sound.mysource, p, looped, true);
     std::string mensaje = "No se pudo cargar el sonido , no se encuentra el archivo " + sound.sourceFile;
     if (mysound == nullptr) {
         Tapioca::logError(mensaje.c_str() );
     }
     else {

         setPaused(ispaused);
     }
}

Tapioca::AudioSource::AudioSource(Sound sound, bool ispaused, bool islooped) { 

    paused = ispaused;
     looped = islooped;
    //tienes que crearlo pausado si luego quieres acceder al el
     mysound = Tapioca::SoundManager::instance()->soundEngine->play2D(sound.mysource, looped, true);
     std::string mensaje = "No se pudo cargar el sonido , no se encuentra el archivo " + sound.sourceFile;
   
     if (mysound == nullptr) {
         Tapioca::logError(mensaje.c_str());
     }
     else {
         setPaused(ispaused);
     }
}

void Tapioca::AudioSource::setPaused(bool p) { 
  if (mysound != nullptr) {
        mysound->setIsPaused(p);
        paused = p;
    }
}

void Tapioca::AudioSource::setAtenuation(float minDist, float maxDist) {
    if (mysound != nullptr) {
        mysound->setMinDistance(minDist);
        minDistance = minDist;
        mysound->setMaxDistance(maxDist);
        maxDistance = maxDist;
    }
}

void Tapioca::AudioSource::setPan(float p) { 
    if (mysound != nullptr) {
        mysound->setPan(p);
        pan = p;
    }
}

void Tapioca::AudioSource::setSpeed(float s) { 
  if (mysound != nullptr) {
        mysound->setPlaybackSpeed(s);
        playbackSpeed = s;
    }
}

void Tapioca::AudioSource::setLooped(bool l) { 
   if (mysound != nullptr) {
        mysound->setIsLooped(l);
        looped = l;
    }
}

void Tapioca::AudioSource::setVolume(float v) { 
   if (mysound != nullptr) {
        mysound->setVolume(v);
        volume = v;
    }

}

void Tapioca::AudioSource::setPosition(Vector3 p) { 
   if (mysound != nullptr) {
    position = p;
    mysound->setPosition(irrklang::vec3df(p.x,p.y,p.z));
   }
    
}

