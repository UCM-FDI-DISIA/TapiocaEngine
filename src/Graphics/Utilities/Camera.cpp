#include "Camera.h"
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"
#include "RenderNode.h"
#include "checkML.h"

namespace Tapioca {
Camera::Camera(Ogre::SceneManager* const scnMngr, RenderNode* const node, std::string const& name,
    const Vector3 targetToLook, const float nearDist, const float farDist, const bool autoAspectRatio,
    const float aspectRatio) : RenderObject(node, scnMngr), mCam(scnMngr->createCamera(name)), 
    autoAspectRatio(autoAspectRatio) 
{
    init(mCam);

    lookAt(targetToLook);
    setNearClipDistance(nearDist);
    setFarClipDistance(farDist);
    if (autoAspectRatio) mCam->setAutoAspectRatio(true);
    else mCam->setAspectRatio(aspectRatio);
}

void Camera::lookAt(const Vector3 targetToLook) { node->lookAt(targetToLook); }

void Camera::setDirection(const Vector3 dir) { node->setDirection(dir); }

void Camera::setNearClipDistance(const float dist) { mCam->setNearClipDistance(dist); }

void Camera::setFOVYRadians(const float radians) { mCam->setFOVy(Ogre::Radian(radians)); }

void Camera::setFOVYDegrees(const float degrees) { mCam->setFOVy(Ogre::Radian(Ogre::Degree(degrees))); }

void Camera::setFarClipDistance(const float dist) { mCam->setFarClipDistance(dist); }

void Camera::setAspectRatio(const float aspectRatio) {
    if (!autoAspectRatio) mCam->setAspectRatio(aspectRatio);
}

float Camera::getNearClipDistance() const { return mCam->getNearClipDistance(); }

float Camera::getFarClipDistance() const { return mCam->getFarClipDistance(); }
}