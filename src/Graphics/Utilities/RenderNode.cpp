#include "RenderNode.h"
// OGRE
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
// TAPIOCA
#include "RenderObject.h"
#include "Utilities/Vector3.h"
#include "Utilities/Quaternion.h"

namespace Tapioca {
void RenderNode::attachObject(RenderObject* const object) {
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
    if (object != nullptr) {   // por precaucion
        node->detachObject(object->getMovObject());
        object = nullptr;
    }
}

// Globales porque la posicion de los nodos siempre es en globales
// (todos cuelgan del root)
void RenderNode::lookAt(const Vector3 target) {
    node->lookAt(Ogre::Vector3(target.x, target.y, target.z), Ogre::Node::TransformSpace::TS_WORLD);
}

void RenderNode::setDirection(const Vector3 dir) { node->setDirection(dir.x, dir.y, dir.z, Ogre::Node::TS_WORLD); }

RenderNode::RenderNode(Ogre::SceneManager* const sceneManager, const Vector3 pos, const Vector3 scale)
    : sceneManager(sceneManager), object(nullptr) {

    node = sceneManager->getRootSceneNode()->createChildSceneNode();

    setPosition(pos);
    setScale(scale);
}

Ogre::SceneNode* RenderNode::getNode() const { return node; }

RenderNode::~RenderNode() {
    delete object;
    object = nullptr;
    sceneManager->destroySceneNode(node);
    node = nullptr;
}

void RenderNode::setPosition(const Vector3 newPos) { node->setPosition(newPos.x, newPos.y, newPos.z); }
void RenderNode::setScale(const Vector3 newScale) { node->setScale(newScale.x, newScale.y, newScale.z); }
void RenderNode::translate(const Vector3 mov) { node->translate(mov.x, mov.y, mov.z); }
void RenderNode::scale(const Vector3 scale) { node->scale(scale.x, scale.y, scale.z); }

void RenderNode::setRotation(const Quaternion q) {
    Ogre::Quaternion oq = Ogre::Quaternion(q.scalar, q.vector.x, q.vector.y, q.vector.z);
    node->setOrientation(oq);
}
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

Vector3 RenderNode::getPosition() const {
    Ogre::Vector3 pos = node->getPosition();
    return Vector3(pos.x, pos.y, pos.z);
}
}