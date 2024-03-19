#include "ParticleSystem.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

namespace Tapioca {

ParticleSystem::ParticleSystem(Ogre::SceneManager* const scnMgr, RenderNode* const node, std::string const& name,
                               std::string const& templateName, const bool emitting)
    : RenderObject(node, scnMgr), mParticleSystem(scnMgr->createParticleSystem(name, templateName)) {
    init(mParticleSystem);
    mParticleSystem->setEmitting(emitting);
}

void ParticleSystem::setEmitting(const bool emitting) { mParticleSystem->setEmitting(emitting); }
bool ParticleSystem::isEmitting() { return mParticleSystem->getEmitting(); }

void ParticleSystem::fastForward(const float time, const float interval) {
    mParticleSystem->fastForward(time, interval);
}
}
