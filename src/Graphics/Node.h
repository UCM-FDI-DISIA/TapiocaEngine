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

class TAPIOCA_API Node : public INode {
public:
    friend GraphicsEngine;
    friend RenderObject;
    friend Camera;
    friend LightDirectional;

private:
    Ogre::SceneManager* sceneManager;
    // si es nullptr, quiere decir que el padre es el nodo root
    INode* parent;
    std::unordered_set<INode*> children;
    std::unordered_set<RenderObject*> objects;
    //Ogre::SceneNode* node;

    // AGREGAR HIJO
    Ogre::SceneNode* createChild(Node* const child);

    void removeParent();

    // ELIMINAR
    //// eliminar todos los objetos del nodo y sus hijos
    //void destroyAllAttachedObjects(Ogre::SceneNode* node);
    //// eliminar el nodo y los hijos del grafo de la escena
    //void removeFromHierarchy();
    //// hacer el delete de todos los nodos hijos
    //void deleteChildren(std::unordered_set<Node*>* nodes);
    //// eliminar nodos y objetos del grafo de la escena y hacer delete de los nodos hijos
    //void removeFromTree(std::unordered_set<Node*>* nodes);

    // PARA OBJETOS
    void attachObject(RenderObject* const object);
    void attachObject(Ogre::MovableObject* const object);   // solo para ManualObject
    void detachObject(RenderObject* const object);

    void lookAt(const Vector3 target);
    void setDirection(const Vector3 dir);

    Node(Ogre::SceneManager* const sceneManager, const Vector3 pos, const Vector3 scale, Node* const parent = nullptr);

public:
    virtual ~Node();

    void removeChild(INode* const node) override;
    void removeAttachedParent() override;
    void addChild(INode* const child) override;

    // mover, rotar y escalar
    void setPosition(const Vector3 pos) override;
    void translate(const Vector3 t) override;
    void setScale(const Vector3 sc) override;
    /*Combina la escala actual con la ya existente (sumando)
    Por ejemplo, si el object tiene una escala de Vector3(2,2,2) y se llama a este
    metodo con una escala de Vector3(3,3,3), la escala resultante del objeto seria
    Vector3(5,5,5)*/
    void scale(const Vector3 s) override;
    void setRotation(const Vector3 rot) override;
    // TODO: terminar rotate y rotaciones en los ejes basicos (pitch, yaw, roll)
    void rotate(const Vector3 r);

    void roll(const float angle) override;
    void yaw(const float angle) override;
    void pitch(const float angle) override;

    std::vector<INode*> getChildren() override;
    void getAllChildrenAux(std::vector<INode*>& allChildren) override;
    std::vector<INode*> getAllChildren() override;

    void setParent(INode* const parent) override;

    virtual inline INode* getParent() const override { return parent; }
};
}