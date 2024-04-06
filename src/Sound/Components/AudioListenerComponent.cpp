#include "AudioListenerComponent.h"
#include "../AudioListener.h"
#include "Components/Transform.h"
#include "Structure/GameObject.h"
#include "../SoundManager.h"

namespace Tapioca {
AudioListenerComponent::AudioListenerComponent() { }

AudioListenerComponent::~AudioListenerComponent() {

	delete listener;
}

bool AudioListenerComponent::initComponent(const CompMap& variables) { 
	//3 opciones para paramentros
	// es obligatorio envia un error si no lo han puesto en el lua 
	//Es un parametro opcional si no esta no pasa nada pones uno por defecto
    if (!setValueFromMap(velocity.x, "velocityX", variables))
         velocity.x = 0;// si no se indica velocidad en el lua se asume que esta quieto
    if (!setValueFromMap(velocity.y, "velocityY", variables))
        velocity.y = 0;   // si no se indica velocidad en el lua se asume que esta quieto
    if (!setValueFromMap(velocity.z, "velocityZ", variables))
        velocity.z = 0;   // si no se indica velocidad en el lua se asume que esta quieto
	//los valores se guardan en variables en el init y es en el awake donde se setean las cosas
	return true;
}
void AudioListenerComponent::awake() { 
	myTransform = getObject()->getComponent<Transform>();
    position = myTransform->getGlobalPosition();
    look = myTransform->forward();
    up = myTransform->up();
    listener = new AudioListener(position,look,velocity,up);
    SoundManager::instance()->setListener(*listener);
		

}
void AudioListenerComponent::handleEvent(std::string const& id, void* info) { 
	if (id == "transformchanged")
	{
        position = myTransform->getGlobalPosition();
        listener->setPosition(position);
        look = myTransform->forward();
        listener->setDirection(look);
        up = myTransform->up();
        listener->setUp(up);
        SoundManager::instance()->setListener(*listener);
	}
}
void AudioListenerComponent::setVelocity(Vector3 v) { 
 
	velocity = v;
    listener->setVelocity(velocity);
    SoundManager::instance()->setListener(*listener);

}
	
}