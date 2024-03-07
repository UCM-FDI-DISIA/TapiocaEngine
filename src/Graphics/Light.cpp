#include "Light.h"
#include <Ogre.h>

namespace Tapioca {

Light::Light(Ogre::SceneManager* const scnMgr, RenderNode* const node, const Vector4 color)
    : RenderObject(node, scnMgr), mLight(scnMgr->createLight()), mColor(color) {
    init(mLight);
    mLight->setDiffuseColour(Ogre::ColourValue(color.x, color.y, color.z, color.w));
}

void Light::setColor(const Vector4 color) {
    mLight->setDiffuseColour(Ogre::ColourValue(color.x, color.y, color.z, color.w));
    mColor = color;
}
}
