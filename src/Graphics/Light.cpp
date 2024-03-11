#include "Light.h"
#include <Ogre.h>

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
