#include "Light.h"
#include "Node.h"
#include "Graphicsdef.h"
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"
#include <Ogre.h>


Tapioca::Light::Light(Ogre::SceneManager* scmngr, Node* n, LightType lt, Vector4 color, Vector3 direction) {
    mNode = n;
    Ogre::Light* l= scmngr->createLight();
    switch (lt) {
    case Tapioca::POINT:
        l->setType(Ogre::Light::LT_POINT);
        break;
    case Tapioca::SPOTLIGHT:
        l->setType(Ogre::Light::LT_SPOTLIGHT);
        break;
    case Tapioca::DIRECTIONAL:
        l->setType(Ogre::Light::LT_DIRECTIONAL);
        break;
    default:
        l->setType(Ogre::Light::LT_POINT);   //
        break;
    }
    mLight = l;
    mLight->setDiffuseColour(Ogre::ColourValue(color.x, color.y, color.z, color.w));
    mColor = color;
    setDirection(direction);
    mDirection = direction;
    mNode->getSceneNode()->attachObject(l);
}

void Tapioca::Light::setColor(Vector4 color) {
    mLight->setDiffuseColour(Ogre::ColourValue(color.x, color.y, color.z, color.w));
    mColor = color;
}

void Tapioca::Light::setDirection(Vector3 dir) { mNode->getSceneNode()->setDirection(dir.x,dir.y,dir.z); }