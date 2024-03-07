#include "LightPoint.h"
#include <Ogre.h>

namespace Tapioca {

LightPoint::LightPoint(Ogre::SceneManager* const sceneManager, RenderNode* const node, const Vector4 color)
    : Light(sceneManager, node, color) {
    mLight->setType(Ogre::Light::LightTypes::LT_POINT);
}

}