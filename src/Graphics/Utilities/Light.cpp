#include "Light.h"
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "checkML.h"

namespace Tapioca {
Light::Light(Ogre::SceneManager* const scnMngr, RenderNode* const node, const Vector4 color)
    : RenderObject(node, scnMngr), mLight(scnMngr->createLight()), mColor(color) 
{
    init(mLight);
    mLight->setDiffuseColour(Ogre::ColourValue(color.x, color.y, color.z, color.w));
    mLight->setSpecularColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f));

    // Elige si la luz puede proyectar sombras o no
    mLight->setCastShadows(false);
}

void Light::setColor(const Vector4 color) {
    mLight->setDiffuseColour(Ogre::ColourValue(color.x, color.y, color.z, color.w));
    mColor = color;
}

void Light::setPowerScale(const float power) { mLight->setPowerScale(power); }
}
