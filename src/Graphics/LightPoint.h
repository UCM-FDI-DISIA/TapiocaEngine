#pragma once
#include "Light.h"
#include "Utilities/defs.h"

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
class RenderNode;
class GraphicsEngine;
class Vector4;

class TAPIOCA_API LightPoint : public Light {
private:
    friend GraphicsEngine;

    // SOLO NECESITA UNA POSICION, ILUMINA IGUAL EN TODAS DIRECCIONES
    LightPoint(Ogre::SceneManager* const sceneManager, RenderNode* const node, const Vector4 color);

public:
};
}