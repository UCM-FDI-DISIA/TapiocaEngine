#pragma once
#include "Structure/Component.h"
#include "Core.h"

namespace Tapioca {
class AudioSource;
class Sound;
class Transform;
class TAPIOCA_API AudioSourceComponent: public Component {
private:
    AudioSource* source;
    Sound* mySound;
    //ruta archivo de sonido
    std::string sourcepath;


public:
    COMPONENT_ID("AudioSourceComponent")
    AudioSourceComponent();
    ~AudioSourceComponent();
    bool initComponent(const CompMap& variables) override;
    void awake() override;
    void handleEvent(std::string const& id, void* info) override;
};
}