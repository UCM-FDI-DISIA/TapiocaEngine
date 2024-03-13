#include "AnimationHelper.h"
#include "Mesh.h"
#include <Ogre.h>

namespace Tapioca {
AnimationHelper::AnimationHelper(Ogre::SceneManager* const scnMgr, Mesh* const object, const bool autoPlay = true,
                                 const bool loop = true)
    : animState(nullptr), playing(autoPlay), looping(loop) {
    //animStateSet = scnMgr->getEntity(object->getHandler())->getAllAnimationStates();
    animStateSet = object->getMesh()->getAllAnimationStates();
}

AnimationHelper::~AnimationHelper() { }

void AnimationHelper::updateAnim(const uint64_t delt, const float speed) {
    if (animState != nullptr) {
        // Actualiza la animacion
        animState->addTime(delt * speed);
    }
}

void AnimationHelper::playAnim(std::string const& anim) {
    animState = animStateSet->getAnimationState(anim);
    animState->setEnabled(playing);
    animState->setLoop(looping);
    animState->setTimePosition(0);
}

void AnimationHelper::setLoop(const bool l) {
    looping = l;
    if (animState != nullptr) animState->setLoop(looping);
}

void AnimationHelper::setPlaying(const bool p) {
    playing = p;
    if (animState != nullptr) animState->setEnabled(playing);
}

bool AnimationHelper::getLoop() const { return looping; }

bool AnimationHelper::getPlaying() const { return playing; }

}