#include "RenderNode.h"
// OGRE
#include <Ogre.h>
// TAPIOCA
#include "RenderObject.h"
#include "Utilities/Vector3.h"
#include "Utilities/Quaternion.h"

namespace Tapioca {
//Ogre::SceneNode* RenderNode::createChild(RenderNode* const child) {
//    children.insert(child);
//    return node->createChildSceneNode();
//}

//void RenderNode::removeParent() {
//    if (this->parent != nullptr) {
//        removeAttachedParent();
//    }
//    else {
//        Ogre::SceneNode* parentNode = node->getParentSceneNode();
//        if (parentNode != nullptr) {
//            parentNode->removeChild(node);
//        }
//    }
//}

//void RenderNode::getAllChildrenAux(std::vector<RenderNode*>& allChildren) {
//    for (auto child : children) {
//        allChildren.push_back(child);
//        child->getAllChildrenAux(allChildren);
//    }
//}

void RenderNode::attachObject(RenderObject* const object) {
    /*auto movObject = object->getMovObject();
    if (!movObject->isAttached() && !objects.contains(object)) {
        node->attachObject(movObject);
        objects.insert(object);
    }*/

    if (this->object != object) {
        // eliminar el objeto si ya hay una colgando para poder meter uno nuevo
        if (this->object != nullptr) {
            delete this->object;
            this->object = nullptr;
        }
        node->attachObject(object->getMovObject());
        this->object = object;
    }
}

void RenderNode::attachObject(Ogre::MovableObject* const object) {
    if (!object->isAttached()) node->attachObject(object);
}

void RenderNode::detachObject() {
    /*auto movObject = object->getMovObject();
    if (movObject->isAttached() && objects.contains(object)) {
        node->detachObject(movObject);
        objects.erase(object);
    }*/

    if (object != nullptr) {   // por precaucion
        node->detachObject(object->getMovObject());
        object = nullptr;
    }
}

// ESTA EN GLOBALES, PERO SE PODRIA PERSONALZIAR
void RenderNode::lookAt(const Vector3 target) {
    node->lookAt(Ogre::Vector3(target.x, target.y, target.z), Ogre::Node::TS_WORLD);
}

// ESTA EN LOCALES, PERO SE PODRIA PERSONALIZAR
void RenderNode::setDirection(const Vector3 dir) { node->setDirection(dir.x, dir.y, dir.z, Ogre::Node::TS_LOCAL); }

RenderNode::RenderNode(Ogre::SceneManager* const sceneManager, const Vector3 pos, const Vector3 scale)
    : sceneManager(sceneManager), object(nullptr) {

    node = sceneManager->getRootSceneNode()->createChildSceneNode();

    node->setPosition(pos.x, pos.y, pos.z);
    node->setScale(scale.x, scale.y, scale.z);
}

RenderNode::~RenderNode() {
    delete object;
    object = nullptr;   // por precaucion
    sceneManager->destroySceneNode(node);
    node = nullptr;

    /*removeParent();

    for (auto it = objects.begin(), itAnt = objects.begin(); it != objects.end();) {
        ++itAnt;
        (*it)->detachFromNode();
        it = itAnt;
    }
    for (auto it = children.begin(), itAnt = children.begin(); it != children.end();) {
        ++itAnt;
        (*it)->removeAttachedParent();
        it = itAnt;
    }
    objects.clear();
    children.clear();
    sceneManager->destroySceneNode(node);
    node = nullptr;*/
}

//void RenderNode::removeChild(RenderNode* const child) {
//    if (children.contains(child)) {
//        children.erase(child);
//        node->removeChild(child->getSceneNode());
//    }
//}

//void RenderNode::removeAttachedParent() {
//    parent->removeChild(this);
//    parent = nullptr;
//}

//void RenderNode::addChild(RenderNode* const child) {
//    if (!children.contains(child)) {
//        children.insert(child);
//        node->addChild(child->getSceneNode());
//    }
//}

void RenderNode::setPosition(const Vector3 newPos) { node->setPosition(newPos.x, newPos.y, newPos.z); }
void RenderNode::setScale(const Vector3 newScale) { node->setScale(newScale.x, newScale.y, newScale.z); }
void RenderNode::translate(const Vector3 mov) { node->translate(mov.x, mov.y, mov.z); }
void RenderNode::scale(const Vector3 scale) { node->scale(scale.x, scale.y, scale.z); }

void RenderNode::setRotation(const Vector3 rot) {
    Quaternion q = Quaternion(rot);
    Ogre::Quaternion oq = Ogre::Quaternion(q.scalar, q.vector.x, q.vector.y, q.vector.z);
    node->setOrientation(oq);
}
void RenderNode::rotate(const Vector3 r) {
    Quaternion q = Quaternion(r);
    Ogre::Quaternion oq = Ogre::Quaternion(q.scalar, q.vector.x, q.vector.y, q.vector.z);
    node->rotate(oq);
}
void RenderNode::roll(const float angle) { node->roll(Ogre::Degree(angle), Ogre::Node::TS_WORLD); }
void RenderNode::yaw(const float angle) {
    //TODO: cambiar el espacio para comprobar que desde blender se exporta con los ejes bien (parece que si)
    node->yaw(Ogre::Degree(angle), Ogre::Node::TS_WORLD);
}
void RenderNode::pitch(const float angle) { node->pitch(Ogre::Degree(angle), Ogre::Node::TS_WORLD); }

//std::vector<RenderNode*> RenderNode::getAllChildren() {
//    std::vector<RenderNode*> allChildren;
//    getAllChildrenAux(allChildren);
//    return allChildren;
//}

//std::vector<RenderNode*> RenderNode::getChildren() {
//    std::vector<RenderNode*> childrenAux(children.size());
//    std::copy(children.begin(), children.end(), childrenAux.begin());
//    return childrenAux;
//}

//void RenderNode::setParent(INode* const parent) {
//    removeParent();
//    parent->addChild(this);
//    this->parent = parent;
//}
}