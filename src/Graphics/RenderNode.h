#pragma once
#include <unordered_set>
#include <vector>
#include "defs.h"

namespace Ogre {
class SceneManager;
class SceneNode;
class MovableObject;
}

namespace Tapioca {
class Vector3;
class Quaternion;
class RenderObject;
class GraphicsManager;
class Camera;
class LightDirectional;
class LightSpotlight;
class LightRectlight;

class TAPIOCA_API RenderNode {
private:
    friend GraphicsManager;
    friend RenderObject;
    friend Camera;
    friend LightDirectional;
    friend LightSpotlight;
    friend LightRectlight;

    Ogre::SceneManager* sceneManager;       // Puntero al manager de escenas de ogre
    Ogre::SceneNode* node;                  // Nodo para renderizado
    RenderObject* object;                   // Objecto para renderizar

    // PARA OBJETOS
    void attachObject(RenderObject* const object);
    void attachObject(Ogre::MovableObject* const object);   // solo para ManualObject
    void detachObject();

    void lookAt(const Vector3 target);
    void setDirection(const Vector3 dir);

    RenderNode(Ogre::SceneManager* const sceneManager, const Vector3 pos, const Vector3 scale);

public:
    virtual ~RenderNode();

    // mover, rotar y escalar
    void setPosition(const Vector3 pos);
    void translate(const Vector3 t);
    void setScale(const Vector3 sc);
    /*Combina la escala actual con la ya existente (sumando)
    Por ejemplo, si el object tiene una escala de Vector3(2,2,2) y se llama a este
    metodo con una escala de Vector3(3,3,3), la escala resultante del objeto seria
    Vector3(5,5,5)*/
    void scale(const Vector3 s);
    void setRotation(const Quaternion q);
    void setRotation(const Vector3 rot);
    void rotate(const Vector3 r);

    void roll(const float angle);
    void yaw(const float angle);
    void pitch(const float angle);

    Vector3 getPosition() const;
};
}