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
AnimationHelper::AnimationHelper(Mesh* const object, const bool autoPlay = true, const bool loop = true)
    : animState(nullptr), animStateSet(object->getMesh()->getAllAnimationStates()), playing(autoPlay), looping(loop) {
#ifdef _DEBUG
    logInfo("AnimationHelper: Animaciones:");
    auto aux = object->getMesh()->getAllAnimationStates();
    for (auto it = aux->getAnimationStateIterator().begin(); it != aux->getAnimationStateIterator().end(); ++it) {
        logInfo(("AnimationHelper: \t" + it->first).c_str());
    }
#endif
}

AnimationHelper::~AnimationHelper() {
    animState = nullptr;
    animStateSet = nullptr;
}

void AnimationHelper::updateAnim(const uint64_t delt, const float speed) {
    if (animState != nullptr) {
        // Actualiza la animacion
        animState->addTime(delt * 0.001 * speed);
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
}