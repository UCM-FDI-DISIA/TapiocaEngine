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

namespace Tapioca {
Animator::Animator() : meshRenderer(nullptr), anim(nullptr), speed(1.f) { }

Animator::~Animator() {
    meshRenderer = nullptr;
    if (anim != nullptr) delete anim;
    anim = nullptr;
}

bool Animator::initComponent(const CompMap& variables) {
    if (!setValueFromMap(speed, "playbackSpeed", variables)) {
        logInfo(("Animator: No se encontro el valor de playbackSpeed. Se inicializo al valor predefinido: \"" +
                 std::to_string(speed) + "\".")
                    .c_str());
    }
    return true;
}

void Animator::start() {
    meshRenderer = object->getComponent<MeshRenderer>();
    GraphicsManager* graphicsManager = GraphicsManager::instance();
    if (graphicsManager != nullptr) anim = graphicsManager->createAnimationHelper(meshRenderer->getMesh(), true, true);
}

void Animator::update(uint64_t delt) { anim->updateAnim(delt, speed); }

void Animator::playAnim(std::string const& animName) { anim->playAnim(animName); }

bool Animator::getLoop() const { return anim->getLoop(); }

bool Animator::getPlaying() const { return anim->getPlaying(); }

void Animator::setLoop(const bool l) { anim->setLoop(l); }

void Animator::setPlaying(const bool p) { anim->setPlaying(p); }
}
