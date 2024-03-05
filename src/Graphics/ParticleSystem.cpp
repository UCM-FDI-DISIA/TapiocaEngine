#include "ParticleSystem.h"
#include <Ogre.h>

namespace Tapioca {

ParticleSystem::ParticleSystem(Ogre::SceneManager* scnMgr, Node* node, std::string const& name,
                               std::string const& templateName, bool emitting)
    : RenderObject(node, scnMgr), mParticleSystem(scnMgr->createParticleSystem(name, templateName))
{
    Ogre::Billboard* bb;
    Ogre::BillboardSet* bbs;
    init(mParticleSystem);
    mParticleSystem->setEmitting(emitting);
}

void ParticleSystem::setEmitting(bool emitting) { mParticleSystem->setEmitting(emitting); }
bool ParticleSystem::isEmitting() { return mParticleSystem->getEmitting(); }

void ParticleSystem::fastForward(float time, float interval) { mParticleSystem->fastForward(time, interval); }
}
