#include "RenderObject.h"
#include "OgreMovableObject.h"
#include "Node.h"
#include "Ogre.h"

namespace Tapioca {

RenderObject::RenderObject(Node* const node, Ogre::SceneManager* const sceneManager)
    : movObject(nullptr), node(node), sceneManager(sceneManager) { }

void RenderObject::init(Ogre::MovableObject* const movObject) {
    this->movObject = movObject;
    node->attachObject(this);
}

void RenderObject::detachFromNode() {
    if (node != nullptr) {
        node->detachObject(this);
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