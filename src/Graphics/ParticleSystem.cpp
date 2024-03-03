#include "ParticleSystem.h"
#include <Ogre.h>

namespace Tapioca {

ParticleSystem::ParticleSystem(Ogre::SceneManager* scnMgr, Node* node, std::string name, std::string materialName, bool emitting)
    : RenderObject(node), mParticleSystem(scnMgr->createParticleSystem(name, materialName))
{
    init(mParticleSystem);
    mParticleSystem->setEmitting(emitting);
}

void ParticleSystem::setEmitting(bool emitting) { mParticleSystem->setEmitting(emitting); }
bool ParticleSystem::isEmitting() { return mParticleSystem->getEmitting(); }

void ParticleSystem::fastForward(float time, float interval) { mParticleSystem->fastForward(time, interval); }


}
