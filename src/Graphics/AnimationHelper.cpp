#include "AnimationHelper.h"
#include "Mesh.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

namespace Tapioca {
AnimationHelper::AnimationHelper(Mesh* const object, const bool autoPlay = true,
                                 const bool loop = true)
    : animState(nullptr), animStateSet(object->getMesh()->getAllAnimationStates()),
      playing(autoPlay), looping(loop) {
//#ifdef _DEBUG
//    auto aux = object->getMesh()->getAllAnimationStates();
//    for (auto it = aux->getAnimationStateIterator().begin(); it != aux->getAnimationStateIterator().end(); ++it) {
//        std::cout << it->first << '\n';
//    }
//#endif   // _DEBUG
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