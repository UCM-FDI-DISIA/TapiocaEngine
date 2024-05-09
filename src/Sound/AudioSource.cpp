#include "Sound.h"
#include "SoundManager.h"
#include "AudioSource.h"
#include <irrKlang.h>

namespace Tapioca {
AudioSource::AudioSource(Sound sound, Vector3 pos, bool isPaused, bool isLooped)
    : maxDistance(100000), minDistance(1), pan(0), playbackSpeed(1), currentTimeStamp(0), isLooped(isLooped), volume(0),
      position(pos) {
    irrklang::vec3df p(pos.x, pos.y, pos.z);
    iSound = SoundManager::instance()->soundEngine->play3D(sound.source, p, isLooped, true);
    if (iSound == nullptr) {
        logError(("No se pudo cargar el sonido , no se encuentra el archivo " + sound.sourceFile).c_str());
    }
    else
        setPaused(isPaused);
}

AudioSource::AudioSource(Sound sound, bool isPaused, bool isLooped)
    : isLooped(isLooped), maxDistance(100000), minDistance(1), pan(0), playbackSpeed(1), currentTimeStamp(0), volume(0),
      position(Vector3()) {
    iSound = SoundManager::instance()->soundEngine->play2D(sound.source, isLooped, true);
    if (iSound == nullptr) {
        logError(("No se pudo cargar el sonido , no se encuentra el archivo " + sound.sourceFile).c_str());
    }
    else
        setPaused(isPaused);
}

void AudioSource::setPaused(bool p) {
    if (iSound != nullptr) {
        iSound->setIsPaused(p);
        position = p;
    }
}

void AudioSource::setAtenuation(float minDist, float maxDist) {
    if (iSound != nullptr) {
        iSound->setMinDistance(minDist);
        minDistance = minDist;
        iSound->setMaxDistance(maxDist);
        maxDistance = maxDist;
    }
}

void AudioSource::setPan(float p) {
    if (iSound != nullptr) {
        iSound->setPan(p);
        pan = p;
    }
}

void AudioSource::setSpeed(float s) {
    if (iSound != nullptr) {
        iSound->setPlaybackSpeed(s);
        playbackSpeed = s;
    }
}

void AudioSource::setLooped(bool l) {
    if (iSound != nullptr) {
        iSound->setIsLooped(l);
        isLooped = l;
    }
}

void AudioSource::setVolume(float v) {
    if (iSound != nullptr) {
        iSound->setVolume(v);
        volume = v;
    }
}

void AudioSource::setPosition(Vector3 p) {
    if (iSound != nullptr) {
        position = p;
        iSound->setPosition(irrklang::vec3df(p.x, p.y, p.z));
    }
}

void AudioSource::playOnce(bool spatialised) {
    if (iSound != nullptr) {
        iSound->stop();
        // Nos aseguramos de que el audio empieza desde el principio
        iSound->setPlayPosition(0);
        setLooped(false);
        if (spatialised) {
            irrklang::vec3df p(position.x, position.y, position.z);
            iSound = SoundManager::instance()->soundEngine->play3D(iSound->getSoundSource(), p, isLooped, true);
        }
        else
            iSound = SoundManager::instance()->soundEngine->play2D(iSound->getSoundSource(), isLooped, true);

        setPaused(false);
    }
}

bool AudioSource::isPaused() {
    if (iSound != nullptr) return iSound->getIsPaused();
}

bool AudioSource::hasFinished() {
    if (iSound != nullptr) return iSound->isFinished();
}

bool AudioSource::hasStopped() {
    if (iSound != nullptr) return iSound->getIsPaused() || iSound->isFinished();
}

void AudioSource::playLooped(bool spatialised) {
    if (iSound != nullptr) {
        iSound->stop();
        // Nos aseguramos de que el audio empieza desde el principio
        iSound->setPlayPosition(0);
        setLooped(true);
        if (spatialised) {
            irrklang::vec3df p(position.x, position.y, position.z);
            iSound = SoundManager::instance()->soundEngine->play3D(iSound->getSoundSource(), p, isLooped, true);
        }
        else
            iSound = SoundManager::instance()->soundEngine->play2D(iSound->getSoundSource(), isLooped, true);

        setPaused(false);
    }
}

void AudioSource::stop() { iSound->stop(); }
}
