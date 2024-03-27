#include "LightDirectional.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "RenderNode.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
LightDirectional::LightDirectional(Ogre::SceneManager* const scnManager, RenderNode* const node, const Vector4 color,
                                   const Vector3 direction)
    : Light(scnManager, node, color) {
    mLight->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);
    node->setDirection(direction);
}

void LightDirectional::setDirection(const Vector3 direction) { node->setDirection(direction); }
}