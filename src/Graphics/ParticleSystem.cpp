#include "ParticleSystem.h"

#include <Ogre.h>


Tapioca::ParticleSystem::ParticleSystem(
    Ogre::SceneManager* scnMgr, Node* node, std::string name, std::string materialName, size_t quota, bool emitting)
    : RenderObject(node), mParticleSystem(scnMgr->createParticleSystem(name, materialName))
{
    commonInit(quota, emitting);
}

Tapioca::ParticleSystem::ParticleSystem(Ogre::SceneManager* scnMgr, Tapioca::Node* node, std::string name, size_t quota, bool emitting)
    : RenderObject(node), mParticleSystem(scnMgr->createParticleSystem(name, quota)) 
{ 
    commonInit(quota, emitting);
}

void Tapioca::ParticleSystem::commonInit(size_t quota, bool emitting) { 
    init(mParticleSystem);
    mParticleSystem->setEmitting(emitting);
    mParticleSystem->setParticleQuota(quota);
}


void Tapioca::ParticleSystem::setEmitting(bool emitting) { mParticleSystem->setEmitting(emitting); }

void Tapioca::ParticleSystem::setParticleQuota(size_t quota) { mParticleSystem->setParticleQuota(quota); }
