#include "RenderObject.h"
#include "OgreMovableObject.h"
#include "RenderNode.h"
#include "Ogre.h"

namespace Tapioca {

RenderObject::RenderObject(RenderNode* const node, Ogre::SceneManager* const sceneManager)
    : movObject(nullptr), node(node), sceneManager(sceneManager) { }

void RenderObject::init(Ogre::MovableObject* const movObject) {
    this->movObject = movObject;
    node->attachObject(this);
}

void RenderObject::detachFromNode() {
    if (node != nullptr) {  // por precaucion
        node->detachObject();
        node = nullptr;
    }
}

RenderObject::~RenderObject() {
    detachFromNode();
    sceneManager->destroyMovableObject(movObject);
    movObject = nullptr;
}

void RenderObject::setVisible(const bool enable) { movObject->setVisible(enable); }

bool RenderObject::getVisible() const { return movObject->getVisible(); }
}