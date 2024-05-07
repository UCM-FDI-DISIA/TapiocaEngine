#include "AudioSourceComponent.h"
#include "Components/Transform.h"
#include "Sound.h"
#include "AudioSource.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "checkML.h"

namespace Tapioca {
AudioSourceComponent::AudioSourceComponent()
    : source(nullptr), sound(nullptr), transform(nullptr), sourcePath(""), is3D(false), isLooping(false),
      isPaused(true), volumen(0.0f), playSpeed(0.0f) { }

AudioSourceComponent::~AudioSourceComponent() {
    if (source != nullptr) {
        source->setPaused(true);
        delete source;
        source = nullptr;
    }
    if (sound != nullptr) {
        delete sound;
        sound = nullptr;
    }
    transform = nullptr;
}

bool AudioSourceComponent::initComponent(const CompMap& variables) {
    if (!setValueFromMap(is3D, "is3D", variables)) {
        logInfo("AudioSourceComponent: No se encontro el valor de is3D. Se inicializo a false.");
    }
    if (!setValueFromMap(isLooping, "islooping", variables)) {
        logInfo("AudioSourceComponent: No se encontro el valor de islooping. Se inicializo a false.");
    }
    if (!setValueFromMap(isPaused, "ispaused", variables)) {
        logInfo("AudioSourceComponent: No se encontro el valor de ispaused. Se inicializo a true.");
    }
    if (!setValueFromMap(sourcePath, "sourcepath", variables)) {
        logError("AudioSourceComponent: No se pudo inicializar el sourcepath del sonido.");
        return false;
    }
    return true;
}

void AudioSourceComponent::awake() {
    transform = getObject()->getComponent<Transform>();
    if (transform != nullptr) {
        position = transform->getGlobalPosition();
        sound = new Sound(sourcePath);
        if (sound != nullptr) {
            if (is3D) source = new AudioSource(*sound, position, isPaused, isLooping);
            else
                source = new AudioSource(*sound, isPaused, isLooping);
        }
    }
}

void AudioSourceComponent::handleEvent(std::string const& id, void* info) {
    if (id == "posChanged") {
        position = transform->getGlobalPosition();
        source->setPosition(position);
    }
}

void AudioSourceComponent::playOnce() { source->playOnce(is3D); }

void AudioSourceComponent::playLooped() { source->playLooped(is3D); }

void AudioSourceComponent::stop() { source->stop(); }

void AudioSourceComponent::pause(bool p) {
    isPaused = p;
    source->setPaused(p);
}
void AudioSourceComponent::loop(bool l) {
    isLooping = l;
    source->setLooped(l);
}
void AudioSourceComponent::setVolume(float v) {
    if (v > 1.0f) v = 1.0f;
    else if (v < 0.0f)
        v = 0.0f;
    volumen = v;
    source->setVolume(v);
}
}
