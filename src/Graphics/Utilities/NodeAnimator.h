#pragma once
#include "defs.h"
#include "Utilities/Vector3.h"
#include "Utilities/Quaternion.h"
#include <vector>

namespace Ogre {
class AnimationState;
class SceneManager;
}

namespace Tapioca {
class GraphicsManager;
class RenderNode;

struct KeyFrame {
    Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
    Quaternion rot = Quaternion(Vector3(0.0f, 0.0f, 1.0f));
};

class TAPIOCA_API NodeAnimator {
private:
    friend GraphicsManager;

    Ogre::SceneManager* sceneManager;
    RenderNode* node;
    Ogre::AnimationState* animationState;
    // en segundos
    float duration;
    std::vector<KeyFrame> keyFrames;
    bool enabled;
    bool loop;
    std::string name;

    NodeAnimator(Ogre::SceneManager* const scnMgr, RenderNode* const node, float duration, std::string name);

public:
    void init();

    void updateAnim(const uint64_t delt, const float speed);

    void play(const bool loop);

    void enable(const bool enabled);

    void setLoop(const bool loop);

    bool isLooping() const;

    bool isEnabled() const;

    bool hasEnded() const;

    KeyFrame* addKeyFrame();
    void addKeyFramePosScaleRot(Vector3 pos, Vector3 scale, Vector3 rot);
    void addKeyFramePosScaleRot(Vector3 pos, Vector3 scale, Quaternion rot);
    void addKeyFramePosScale(Vector3 pos, Vector3 scale);
    void addKeyFramePosRot(Vector3 pos, Vector3 rot);
    void addKeyFramePosRot(Vector3 pos, Quaternion rot);
    void addKeyFrameScaleRot(Vector3 scale, Vector3 rot);
    void addKeyFrameScaleRot(Vector3 scale, Quaternion rot);
    void addKeyFramePos(Vector3 pos);
    void addKeyFrameScale(Vector3 scale);
    void addKeyFrameRot(Vector3 rot);
    void addKeyFrameRot(Quaternion rot);
};
}