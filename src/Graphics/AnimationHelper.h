#pragma once
#include <string>
#include "defs.h"

namespace Ogre {
class AnimationState;
class AnimationStateSet;
}

namespace Tapioca {
class RenderNode;
class Mesh;

class TAPIOCA_API AnimationHelper {

private:
    Ogre::AnimationState* animState;
    Ogre::AnimationStateSet* animStateSet;
    bool playing;
    bool looping;

public:
    AnimationHelper(Mesh* const object, const bool autoPlay, const bool loop);
    ~AnimationHelper();

    void updateAnim(const uint64_t delt, const float speed);
    void playAnim(std::string const& anim);

    void setLoop(const bool l);
    void setPlaying(const bool p);

    bool getLoop() const;
    bool getPlaying() const;
};

}
