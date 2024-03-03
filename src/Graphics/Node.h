#pragma once
#include "Utilities/INode.h"

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

class Node : public INode {
public:
    friend GraphicsEngine;
    friend RenderObject;
    friend Camera;
    friend LightDirectional;

private:
    Ogre::SceneManager* sceneManager;
    std::unordered_set<Node*> children;
    Node* parent;
    std::unordered_set<RenderObject*> objects;
    Ogre::SceneNode* node;

    // AGREGAR HIJO
    Ogre::SceneNode* createChild(Node* child);

    void addChild(Node* child);
    // eliminar un nodo hijo
    void removeChild(Node* child);

    // ELIMINAR
    // eliminar todos los objetos del nodo y sus hijos
    void destroyAllAttachedObjects(Ogre::SceneNode* node);
    // eliminar el nodo y los hijos del grafo de la escena
    void removeFromHierarchy();
    // hacer el delete de todos los nodos hijos
    void deleteChildren(std::unordered_set<Node*>* nodes);
    // eliminar nodos y objetos del grafo de la escena y hacer delete de los nodos hijos
    void removeFromTree(std::unordered_set<Node*>* nodes);

    void getAllChildrenAux(std::vector<INode*>& allChildren);

    // PARA DIFERENTES OBJETOS
    void attachObject(Tapioca::RenderObject* object);
    void attachObject(Ogre::MovableObject* object);   // solo para ManualObject
    void detachObject(Tapioca::RenderObject* object);

    void lookAt(Vector3 target);
    void setDirection(Vector3 dir);

    inline Ogre::SceneNode* getSceneNode() const { return node; }

    Node(Ogre::SceneManager* sceneManager, Vector3 pos, Vector3 scale, Node* parent = nullptr);

public:
    virtual ~Node();

    // mover, rotar y escalar
    void setPosition(Vector3 pos) override;
    void setRotation(Vector3 rot) override;
    void setScale(Vector3 sc) override;
    void translate(Vector3 t) override;
    void rotate(Vector3 r);
    /*Combina la escala actual con la ya existente (sumando)
    Por ejemplo, si el object tiene una escala de Vector3(2,2,2) y se llama a este
    metodo con una escala de Vector3(3,3,3), la escala resultante del objeto seria
    Vector3(5,5,5)*/
    void scale(Vector3 s) override;

    // TODO: Quitar
    // coger hijo por indice
    Node* getChild(int index);

    // TODO: Cambiar a que solo sean los hijos
    // vector con todos los hijos y los subhijos
    std::vector<INode*> getAllChildren() override;

    void setParent(INode* parent) override;
};
}