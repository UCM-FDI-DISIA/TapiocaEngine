#include "KeyFrameAnimator.h"
#include "Utilities/NodeAnimator.h"
#include "GraphicsManager.h"
#include "MeshRenderer.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "checkML.h"

namespace Tapioca {
KeyFrameAnimator::KeyFrameAnimator()
    : nodeAnimator(nullptr), speed(1.0f), duration(0.0f), info(), playByDefault(true), loop(true) { }

KeyFrameAnimator::~KeyFrameAnimator() {
    if (nodeAnimator != nullptr) delete nodeAnimator;
    nodeAnimator = nullptr;
}

bool KeyFrameAnimator::initComponent(const CompMap& variables) {
    if (!setValueFromMap(speed, "playbackSpeed", variables) || speed == 0.0f) {
        logInfo("KeyFrameAnimator: La velocidad inicial es 1.0f.");
    }

    if (!setValueFromMap(duration, "duration", variables)) {
        logError("KeyFrameAnimator: No se pudo establecer la duracion.");
        return false;
    }
    if (duration <= 0.0f) {
        logError("KeyFrameAnimator: La duracion establecida no es correcta. Tiene que ser mayor que 0.0f.");
        return false;
    }

    if (!setValueFromMap(playByDefault, "play", variables)) {
        logInfo("KeyFrameAnimator: La animacion se reproduce por defecto.");
    }

    if (!setValueFromMap(loop, "loop", variables)) {
        logInfo("KeyFrameAnimator: La animacion se loopea por defecto.");
    }

    bool end = false;
    while (!end) {
        Vector3 pos;
        bool posSet = setValueFromMap(pos.x, "posKeyX" + std::to_string(info.size + 1), variables) &&
            setValueFromMap(pos.y, "posKeyY" + std::to_string(info.size + 1), variables) &&
            setValueFromMap(pos.z, "posKeyZ" + std::to_string(info.size + 1), variables);
        if (posSet) info.pos[info.size] = pos;

        Vector3 scale;
        bool scaleSet = setValueFromMap(scale.x, "scaleKeyX" + std::to_string(info.size + 1), variables) &&
            setValueFromMap(scale.y, "scaleKeyY" + std::to_string(info.size + 1), variables) &&
            setValueFromMap(scale.z, "scaleKeyZ" + std::to_string(info.size + 1), variables);
        if (scaleSet) info.scale[info.size] = scale;

        Vector3 rot;
        bool rotSet = setValueFromMap(rot.x, "rotKeyX" + std::to_string(info.size + 1), variables) &&
            setValueFromMap(rot.y, "rotKeyY" + std::to_string(info.size + 1), variables) &&
            setValueFromMap(rot.z, "rotKeyZ" + std::to_string(info.size + 1), variables);
        if (rotSet) {
            info.rot[info.size] = rot;
        }

        if (!posSet && !scaleSet && !rotSet) end = true;
        else
            ++info.size;
    }
    if (info.size <= 0) return false;
    return true;
}

void KeyFrameAnimator::start() {
    GraphicsManager* graphicsManager = GraphicsManager::instance();
    MeshRenderer* meshRenderer = object->getComponent<MeshRenderer>();
    RenderNode* node = meshRenderer->getNode();
    nodeAnimator = graphicsManager->createNodeAnimatorWithName(node, duration);

    Transform* transform = object->getComponent<Transform>();
    nodeAnimator->addKeyFramePosScaleRot(transform->getGlobalPosition(), transform->getGlobalScale(),
                                         transform->getGlobalRotation());

    for (int i = 0; i < info.size; ++i) {
        KeyFrame* keyFrame = nodeAnimator->addKeyFrame();
        auto itPos = info.pos.find(i);
        if (itPos != info.pos.end()) keyFrame->pos = itPos->second;
        auto itScale = info.scale.find(i);
        if (itScale != info.scale.end()) keyFrame->scale = itScale->second;
        auto itRot = info.rot.find(i);
        if (itRot != info.rot.end()) keyFrame->rot = Quaternion(itRot->second);
    }

    nodeAnimator->init();
    if (playByDefault) play(loop);
}

void KeyFrameAnimator::update(uint64_t deltaTime) { nodeAnimator->updateAnim(deltaTime, speed); }

void KeyFrameAnimator::play(const bool loop) { nodeAnimator->play(loop); }

void KeyFrameAnimator::enable(const bool enabled) { nodeAnimator->enable(enabled); }

void KeyFrameAnimator::setLoop(const bool loop) { nodeAnimator->setLoop(loop); }

bool KeyFrameAnimator::isLooping() const { return nodeAnimator->isLooping(); }

bool KeyFrameAnimator::isEnabled() const { return nodeAnimator->isEnabled(); }

bool KeyFrameAnimator::hasEnded() const { return nodeAnimator->hasEnded(); }
}
