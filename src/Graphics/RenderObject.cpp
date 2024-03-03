#include "RenderObject.h"
#include "OgreMovableObject.h"
#include "Node.h"
#include "Ogre.h"

Tapioca::RenderObject::RenderObject(Tapioca::Node* node, Ogre::SceneManager* sceneManager)
    : movObject(nullptr), node(node), sceneManager(sceneManager) { }

void Tapioca::RenderObject::init(Ogre::MovableObject* movObject) {
    this->movObject = movObject;
    node->attachObject(this);
}

void Tapioca::RenderObject::detachFromNode() {
    if (node != nullptr) {
        node->detachObject(this);
        node = nullptr;
    }
}

Tapioca::RenderObject::~RenderObject() {
    detachFromNode();
    sceneManager->destroyMovableObject(movObject);
    movObject = nullptr;
}

void Tapioca::RenderObject::setVisible(bool enable) { movObject->setVisible(enable); }
