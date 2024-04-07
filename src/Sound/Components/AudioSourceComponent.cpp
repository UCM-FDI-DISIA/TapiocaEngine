#include "AudioSourceComponent.h"
//#include "AudioSourceComponent.h"
#include "Components/Transform.h"
#include "../Sound.h"
#include "../AudioSource.h"

Tapioca::AudioSourceComponent::AudioSourceComponent() { }

Tapioca::AudioSourceComponent::~AudioSourceComponent() {
 delete mySource;
 delete mySound;
 //delete myTransform;
}

bool Tapioca::AudioSourceComponent::initComponent(const CompMap& variables) { 
	
	//para que se pueda acceder a ellos todos los sonidos se crearan pausados
    if (!setValueFromMap(is3D, "is3D", variables)) {
            is3D = false;                         //por defecto sonido sin espacializar
    }
    if (!setValueFromMap(islooping, "islooping", variables)) {
        islooping = false;   //por defecto sonido sin bucle
    }
    if (!setValueFromMap(ispaused, "ispaused", variables)) {
        ispaused = true;   //por defecto sonido sin bucle
    }
    if (!setValueFromMap(sourcepath, "sourcepath", variables)) {
        std::cerr << "Error: AudioSourceComponent: no se pudo inicializar el sourcepath del sonido.\n";

        return false;
    }
	return true;


}

void Tapioca::AudioSourceComponent::awake() {
    myTransform = getObject()->getComponent<Transform>();
    position = myTransform->getGlobalPosition();
    mySound = new Sound(sourcepath);
    if (is3D) {
        mySource = new AudioSource(*mySound,position,ispaused,islooping);
    }
    else {
        mySource = new AudioSource(*mySound,ispaused, islooping);
   
    }

    }

void Tapioca::AudioSourceComponent::handleEvent(std::string const& id, void* info) {
        if (id == "transformchanged") {
            position = myTransform->getGlobalPosition();
            mySource->setPosition(position);
        }

}

void Tapioca::AudioSourceComponent::playSound() {

}
