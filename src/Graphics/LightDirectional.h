#pragma once
#include "Light.h"

namespace Ogre {
class SceneManager;
}
namespace Tapioca {
class Node;
class GraphicsEngine;
class Vector4;
class Vector3;
}

namespace Tapioca {
class LightDirectional : public Light {
public:
    friend GraphicsEngine;

private:
    // SOLO NECESITA UNA DIRECCION, PERO NO UNA POSICION (LA POSICION NO AFECTA PARA NADA)
    // SIMULA RAYOS DE LUZ DESDE UNA POSICION LEJANA (COMO EL SOL)
    LightDirectional(Ogre::SceneManager* sceneManager, Node* node, Vector4 color, Vector3 direction);

public:
};
}