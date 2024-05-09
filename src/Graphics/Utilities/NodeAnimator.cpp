#include "NodeAnimator.h"
#include "RenderNode.h"
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

namespace Tapioca {
NodeAnimator::NodeAnimator(Ogre::SceneManager* scnMngr, RenderNode* node, float duration, std::string name)
    : animationState(nullptr), sceneManager(scnMngr), node(node), duration(duration), enabled(false), loop(false),
    name(name), keyFrames() { }

NodeAnimator::~NodeAnimator() { enable(false); }

void NodeAnimator::init() {
    Ogre::Animation* animation = sceneManager->createAnimation(name, duration);
    Ogre::NodeAnimationTrack* track = animation->createNodeTrack(0);
    track->setAssociatedNode(node->getNode());
    float keyDuration = duration / (keyFrames.size() - 1);

    Ogre::TransformKeyFrame* kf;
    for (int i = 0; i < keyFrames.size(); ++i) {
        kf = track->createNodeKeyFrame(keyDuration * i);
        KeyFrame keyFrame = keyFrames[i];
        kf->setTranslate(Ogre::Vector3(keyFrame.pos.x, keyFrame.pos.y, keyFrame.pos.z));
        kf->setScale(Ogre::Vector3(keyFrame.scale.x, keyFrame.scale.y, keyFrame.scale.z));
        kf->setRotation(
            Ogre::Quaternion(keyFrame.rot.scalar, keyFrame.rot.vector.x, keyFrame.rot.vector.y, keyFrame.rot.vector.z));
    }

    // Aunque son menos precisos y se basan en lineas rectos, se usan los modos de interpolacion
    // por defecto porque los otros dos causaban problemas
    animation->setInterpolationMode(Ogre::Animation::IM_LINEAR);
    // Para rotaciones
    animation->setRotationInterpolationMode(Ogre::Animation::RIM_LINEAR);

    animationState = sceneManager->createAnimationState(name);

    enable(enabled);
    setLoop(loop);
}

void NodeAnimator::updateAnim(const uint64_t delt, const float speed) {
    if (animationState != nullptr) animationState->addTime(delt * 0.001f * speed);
}

void NodeAnimator::play(const bool loop) {
    if (animationState != nullptr) {
        animationState->setTimePosition(0.0f);
        setLoop(loop);
        enable(true);
    }
}

void NodeAnimator::enable(const bool enabled) {
    if (animationState != nullptr) {
        this->enabled = enabled;
        animationState->setEnabled(enabled);
    }
}

void NodeAnimator::setLoop(const bool loop) {
    if (animationState != nullptr) {
        this->loop = loop;
        animationState->setLoop(loop);
    }
}

bool NodeAnimator::hasEnded() const {
    // Devuelve true si la animacion ha terminado y no esta loopeando
    return animationState->hasEnded();
}

KeyFrame* NodeAnimator::addKeyFrame() {
    KeyFrame keyFrame;
    if (keyFrames.size() > 0) keyFrame = keyFrames[keyFrames.size() - 1];
    keyFrames.push_back(keyFrame);
    return &keyFrames[keyFrames.size() - 1];
}

void NodeAnimator::addKeyFramePosScaleRot(Vector3 pos, Vector3 scale, Vector3 rot) {
    KeyFrame keyFrame;
    keyFrame.pos = pos;
    keyFrame.scale = scale;
    keyFrame.rot = Quaternion(rot);
    keyFrames.push_back(keyFrame);
}

void NodeAnimator::addKeyFramePosScaleRot(Vector3 pos, Vector3 scale, Quaternion rot) {
    KeyFrame keyFrame;
    keyFrame.pos = pos;
    keyFrame.scale = scale;
    keyFrame.rot = rot;
    keyFrames.push_back(keyFrame);
}

void NodeAnimator::addKeyFramePosScale(Vector3 pos, Vector3 scale) {
    KeyFrame keyFrame;
    keyFrame.pos = pos;
    keyFrame.scale = scale;
    if (keyFrames.size() > 0) {
        KeyFrame prevKeyFrame = keyFrames[keyFrames.size() - 1];
        keyFrame.rot = prevKeyFrame.rot;
    }
    keyFrames.push_back(keyFrame);
}

void NodeAnimator::addKeyFramePosRot(Vector3 pos, Vector3 rot) {
    KeyFrame keyFrame;
    keyFrame.pos = pos;
    keyFrame.rot = Quaternion(rot);
    if (keyFrames.size() > 0) {
        KeyFrame prevKeyFrame = keyFrames[keyFrames.size() - 1];
        keyFrame.scale = prevKeyFrame.scale;
    }
    keyFrames.push_back(keyFrame);
}

void NodeAnimator::addKeyFramePosRot(Vector3 pos, Quaternion rot) {
    KeyFrame keyFrame;
    keyFrame.pos = pos;
    keyFrame.rot = rot;
    if (keyFrames.size() > 0) {
        KeyFrame prevKeyFrame = keyFrames[keyFrames.size() - 1];
        keyFrame.scale = prevKeyFrame.scale;
    }
    keyFrames.push_back(keyFrame);
}

void NodeAnimator::addKeyFrameScaleRot(Vector3 scale, Vector3 rot) {
    KeyFrame keyFrame;
    keyFrame.scale = scale;
    keyFrame.rot = Quaternion(rot);
    if (keyFrames.size() > 0) {
        KeyFrame prevKeyFrame = keyFrames[keyFrames.size() - 1];
        keyFrame.pos = prevKeyFrame.pos;
    }
    keyFrames.push_back(keyFrame);
}

void NodeAnimator::addKeyFrameScaleRot(Vector3 scale, Quaternion rot) {
    KeyFrame keyFrame;
    keyFrame.scale = scale;
    keyFrame.rot = rot;
    if (keyFrames.size() > 0) {
        KeyFrame prevKeyFrame = keyFrames[keyFrames.size() - 1];
        keyFrame.pos = prevKeyFrame.pos;
    }
    keyFrames.push_back(keyFrame);
}

void NodeAnimator::addKeyFramePos(Vector3 pos) {
    KeyFrame keyFrame;
    keyFrame.pos = pos;
    if (keyFrames.size() > 0) {
        KeyFrame prevKeyFrame = keyFrames[keyFrames.size() - 1];
        keyFrame.scale = prevKeyFrame.scale;
        keyFrame.rot = prevKeyFrame.rot;
    }
    keyFrames.push_back(keyFrame);
}

void NodeAnimator::addKeyFrameScale(Vector3 scale) {
    KeyFrame keyFrame;
    keyFrame.scale = scale;
    if (keyFrames.size() > 0) {
        KeyFrame prevKeyFrame = keyFrames[keyFrames.size() - 1];
        keyFrame.pos = prevKeyFrame.pos;
        keyFrame.rot = prevKeyFrame.rot;
    }
    keyFrames.push_back(keyFrame);
}

void NodeAnimator::addKeyFrameRot(Vector3 rot) {
    KeyFrame keyFrame;
    keyFrame.rot = Quaternion(rot);
    if (keyFrames.size() > 0) {
        KeyFrame prevKeyFrame = keyFrames[keyFrames.size() - 1];
        keyFrame.pos = prevKeyFrame.pos;
        keyFrame.scale = prevKeyFrame.scale;
    }
    keyFrames.push_back(keyFrame);
}

void NodeAnimator::addKeyFrameRot(Quaternion rot) {
    KeyFrame keyFrame;
    keyFrame.rot = rot;
    if (keyFrames.size() > 0) {
        KeyFrame prevKeyFrame = keyFrames[keyFrames.size() - 1];
        keyFrame.pos = prevKeyFrame.pos;
        keyFrame.scale = prevKeyFrame.scale;
    }
    keyFrames.push_back(keyFrame);
}
}
