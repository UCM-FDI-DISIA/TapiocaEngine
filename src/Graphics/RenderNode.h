#pragma once
#include <unordered_set>
#include <vector>
#include "Utilities/defs.h"

namespace Ogre {
class SceneManager;
class SceneNode;
class MovableObject;
}

namespace Tapioca {
class Vector3;
class RenderObject;
class GraphicsEngine;
class Camera;
class LightDirectional;

class TAPIOCA_API RenderNode {
private:
    friend GraphicsEngine;
    friend RenderObject;
    friend Camera;
    friend LightDirectional;

    Ogre::SceneManager* sceneManager;
    Ogre::SceneNode* node;
    RenderObject* object;

    // si es nullptr, quiere decir que el padre es el nodo root
    //RenderNode* parent;
    //std::unordered_set<RenderNode*> children;
    //std::unordered_set<RenderObject*> objects;

    //void removeParent();

    // PARA OBJETOS
    void attachObject(RenderObject* const object);
    void attachObject(Ogre::MovableObject* const object);   // solo para ManualObject
    void detachObject();

    void lookAt(const Vector3 target);
    void setDirection(const Vector3 dir);

    RenderNode(Ogre::SceneManager* const sceneManager, const Vector3 pos, const Vector3 scale);

public:
    virtual ~RenderNode();

    /*void removeChild(RenderNode* const node);
    void removeAttachedParent();
    void addChild(RenderNode* const child);*/

    // mover, rotar y escalar
    void setPosition(const Vector3 pos);
    void translate(const Vector3 t);
    void setScale(const Vector3 sc);
    /*Combina la escala actual con la ya existente (sumando)
    Por ejemplo, si el object tiene una escala de Vector3(2,2,2) y se llama a este
    metodo con una escala de Vector3(3,3,3), la escala resultante del objeto seria
    Vector3(5,5,5)*/
    void scale(const Vector3 s);
    void setRotation(const Vector3 rot);
    void rotate(const Vector3 r);

    void roll(const float angle);
    void yaw(const float angle);
    void pitch(const float angle);

    /*std::vector<RenderNode*> getChildren();
    void getAllChildrenAux(std::vector<RenderNode*>& allChildren);
    std::vector<RenderNode*> getAllChildren();

    void setParent(RenderNode* const parent);

    inline RenderNode* getParent() const override { return parent; }*/
};
}