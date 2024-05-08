#include "LightPoint.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

namespace Tapioca {
LightPoint::LightPoint(Ogre::SceneManager* const scnMngr, RenderNode* const node, const Vector4 color)
    : Light(scnMngr, node, color) 
{
    mLight->setType(Ogre::Light::LightTypes::LT_POINT);
}

void LightPoint::setAttenuation(float maximumRange, float constant, float linear, float quadratic) {
    mLight->setAttenuation(maximumRange, constant, linear, quadratic);
}
}
