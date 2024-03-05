#include "LightPoint.h"
#include <Ogre.h>

namespace Tapioca {

LightPoint::LightPoint(Ogre::SceneManager* sceneManager, Node* node, Vector4 color)
    : Light(sceneManager, node, color)
{
    mLight->setType(Ogre::Light::LightTypes::LT_POINT);
}

}