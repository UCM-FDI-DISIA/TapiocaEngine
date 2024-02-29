#pragma once
#include "Light.h"

namespace Ogre {
class SceneManager;
}
namespace Tapioca {
class Node;
class GraphicsEngine;
class Vector4;
}

namespace Tapioca {
class LightPoint : public Light {
public:
    friend GraphicsEngine;

private:
    // SOLO NECESITA UNA POSICION, ILUMINA IGUAL EN TODAS DIRECCIONES
    LightPoint(Ogre::SceneManager* sceneManager, Node* node, Vector4 color);

public:
};
}