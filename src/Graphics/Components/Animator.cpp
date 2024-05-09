#include "Animator.h"
#include "GraphicsManager.h"
#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Structure/GameObject.h"
#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "Utilities/RenderNode.h"
#include "Utilities/Mesh.h"
#include "Utilities/AnimationHelper.h"
#include "checkML.h"

namespace Tapioca {
Animator::Animator() : meshRenderer(nullptr), anim(nullptr), speed(1.f), initAnim("") { }

Animator::~Animator() {
    meshRenderer = nullptr;
    if (anim != nullptr) delete anim;
    anim = nullptr;
}

bool Animator::initComponent(const CompMap& variables) {
    if (!setValueFromMap(speed, "playbackSpeed", variables)) {
        logInfo(("Animator: No se encontro el valor de playbackSpeed. Se inicializo al valor predefinido: \"" +
                 std::to_string(speed) + "\".").c_str());
    }
    if (!setValueFromMap(initAnim, "initAnim", variables)) {
        logInfo("Animator: No se encontro el valor de initAnim. Se inicializa sin animacion inicial. \"");
    }
    return true;
}

void Animator::start() {
    meshRenderer = object->getComponent<MeshRenderer>();
    if (meshRenderer == nullptr) {
        alive = active = false;
        pushEvent("ev_componentDied", this);
    }
    else {
        GraphicsManager* graphicsManager = GraphicsManager::instance();
        if (graphicsManager != nullptr) anim = graphicsManager->createAnimationHelper(meshRenderer->getMesh(), true, true);
        if (initAnim != "") playAnim(initAnim);
    }
}

void Animator::update(uint64_t deltaTime) { anim->updateAnim(deltaTime, speed); }

void Animator::playAnim(std::string const& animName) { anim->playAnim(animName); }

bool Animator::getLoop() const { return anim->getLoop(); }

bool Animator::getPlaying() const { return anim->getPlaying(); }

bool Animator::hasEnded() const { return anim->hasEnded(); }

std::string Animator::getAnimName() const { return anim->getAnimName(); }

void Animator::setLoop(const bool l) { anim->setLoop(l); }

void Animator::setPlaying(const bool p) { anim->setPlaying(p); }
}
