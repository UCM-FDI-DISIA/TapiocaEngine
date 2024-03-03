#include "Node.h"
#include "RenderObject.h"
// OGRE
#include <Ogre.h>
// CORE
#include "Utilities/Vector3.h"
#include "Utilities/checkML.h"
#include <iostream>


Ogre::SceneNode* Tapioca::Node::createChild(Node* child) {
    children.insert(child);
    return node->createChildSceneNode();
}

//void Tapioca::Node::destroyAllAttachedObjects(Ogre::SceneNode* node) {
//    // por precaucion
//    if (node != nullptr) {
//        return;
//    } else {
//        // se recorren todos los objetos que cuelgan del nodo y se eliminan
//        // (un nodo puede tener colgando varios objetos)
//        for (auto object : node->getAttachedObjects()) {
//            sceneManager->destroyMovableObject(object);
//        }
//
//        // se hace lo mismo con todos los hijos del nodo
//        for (auto child : node->getChildren()) {
//            destroyAllAttachedObjects(static_cast<Ogre::SceneNode*>(child));
//        }
//    }
//}
//
//void Tapioca::Node::removeFromHierarchy() {
//    // se eliminan todos los objetos que cuelgan del nodo y de todos los nodos hijos
//    destroyAllAttachedObjects(node);
//    // se eliminan todos los nodos hijos del grafo de la escena
//    node->removeAndDestroyAllChildren();
//    // se elimina el nodo de la escena
//    sceneManager->destroySceneNode(node);
//}
//
//void Tapioca::Node::deleteChildren(std::unordered_set<Node*>* nodes) {
//    for (auto& child : children) {
//        if (nodes->contains(child)) {
//            nodes->erase(child);
//            child->deleteChildren(nodes);
//            delete child;
//        }
//    }
//}

//void Tapioca::Node::removeFromTree(std::unordered_set<Node*>* nodes) {
//    if (parent != nullptr) {
//        // si el nodo tiene padre, se elimina de la lista del padre
//        parent->removeChild(this);
//    }
//    // se eliminan objetos y nodos del grafo de la escena
//    this->removeFromHierarchy();
//    // se hace el delete de todos los nodos hijos
//    this->deleteChildren(nodes);
//}

void Tapioca::Node::getAllChildrenAux(std::vector<INode*>& allChildren) {
    for (auto child : children) {
        allChildren.push_back(child);
        getAllChildrenAux(allChildren);
    }
}

void Tapioca::Node::attachObject(Tapioca::RenderObject* object) {
    auto movObject = object->getMovObject();
    if (!movObject->isAttached() && !objects.contains(object)) {
        node->attachObject(movObject);
        objects.insert(object);
    }
}

void Tapioca::Node::attachObject(Ogre::MovableObject* object) {
    if (!object->isAttached()) node->attachObject(object);
}

void Tapioca::Node::detachObject(Tapioca::RenderObject* object) {
    auto movObject = object->getMovObject();
    if (movObject->isAttached() && objects.contains(object)) {
        node->detachObject(movObject);
        objects.erase(object);
    }
}

// ESTA EN GLOBALES, PERO SE PODRIA PERSONALZIAR
void Tapioca::Node::lookAt(Vector3 target) {
    node->lookAt(Ogre::Vector3(target.x, target.y, target.z), Ogre::Node::TS_WORLD);
}

// ESTA EN LOCALES, PERO SE PODRIA PERSONALIZAR
void Tapioca::Node::setDirection(Vector3 dir) { node->setDirection(dir.x, dir.y, dir.z, Ogre::Node::TS_LOCAL); }

Tapioca::Node::Node(Ogre::SceneManager* sceneManager, Vector3 pos, Vector3 scale, Node* parent)
    : parent(parent), sceneManager(sceneManager), children(), objects() {
    if (parent == nullptr) node = sceneManager->getRootSceneNode()->createChildSceneNode();
    else
        node = parent->createChild(this);

    node->setPosition(pos.x, pos.y, pos.z);
    node->setScale(scale.x, scale.y, scale.z);
}

Tapioca::Node::~Node() {
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
    node = nullptr;
}

void Tapioca::Node::removeChild(INode* child) {
    if (children.contains(child)) {
        children.erase(child);
        node->removeChild(child->getSceneNode());
    }
}

void Tapioca::Node::removeAttachedParent() {
    parent->removeChild(this);
    parent = nullptr;
}

void Tapioca::Node::addChild(INode* child) {
    if (!children.contains(child)) {
        children.insert(child);
        node->addChild(child->getSceneNode());
    }
}

void Tapioca::Node::setPosition(Vector3 newPos) { node->setPosition(newPos.x, newPos.y, newPos.z); }
void Tapioca::Node::setScale(Vector3 newScale) { node->setScale(newScale.x, newScale.y, newScale.z); }
void Tapioca::Node::translate(Vector3 mov) { node->translate(mov.x, mov.y, mov.z); }
void Tapioca::Node::scale(Vector3 scale) { node->scale(scale.x, scale.y, scale.z); }
void Tapioca::Node::setRotation(Vector3 rot) { }
void Tapioca::Node::rotate(Vector3 r) { }

std::vector<Tapioca::INode*> Tapioca::Node::getAllChildren() {
    std::vector<Tapioca::INode*> allChildren;
    getAllChildrenAux(allChildren);
    return allChildren;
}

std::vector<Tapioca::INode*> Tapioca::Node::getChildren() {
    std::vector<Tapioca::INode*> childrenAux(children.size());
    std::copy(children.begin(), children.end(), childrenAux.begin());
    return childrenAux;
}


void Tapioca::Node::setParent(INode* parent) {
    if (this->parent != nullptr) {
        removeAttachedParent();
    }
    else {
        Ogre::SceneNode* parentNode = node->getParentSceneNode();
        if (parentNode != nullptr) {
            parentNode->removeChild(node);
        }
    }
    parent->addChild(this);
    this->parent = parent;
}
