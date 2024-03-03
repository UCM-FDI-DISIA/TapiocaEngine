#pragma once
#include "RenderObject.h"
#include <string>

namespace Ogre {
class SceneManager;
class ParticleSystem;
}

namespace Tapioca {
class Node;

class ParticleSystem : public RenderObject {
protected:
    Ogre::ParticleSystem* mParticleSystem;

    /*
    * @brief Constructora.Solo se podra crear un ParticleSystem a partir de un sistema ya creado en un.particle
    */ 
    ParticleSystem(Ogre::SceneManager* scnMgr, Tapioca::Node* node, std::string name, std::string materialName, bool emitting = false);

public:
    virtual ~ParticleSystem() { }

    void setEmitting(bool emitting);
    bool isEmitting();

    void fastForward(float time, float interval = 0.1f);
};
}