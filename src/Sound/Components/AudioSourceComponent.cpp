#include "AudioSourceComponent.h"
#include "Components/Transform.h"
#include "Sound.h"
#include "AudioSource.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"

namespace Tapioca {
AudioSourceComponent::AudioSourceComponent()
    : is3D(false), islooping(false), ispaused(true), mySound(nullptr), mySource(nullptr), myTransform(nullptr),
      volumen(0.0f), playSpeed(0.0f) { }

AudioSourceComponent::~AudioSourceComponent() {
    mySource->setPaused(true);
    delete mySource;
    mySource = nullptr;
    delete mySound;
    mySound = nullptr;
    myTransform = nullptr;
}

bool AudioSourceComponent::initComponent(const CompMap& variables) {

    //para que se pueda acceder a ellos todos los sonidos se crearan pausados
    if (!setValueFromMap(is3D, "is3D", variables)) {
        logInfo("AudioSourceComponent: No se encontro el valor de is3D. Se inicializo a false.");
    }
    if (!setValueFromMap(islooping, "islooping", variables)) {
        logInfo("AudioSourceComponent: No se encontro el valor de islooping. Se inicializo a false.");
    }
    if (!setValueFromMap(ispaused, "ispaused", variables)) {
        logInfo("AudioSourceComponent: No se encontro el valor de ispaused. Se inicializo a true.");
    }
    if (!setValueFromMap(sourcepath, "sourcepath", variables)) {
        logError("AudioSourceComponent: No se pudo inicializar el sourcepath del sonido.");
        return false;
    }
    return true;
}

void AudioSourceComponent::awake() {
    myTransform = getObject()->getComponent<Transform>();
    position = myTransform->getGlobalPosition();
    mySound = new Sound(sourcepath);
    if (is3D) {
        mySource = new AudioSource(*mySound, position, ispaused, islooping);
    }
    else {
        mySource = new AudioSource(*mySound, ispaused, islooping);
    }
}

void AudioSourceComponent::handleEvent(std::string const& id, void* info) {
    if (id == "transformchanged") {
        position = myTransform->getGlobalPosition();
        mySource->setPosition(position);
    }
}

//void AudioSourceComponent::playSound() { } en verdad con un set pause false ya suena
void AudioSourceComponent::pause(bool p) { 
    ispaused = p;
    mySource->setPaused(p);
}
void AudioSourceComponent::loop(bool l) { 
    islooping = l;
    mySource->setLooped(l);
}
void AudioSourceComponent::setVolume(float v) { 
    if (v > 1.0f) {
        v = 1.0f;
    }
    else if (v < 0.0f) {
        v = 0.0f;
    }
    volumen = v;
    mySource->setVolume(v);
       
}
}
