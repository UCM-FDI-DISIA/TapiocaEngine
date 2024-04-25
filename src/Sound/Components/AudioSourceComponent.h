#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class AudioSource;
class Sound;
class Transform;

/*
*/
class TAPIOCA_API AudioSourceComponent: public Component {
private:
    //envoltorios 
    AudioSource* mySource;
    Sound* mySound;
    //info de posicion etc
    Vector3 position;
    Transform* myTransform;
    //ruta archivo de sonido
    std::string sourcepath;

    bool is3D;//Se sabe al leer desde dua si qeuremos una fuente espacializada o no

    bool islooping;

    bool ispaused;

    float volumen;

    float playSpeed;


public:
    COMPONENT_ID("AudioSourceComponent")
    AudioSourceComponent();
    ~AudioSourceComponent();
    bool initComponent(const CompMap& variables) override;
    void awake() override;
    void start() override;
    void handleEvent(std::string const& id, void* info) override;
    void playOnce();
    void pause(bool p);
    void loop(bool l);
    void setVolume(float v);
};
}