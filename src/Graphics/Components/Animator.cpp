#include "Animator.h"
#include "../GraphicsManager.h"
#include "Components/Transform.h"
#include "../Components/MeshRenderer.h"
#include "Structure/GameObject.h"
#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "../RenderNode.h"
#include "../Mesh.h"
#include "../AnimationHelper.h"

namespace Tapioca {

Animator::Animator()
    : meshRenderer(nullptr), anim(nullptr), speed(1.f) { }

Animator::~Animator() { }

bool Animator::initComponent(const CompMap& variables) {
    if (!setValueFromMap(speed, "playbackSpeed", variables)) speed = 1.f;
    return true;
}

void Animator::start() {
    meshRenderer = object->getComponent<MeshRenderer>();
    anim = GraphicsManager::instance()->createAnimationHelper(meshRenderer->getMesh(), true, true);

    //anim->playAnim("CatMouth2");
}

void Animator::update(uint64_t delt) { anim->updateAnim(delt, speed); }

void Animator::playAnim(std::string const& animName) { anim->playAnim(animName); }

float Animator::getSpeed() const { return speed; }

bool Animator::getLoop() const { return anim->getLoop(); }

bool Animator::getPlaying() const { return anim->getPlaying(); }

void Animator::setSpeed(const float s) { speed = s; }

void Animator::setLoop(const bool l) { anim->setLoop(l); }

void Animator::setPlaying(const bool p) { anim->setPlaying(p); }
}