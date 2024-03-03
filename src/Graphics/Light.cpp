#include "Light.h"
#include <Ogre.h>

Tapioca::Light::Light(Ogre::SceneManager* scnMgr, Node* node, Vector4 color)
    : RenderObject(node, scnMgr), mLight(scnMgr->createLight()), mColor(color) {

    init(mLight);
    mLight->setDiffuseColour(Ogre::ColourValue(color.x, color.y, color.z, color.w));
}

void Tapioca::Light::setColor(Vector4 color) {
    mLight->setDiffuseColour(Ogre::ColourValue(color.x, color.y, color.z, color.w));
    mColor = color;
}