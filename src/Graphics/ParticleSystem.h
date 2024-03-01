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

    ParticleSystem(Ogre::SceneManager* scnMgr, Tapioca::Node* node, std::string name, std::string materialName, size_t quota = 500, bool emitting = false);
    ParticleSystem(Ogre::SceneManager* scnMgr, Tapioca::Node* node, std::string name, size_t quota = 500, bool emitting = false);

    void commonInit(size_t quota, bool emitting);

public:
    virtual ~ParticleSystem() { }

    void setEmitting(bool emitting);
    void setParticleQuota(size_t quota);
};
}