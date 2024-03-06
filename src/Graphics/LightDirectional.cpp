#include "LightDirectional.h"
#include <Ogre.h>
#include "Node.h"
#include "Utilities/Vector3.h"

namespace Tapioca {

LightDirectional::LightDirectional(Ogre::SceneManager* const sceneManager, Node* const node, const Vector4 color,
                                   const Vector3 direction)
    : Light(sceneManager, node, color) 
{
    mLight->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);
    node->setDirection(direction);
}

void LightDirectional::setDirection(const Vector3 direction) { node->setDirection(direction); }

}