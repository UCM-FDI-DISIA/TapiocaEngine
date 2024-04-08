#include "Light.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

namespace Tapioca {

Light::Light(Ogre::SceneManager* const scnMgr, RenderNode* const node, const Vector4 color)
    : RenderObject(node, scnMgr), mLight(scnMgr->createLight()), mColor(color) {
    init(mLight);
    mLight->setDiffuseColour(Ogre::ColourValue(color.x, color.y, color.z, color.w));

    // elegir si la luz puede proyectar sombras o no
    mLight->setCastShadows(false);
}

void Light::setColor(const Vector4 color) {
    mLight->setDiffuseColour(Ogre::ColourValue(color.x, color.y, color.z, color.w));
    mColor = color;
}

void Light::setPowerScale(const float power) { mLight->setPowerScale(power); }
}
