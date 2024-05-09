#include "AnimationHelper.h"
#include "GraphicsManager.h"
#include "Mesh.h"
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "checkML.h"

namespace Tapioca {
AnimationHelper::AnimationHelper(Mesh* const object, const bool autoPlay = true, const bool loop = true)
    : animState(nullptr), animStateSet(object->getMesh()->getAllAnimationStates()), playing(autoPlay), looping(loop) 
{
    logInfo("AnimationHelper: Animaciones:");
    if (animStateSet != nullptr)
        for (auto it = animStateSet->getAnimationStateIterator().begin();
             it != animStateSet->getAnimationStateIterator().end(); ++it) {
            logInfo(("AnimationHelper: \t" + it->first).c_str());
        }
}

AnimationHelper::~AnimationHelper() {
    animState = nullptr;
    animStateSet = nullptr;
}

void AnimationHelper::updateAnim(const uint64_t delt, const float speed) {
    if (animStateSet != nullptr && animState != nullptr) {
        // Actualiza la animacion
        animState->addTime((float)(delt * 0.001 * speed));
    }
}

void AnimationHelper::playAnim(std::string const& anim) {
    if (animStateSet == nullptr || !animStateSet->hasAnimationState(anim)) {
        Tapioca::logError("GraphicsEngine: Error al cargar recursos: No existe la animacion");
        return;
    }

    if (animStateSet != nullptr) { 
        if(animState != nullptr) animState->setEnabled(false);
        animState = animStateSet->getAnimationState(anim);
        animState->setEnabled(playing);
        animState->setLoop(looping);
        animState->setTimePosition(0);
    }
}

void AnimationHelper::setLoop(const bool l) {
    looping = l;
    if (animStateSet != nullptr && animState != nullptr) animState->setLoop(looping);
}

inline bool AnimationHelper::hasEnded() const {
    return animStateSet != nullptr && animState != nullptr && animState->hasEnded();
}

void AnimationHelper::setPlaying(const bool p) {
    playing = p;
    if (animStateSet != nullptr && animState != nullptr) animState->setEnabled(playing);
}
std::string AnimationHelper::getAnimName() const {
    if (animStateSet != nullptr && animState != nullptr) return animState->getAnimationName();
    else
        return "";
}
}
