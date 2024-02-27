#pragma once
#include <unordered_set>
#include <vector>

namespace Ogre {
class SceneManager;
class SceneNode;
}

namespace Tapioca {
class Vector3;
class GraphicsEngine;
}

namespace Tapioca {
class Node {
public:
    friend GraphicsEngine;

private:
    std::unordered_set<Node*> children;
    Node* parent;
    Ogre::SceneNode* node;

    Ogre::SceneNode* addChild(Node* child);

    void removeChild(Node* child);
    void removeFromHierarchy();
    void deleteChildren(std::unordered_set<Node*>* nodes);

    void getAllChildrenAux(std::vector<Node*>& allChildren);

    Node(Ogre::SceneManager* sceneManager, Vector3 pos, Vector3 scale, Node* parent = nullptr);

public:
    virtual ~Node();

    void removeFromTree(std::unordered_set<Node*>* nodes);
    void setPosition(Vector3 pos);
    void setRotation(Vector3 rot);
    void setScale(Vector3 sc);
    void translate(Vector3 t);
    void rotate(Vector3 r);
    void scale(Vector3 s);
    Node* getChild(int index);
    std::vector<Node*> getAllChildren();
};
}