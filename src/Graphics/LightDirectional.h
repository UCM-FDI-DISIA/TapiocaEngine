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

class TAPIOCA_API LightDirectional : public Light {
private:
    friend GraphicsEngine;

    // SOLO NECESITA UNA DIRECCION, PERO NO UNA POSICION (LA POSICION NO AFECTA PARA NADA)
    // SIMULA RAYOS DE LUZ DESDE UNA POSICION LEJANA (COMO EL SOL)
    LightDirectional(Ogre::SceneManager* const sceneManager, Node* const node, const Vector4 color,
                     const Vector3 direction);

public:
    void setDirection(const Vector3 direction);
};
}