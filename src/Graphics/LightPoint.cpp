#include <Ogre.h>
#include "LightPoint.h"

Tapioca::LightPoint::LightPoint(Ogre::SceneManager* sceneManager, Tapioca::Node* node, Vector4 color)
    : Light(sceneManager, node, color) {

    mLight->setType(Ogre::Light::LightTypes::LT_POINT);
}