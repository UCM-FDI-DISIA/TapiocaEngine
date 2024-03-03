#include "ParticleSystem.h"
#include <Ogre.h>

namespace Tapioca {

ParticleSystem::ParticleSystem(
    Ogre::SceneManager* scnMgr, Node* node, std::string name, std::string templateName, bool emitting)
    : RenderObject(node, scnMgr), mParticleSystem(scnMgr->createParticleSystem(name, templateName)) {
    init(mParticleSystem);
    mParticleSystem->setEmitting(emitting);
}

void ParticleSystem::setEmitting(bool emitting) { mParticleSystem->setEmitting(emitting); }
bool ParticleSystem::isEmitting() { return mParticleSystem->getEmitting(); }

void ParticleSystem::fastForward(float time, float interval) { mParticleSystem->fastForward(time, interval); }
}
