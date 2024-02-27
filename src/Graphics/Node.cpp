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

void Tapioca::Node::destroyAllAttachedObjects(Ogre::SceneNode* node) {
    // por precaucion
    if (!node) {
        return;
    } else {
        // se recorren todos los objetos que cuelgan del nodo y se eliminan
        // (un nodo puede tener colgando varios objetos)
        for (auto object : node->getAttachedObjects()) {
            sceneManager->destroyMovableObject(object);
        }

        // se hace lo mismo con todos los hijos del nodo
        for (auto child : node->getChildren()) {
            destroyAllAttachedObjects(static_cast<Ogre::SceneNode*>(child));
        }
    }
}

void Tapioca::Node::removeFromHierarchy() {
    // se eliminan todos los objetos que cuelgan del nodo y de todos los nodos hijos
    destroyAllAttachedObjects(node);
    // se eliminan todos los nodos hijos del grafo de la escena
    node->removeAndDestroyAllChildren();
    // se elimina el nodo de la escena
    sceneManager->destroySceneNode(node);
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

void Tapioca::Node::removeChild(Node* child) {
    if (children.contains(child)) {
        children.erase(child);
    }
}

void Tapioca::Node::getAllChildrenAux(std::vector<Node*>& allChildren) {
    for (auto child : children) {
        allChildren.push_back(child);
        child->getAllChildrenAux(allChildren);
    }
}

Tapioca::Node::Node(Ogre::SceneManager* sceneManager, Vector3 pos, Vector3 scale, Node* parent)
    : parent(parent)
    , sceneManager(sceneManager) {
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
        // si el nodo tiene padre, se elimina de la lista del padre
        parent->removeChild(this);
    }
    // se eliminan objetos y nodos del grafo de la escena
    this->removeFromHierarchy();
    // se hace el delete de todos los nodos hijos
    this->deleteChildren(nodes);
}

std::vector<Tapioca::Node*> Tapioca::Node::getAllChildren() {
    std::vector<Tapioca::Node*> allChildren;
    getAllChildrenAux(allChildren);
    return allChildren;
}