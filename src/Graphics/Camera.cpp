#include "Camera.h"
#include <Ogre.h>
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"
#include "Node.h"

Tapioca::Camera::Camera(Ogre::SceneManager* scnMgr, Node* node, std::string name, Vector3 targetToLook, float nearDist,
    float farDist, bool autoAspectRatio, float aspectRatio)
    : RenderObject(node)
    , mCam(scnMgr->createCamera(name)) {

    init(mCam);

    lookAt(targetToLook);
    setNearClipDistance(nearDist);
    if (autoAspectRatio) {
        mCam->setAutoAspectRatio(true);
    } else {
        mCam->setAspectRatio(aspectRatio);
    }
}

void Tapioca::Camera::lookAt(Vector3 targetToLook) { node->lookAt(targetToLook); }

void Tapioca::Camera::setDirection(Vector3 dir) { node->setDirection(dir); }

void Tapioca::Camera::setNearClipDistance(float dist) { mCam->setNearClipDistance(dist); }

//void Tapioca::Camera::setFOVY(float radians) { mCam->setFOVy(Ogre::Radian(radians)); }

void Tapioca::Camera::setFOVY(float degrees) { mCam->setFOVy(Ogre::Radian(Ogre::Degree(degrees))); }

void Tapioca::Camera::setFarClipDistance(float dist) { mCam->setFarClipDistance(dist); }