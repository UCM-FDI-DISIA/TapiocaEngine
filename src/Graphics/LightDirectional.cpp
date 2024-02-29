#include <Ogre.h>
#include "LightDirectional.h"
#include "Node.h"
#include "Utilities/Vector3.h"

Tapioca::LightDirectional::LightDirectional(
    Ogre::SceneManager* sceneManager, Tapioca::Node* node, Vector4 color, Vector3 direction)
    : Light(sceneManager, node, color) {

    mLight->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);
    node->setDirection(direction);
}