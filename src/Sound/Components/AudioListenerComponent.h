#pragma once

#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class AudioListener;
class Transform;
class TAPIOCA_API AudioListenerComponent : public Component {
private:
    AudioListener* listener;
    Transform* myTransform;
    Vector3 position; // apartir del transform
    Vector3 look; //vector forward del transform
    Vector3 velocity;   //en metros por segundo
    Vector3 up; //vector up deltransform

  

public:
    COMPONENT_ID("AudioListenerComponent");

    AudioListenerComponent();

    ~AudioListenerComponent();

    bool initComponent(const CompMap& variables) override;
    //crear el audio listener y el transfor aqui 
    void awake() override;

    void handleEvent(std::string const& id, void* info) override;
    /*
    * @brief  Establece la velocidad a la qeu se mueve el listener para calcular el doppler
    */
    void setVelocity(Vector3 v);
    /*
    * @brief sincroniza los valores del Listener con su transform y actualiza el motor de audion
    */
    void updateData();  
    
};
}