#include "Camera.h"
#include <Ogre.h>
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"
#include "Node.h"


Tapioca::Camera::Camera(Ogre::SceneManager* scnMgr, Node* n, std::string name, Vector3 look, int clipdst, bool autoAR) {
    mCam = scnMgr->createCamera(name);
    mNode = n;
    mNode->getSceneNode()->lookAt(Ogre::Vector3(look.x, look.y, look.z),Ogre::Node::TS_PARENT);
    mCam->setNearClipDistance(clipdst);
    mCam->setAspectRatio(autoAR);
    mNode->getSceneNode()->attachObject(mCam);

}

void Tapioca::Camera::setLookAt(Vector3 look) { 
     mNode->getSceneNode()->lookAt(Ogre::Vector3(look.x, look.y, look.z), Ogre::Node::TS_PARENT);
}

void Tapioca::Camera::setNearClipDistance(float dst) { 
     mCam->setNearClipDistance(dst); }


