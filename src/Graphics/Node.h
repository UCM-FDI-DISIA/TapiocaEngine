#pragma once
#include <unordered_set>
#include <vector>

namespace Ogre {
class SceneManager;
class SceneNode;
class MovableObject;
}

namespace Tapioca {
class Vector3;
class GraphicsEngine;
class RenderObject;
class Camera;
class LightDirectional;
}

namespace Tapioca {
class Node {
public:
    friend GraphicsEngine;
    friend RenderObject;
    friend Camera;
    friend LightDirectional;

private:
    Ogre::SceneManager* sceneManager;
    std::unordered_set<Node*> children;
    Node* parent;
    Ogre::SceneNode* node;

    // AGREGAR HIJO
    Ogre::SceneNode* addChild(Node* child);

    // ELIMINAR
    // eliminar un nodo hijo
    void removeChild(Node* child);
    // eliminar todos los objetos del nodo y sus hijos
    void destroyAllAttachedObjects(Ogre::SceneNode* node);
    // eliminar el nodo y los hijos del grafo de la escena
    void removeFromHierarchy();
    // hacer el delete de todos los nodos hijos
    void deleteChildren(std::unordered_set<Node*>* nodes);
    // eliminar nodos y objetos del grafo de la escena y hacer delete de los nodos hijos
    void removeFromTree(std::unordered_set<Node*>* nodes);

    void getAllChildrenAux(std::vector<Node*>& allChildren);

    // PARA DIFERENTES OBJETOS
    void attachObject(Ogre::MovableObject* object);
    void detachObject(Ogre::MovableObject* object);
    void lookAt(Vector3 target);
    void setDirection(Vector3 dir);

    Node(Ogre::SceneManager* sceneManager, Vector3 pos, Vector3 scale, Node* parent = nullptr);

public:
    virtual ~Node();

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