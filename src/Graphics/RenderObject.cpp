#include "RenderObject.h"
#include "OgreMovableObject.h"
#include "Node.h"

Tapioca::RenderObject::RenderObject(Node* node)
    : movObject(nullptr)
    , node(node) { }

void Tapioca::RenderObject::init(Ogre::MovableObject* movObject) {
    this->movObject = movObject;
    node->attachObject(this);
}

void Tapioca::RenderObject::setVisible(bool enable) { movObject->setVisible(enable); }

void Tapioca::RenderObject::detachFromNode() { node->detachObject(this); }