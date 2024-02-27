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
class Light;
class Camera;
}

namespace Tapioca {
class Node {
public:
    friend GraphicsEngine;
    friend Light;
    friend Camera;

private:
    Ogre::SceneManager* sceneManager;
    std::unordered_set<Node*> children;
    Node* parent;
    Ogre::SceneNode* node;

    Ogre::SceneNode* addChild(Node* child);
    // eliminar todos los objetos del nodo y sus hijos
    void destroyAllAttachedObjects(Ogre::SceneNode* node);
    // eliminar el nodo y los hijos del grafo de la escena
    void removeFromHierarchy();
    // hacer el delete de todos los nodos hijos
    void deleteChildren(std::unordered_set<Node*>* nodes);
    // eliminar un nodo hijo
    void removeChild(Node* child);

    void getAllChildrenAux(std::vector<Node*>& allChildren);
    Ogre::SceneNode* getSceneNode() { return node; }

    Node(Ogre::SceneManager* sceneManager, Vector3 pos, Vector3 scale, Node* parent = nullptr);

public:
    virtual ~Node();

    // eliminar nodos y objetos del grafo de la escena y hacer delete de los nodos hijos
    void removeFromTree(std::unordered_set<Node*>* nodes);

    // mover, rotar y escalar
    void setPosition(Vector3 pos);
    void setRotation(Vector3 rot);
    void setScale(Vector3 sc);
    void translate(Vector3 t);
    void rotate(Vector3 r);
    void scale(Vector3 s);

    // coger hijo por indice
    Node* getChild(int index);

    // vector con todos los hijos y los subhijos
    std::vector<Node*> getAllChildren();
};
}