#pragma once
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"   //si no me obliga a guardar punteros
namespace Ogre {
class SceneManager;
class Light;
}
namespace Tapioca {
class Vector3;
class Vector4;
class GraphicsEngine;
enum LightType;
class Node;
}
namespace Tapioca {
class Light {


public:
    friend class Node;
    friend GraphicsEngine;


private:
    Ogre::Light* mLight;
    LightType mLightType;
    Node* mNode;
    Tapioca::Vector4 mColor;
    Tapioca::Vector3 mDirection;
    Light(Ogre::SceneManager* scmngr, Tapioca::Node* n, LightType lt, Vector4 color, Vector3 direction);

public:
    void setColor(Vector4 color);
    void setDirection(Vector3 dir);
};
}