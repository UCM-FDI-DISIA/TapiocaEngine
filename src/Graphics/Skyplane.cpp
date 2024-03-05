#include "Skyplane.h"
#include <Ogre.h>¡
#include "OgreVector3.h"

namespace Tapioca {

Skyplane::Skyplane(Ogre::SceneManager* scnMgr, Node* node, std::string material)
    : RenderObject(node, scnMgr), material(material) {
    scnM = scnMgr;
    scnM->setSkyPlane(true, Ogre::Plane(Ogre::Vector3::UNIT_Z, -20), material, 0.3, 1, true, 1.0, 100, 100);
}

void Tapioca::Skyplane::setVisible(bool visible) {
    scnM->setSkyPlane(visible, Ogre::Plane(Ogre::Vector3::UNIT_Z, -20), material, 0.3, 1, true, 1.0, 100, 100);
}
}
