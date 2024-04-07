#include "AudioSourceComponent.h"
//#include "AudioSourceComponent.h"
#include "Components/Transform.h"
#include "../Sound.h"
#include "../AudioSource.h"

Tapioca::AudioSourceComponent::AudioSourceComponent()
    : is3D(false), islooping(false), ispaused(true), mySound(nullptr), mySource(nullptr), myTransform(nullptr), playSpeed() { }

Tapioca::AudioSourceComponent::~AudioSourceComponent() {
    delete mySource;
    mySource = nullptr;
    delete mySound;
    mySound = nullptr;
    myTransform = nullptr;
}

bool Tapioca::AudioSourceComponent::initComponent(const CompMap& variables) {

    //para que se pueda acceder a ellos todos los sonidos se crearan pausados
    if (!setValueFromMap(is3D, "is3D", variables)) {
#ifdef _DEBUG
        std::cout << "AudioSourceComponent: no se encontro el valor de is3D. Se inicializo a false\n";
#endif
    }
    if (!setValueFromMap(islooping, "islooping", variables)) {
#ifdef _DEBUG
		std::cout << "AudioSourceComponent: no se encontro el valor de islooping. Se inicializo a false\n";
#endif
    }
    if (!setValueFromMap(ispaused, "ispaused", variables)) {
#ifdef _DEBUG
        std::cout << "AudioSourceComponent: no se encontro el valor de ispaused. Se inicializo a true\n";
#endif
    }
    if (!setValueFromMap(sourcepath, "sourcepath", variables)) {
#ifdef _DEBUG
        std::cerr << "Error: AudioSourceComponent: no se pudo inicializar el sourcepath del sonido.\n";
#endif
        return false;
    }
    return true;
}

void Tapioca::AudioSourceComponent::awake() {
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

void Tapioca::AudioSourceComponent::handleEvent(std::string const& id, void* info) {
    if (id == "transformchanged") {
        position = myTransform->getGlobalPosition();
        mySource->setPosition(position);
    }
}

void Tapioca::AudioSourceComponent::playSound() { }
