#include "LightSpotlight.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "Utilities/Vector3.h"
#include "RenderNode.h"

namespace Tapioca {
LightSpotlight::LightSpotlight(Ogre::SceneManager* sceneManager, RenderNode* node, Vector4 color, Vector3 direction)
    : Light(sceneManager, node, color) {

    mLight->setType(Ogre::Light::LT_SPOTLIGHT);

    node->setDirection(direction);
}

void LightSpotlight::setInnerAngle(float degrees) {
    mLight->setSpotlightInnerAngle(Ogre::Radian(Ogre::Degree(degrees)));
}

void LightSpotlight::setOuterAngle(float degrees) {
    mLight->setSpotlightOuterAngle(Ogre::Radian(Ogre::Degree(degrees)));
}

void LightSpotlight::setFalloff(float falloff) { mLight->setSpotlightFalloff(falloff); }

void LightSpotlight::setNearClipDistance(float distance) { mLight->setSpotlightNearClipDistance(distance); }

void LightSpotlight::setAttenuation(float maximumRange, float constant, float linear, float quadratic) {
    mLight->setAttenuation(maximumRange, constant, linear, quadratic);
}

void LightSpotlight::setDirection(Vector3 direction) { node->setDirection(direction); }
}