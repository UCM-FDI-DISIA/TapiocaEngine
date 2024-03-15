#pragma once
#include "Light.h"
#include "defs.h"

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
class RenderNode;
class GraphicsManager;
class Vector4;

/*
* @brief Wrapper de la clase LightPoint de Ogre.
*/
class TAPIOCA_API LightPoint : public Light {
private:
    friend GraphicsManager;

    /*
    * @brief Constructora de la clase LightPoint.
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param colour Color de la luz
    */
    LightPoint(Ogre::SceneManager* const sceneManager, RenderNode* const node, const Vector4 color);

public:
};
}