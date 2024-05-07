#include "Sound.h"
#include "SoundManager.h"
#include "AudioSource.h"
#include <irrKlang.h>

namespace Tapioca {
Tapioca::AudioSource::AudioSource(Sound sound, Vector3 pos, bool isPaused, bool isLooped)
    : isPaused(isPaused), maxDistance(100000), minDistance(1), pan(0), playbackSpeed(1), currentTimeStamp(0),
      isLooped(isLooped), volume(0), position(pos) {
    irrklang::vec3df p(pos.x, pos.y, pos.z);
    iSound = Tapioca::SoundManager::instance()->soundEngine->play3D(sound.source, p, isLooped, true);
    if (iSound == nullptr)
        Tapioca::logError(("No se pudo cargar el sonido , no se encuentra el archivo " + sound.sourceFile).c_str());
    else
        setPaused(isPaused);
}

Tapioca::AudioSource::AudioSource(Sound sound, bool isPaused, bool isLooped)
    : isPaused(isPaused), isLooped(isLooped), maxDistance(100000), minDistance(1), pan(0), playbackSpeed(1),
      currentTimeStamp(0), volume(0), position(Vector3()) {
    iSound = Tapioca::SoundManager::instance()->soundEngine->play2D(sound.source, isLooped, true);
    if (iSound == nullptr)
        Tapioca::logError(("No se pudo cargar el sonido , no se encuentra el archivo " + sound.sourceFile).c_str());
    else
        setPaused(isPaused);
}

void Tapioca::AudioSource::setPaused(bool p) {
    if (iSound != nullptr) {
        iSound->setIsPaused(p);
        position = p;
    }
}

void Tapioca::AudioSource::setAtenuation(float minDist, float maxDist) {
    if (iSound != nullptr) {
        iSound->setMinDistance(minDist);
        minDistance = minDist;
        iSound->setMaxDistance(maxDist);
        maxDistance = maxDist;
    }
}

void Tapioca::AudioSource::setPan(float p) {
    if (iSound != nullptr) {
        iSound->setPan(p);
        pan = p;
    }
}

void Tapioca::AudioSource::setSpeed(float s) {
    if (iSound != nullptr) {
        iSound->setPlaybackSpeed(s);
        playbackSpeed = s;
    }
}

void Tapioca::AudioSource::setLooped(bool l) {
    if (iSound != nullptr) {
        iSound->setIsLooped(l);
        isLooped = l;
    }
}

void Tapioca::AudioSource::setVolume(float v) {
    if (iSound != nullptr) {
        iSound->setVolume(v);
        volume = v;
    }
}

void Tapioca::AudioSource::setPosition(Vector3 p) {
    if (iSound != nullptr) {
        position = p;
        iSound->setPosition(irrklang::vec3df(p.x, p.y, p.z));
    }
}

void Tapioca::AudioSource::playOnce(bool spatialised) {
    if (iSound != nullptr) {
        iSound->stop();
        // mysound->drop();
        // Nos aseguramos de que el audio empieza desde el principio
        iSound->setPlayPosition(0);
        setLooped(false);
        if (spatialised) {
            irrklang ::vec3df p(position.x, position.y, position.z);
            iSound =
                Tapioca::SoundManager::instance()->soundEngine->play3D(iSound->getSoundSource(), p, isLooped, true);
        }
        else {
            iSound = Tapioca::SoundManager::instance()->soundEngine->play2D(iSound->getSoundSource(), isLooped, true);
        }
        setPaused(false);
    }
}

void Tapioca::AudioSource::playLooped(bool spatialised) {
    if (iSound != nullptr) {
        iSound->stop();
        // mysound->drop();
        // Nos aseguramos de que el audio empieza desde el principio
        iSound->setPlayPosition(0);
        setLooped(true);
        if (spatialised) {
            irrklang ::vec3df p(position.x, position.y, position.z);
            iSound =
                Tapioca::SoundManager::instance()->soundEngine->play3D(iSound->getSoundSource(), p, isLooped, true);
        }
        else
            iSound = Tapioca::SoundManager::instance()->soundEngine->play2D(iSound->getSoundSource(), isLooped, true);
        setPaused(false);
    }
}

void Tapioca::AudioSource::stop() { iSound->stop(); }
}
