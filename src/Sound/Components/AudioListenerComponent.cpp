#include "AudioListenerComponent.h"
#include "AudioListener.h"
#include "Components/Transform.h"
#include "Structure/GameObject.h"
#include "SoundManager.h"

namespace Tapioca {
AudioListenerComponent::AudioListenerComponent()
    : soundManager(nullptr), listener(nullptr), transform(nullptr), position(Vector3()), look(Vector3()),
      velocity(Vector3()), up(Vector3()) { }

AudioListenerComponent::~AudioListenerComponent() {
    if (listener != nullptr) delete listener;
    listener = nullptr;
    transform = nullptr;
}

bool AudioListenerComponent::initComponent(const CompMap& variables) {
    if (!setValueFromMap(velocity.x, "velocityX", variables)) {
        logInfo(
            ("AudioListenerComponent: No se encontro el valor de velocityX. Se inicializo al valor predefinido: \"" +
             std::to_string(velocity.x) + "\".")
                .c_str());
    }
    if (!setValueFromMap(velocity.y, "velocityY", variables)) {
        logInfo(
            ("AudioListenerComponent: No se encontro el valor de velocityX. Se inicializo al valor predefinido: \"" +
             std::to_string(velocity.y) + "\".")
                .c_str());
    }
    if (!setValueFromMap(velocity.z, "velocityZ", variables)) {
        logInfo(
            ("AudioListenerComponent: No se encontro el valor de velocityX. Se inicializo al valor predefinido: \"" +
             std::to_string(velocity.z) + "\".")
                .c_str());
    }
    return true;
}
void AudioListenerComponent::awake() {
    transform = getObject()->getComponent<Transform>();
    if (transform != nullptr) {
        position = transform->getGlobalPosition();
        look = transform->forward();
        up = transform->up();
    }
    listener = new AudioListener(position, look, velocity, up);
    soundManager = SoundManager::instance();
    if (soundManager != nullptr) soundManager->setListener(*listener);
}
void AudioListenerComponent::handleEvent(std::string const& id, void* info) {
    if (id == "posChanged") {
        if (transform != nullptr) position = transform->getGlobalPosition();
        if (soundManager != nullptr) soundManager->setListener(*listener);
    }
    else if (id == "rotChanged") {
        if (transform != nullptr) {
            look = transform->forward();
            listener->setDirection(look);
            up = transform->up();
            listener->setUp(up);
        }
        if (soundManager != nullptr) soundManager->setListener(*listener);
    }
}

void AudioListenerComponent::setVelocity(Vector3 v) {
    velocity = v;
    if (listener != nullptr) {
        listener->setVelocity(velocity);
        if (soundManager != nullptr) soundManager->setListener(*listener);
    }
}
}
