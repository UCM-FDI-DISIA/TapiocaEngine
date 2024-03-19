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

Light::Light(Ogre::SceneManager* const scnMgr, RenderNode* const node, const Vector4 colour)
    : RenderObject(node, scnMgr), mLight(scnMgr->createLight()), mColour(colour) {
    init(mLight);
    mLight->setDiffuseColour(Ogre::ColourValue(colour.x, colour.y, colour.z, colour.w));
}

void Light::setColor(const Vector4 colour) {
    mLight->setDiffuseColour(Ogre::ColourValue(colour.x, colour.y, colour.z, colour.w));
    mColour = colour;
}
}
