#include "Node.h"
// Includes de Ogre
#include <Ogre.h>
// Includes del Core
#include "Utilities/Vector3.h"
#include "Utilities/checkML.h"

Ogre::SceneNode* Tapioca::Node::addChild(Node* child) {
    children.insert(child);
    return node->createChildSceneNode();
}

void Tapioca::Node::removeChild(Node* child) {
    if (children.contains(child)) {
        children.erase(child);
    }
}

void Tapioca::Node::deleteChildren(std::unordered_set<Node*>* nodes) {
    for (auto& child : children) {
        if (nodes->contains(child)) {
            nodes->erase(child);
            child->deleteChildren(nodes);
            delete child;
        }
    }
}

void Tapioca::Node::getAllChildrenAux(std::vector<Node*>& allChildren) {
    for (auto child : children) {
        allChildren.push_back(child);
        child->getAllChildrenAux(allChildren);
    }
}

Tapioca::Node::Node(Ogre::SceneManager* sceneManager, Vector3 pos, Vector3 scale, Node* parent)
    : parent(parent) {
    if (parent == nullptr) {
        node = sceneManager->getRootSceneNode()->createChildSceneNode();
    } else {
        node = parent->addChild(this);
    }
    node->setPosition(pos.x, pos.y, pos.z);
    node->setScale(scale.x, scale.y, scale.z);
}

Tapioca::Node::~Node() {
    parent = nullptr;
    children.clear();
}

void Tapioca::Node::removeFromTree(std::unordered_set<Node*>* nodes) {
    if (parent != nullptr) {
        parent->removeChild(this);
    }
    // this->removeFromHierarchy
    this->deleteChildren(nodes);
}

std::vector<Tapioca::Node*> Tapioca::Node::getAllChildren() {
    std::vector<Tapioca::Node*> allChildren;
    getAllChildrenAux(allChildren);
    return allChildren;
}