/*
 -----------------------------------------------------------------------------
 This source file is part of OGRE
 (Object-oriented Graphics Rendering Engine)
 For the latest info, see http://www.ogre3d.org/

 Copyright (c) 2000-2014 Torus Knot Software Ltd

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
#ifndef __SdkCameraMan_H__
#define __SdkCameraMan_H__

#include "OgreBitesPrerequisites.h"
#include "OgreCamera.h"
#include "OgreSceneNode.h"
#include "OgreFrameListener.h"

#include "OgreInput.h"

namespace OgreBites
{
    /** \addtogroup Optional
    *  @{
    */
    /** \addtogroup Bites
    *  @{
    */
    enum CameraStyle   /// enumerator values for different styles of camera movement
    {
        CS_FREELOOK,
        CS_ORBIT,
        CS_MANUAL
    };

    /**
    Utility class for controlling the camera in samples.
    */
    class _OgreBitesExport CameraMan : public InputListener
    {
    public:
        CameraMan(Ogre::SceneNode* cam);

        /**
        Swaps the camera on our camera man for another camera.
        */
        void setCamera(Ogre::SceneNode* cam);

        Ogre::SceneNode* getCamera()
        {
            return mCamera;
        }

        /**
        Sets the target we will revolve around. Only applies for orbit style.
        */
        virtual void setTarget(Ogre::SceneNode* target);

        Ogre::SceneNode* getTarget()
        {
            return mTarget;
        }

        /**
        Sets the spatial offset from the target. Only applies for orbit style.
        */
        void setYawPitchDist(const Ogre::Radian& yaw, const Ogre::Radian& pitch, Ogre::Real dist);

        /**
        Sets the camera's top speed. Only applies for free-look style.
        */
        void setTopSpeed(Ogre::Real topSpeed)
        {
            mTopSpeed = topSpeed;
        }

        Ogre::Real getTopSpeed()
        {
            return mTopSpeed;
        }

        /**
        Sets the movement style of our camera man.
        */
        virtual void setStyle(CameraStyle style);

        CameraStyle getStyle()
        {
            return mStyle;
        }

        /**
        Manually stops the camera when in free-look mode.
        */
        void manualStop();

        void frameRendered(const Ogre::FrameEvent& evt) override;

        /**
        Processes key presses for free-look style movement.
        */
        bool keyPressed(const KeyboardEvent& evt) override;

        /**
        Processes key releases for free-look style movement.
        */
        bool keyReleased(const KeyboardEvent& evt) override;

        /**
        Processes mouse movement differently for each style.
        */
        bool mouseMoved(const MouseMotionEvent& evt) override;

        bool mouseWheelRolled(const MouseWheelEvent& evt) override;

        /**
        Processes mouse presses. Only applies for orbit style.
        Left button is for orbiting, and right button is for zooming.
        */
        bool mousePressed(const MouseButtonEvent& evt) override;

        /**
        Processes mouse releases. Only applies for orbit style.
        Left button is for orbiting, and right button is for zooming.
        */
        bool mouseReleased(const MouseButtonEvent& evt) override;

        /**
         * fix the yaw axis to be Vector3::UNIT_Y of the parent node (tabletop mode)
         * 
         * otherwise the yaw axis can change freely
         */
        void setFixedYaw(bool fixed)
        {
            mYawSpace = fixed ? Ogre::Node::TS_PARENT : Ogre::Node::TS_LOCAL;
        }

        void setPivotOffset(const Ogre::Vector3& offset);
    protected:
        Ogre::Real getDistToTarget();
        Ogre::Node::TransformSpace mYawSpace;
        Ogre::SceneNode* mCamera;
        CameraStyle mStyle;
        Ogre::SceneNode* mTarget;
        bool mOrbiting;
        bool mMoving;
        Ogre::Real mTopSpeed;
        Ogre::Vector3 mVelocity;
        bool mGoingForward;
        bool mGoingBack;
        bool mGoingLeft;
        bool mGoingRight;
        bool mGoingUp;
        bool mGoingDown;
        bool mFastMove;
        Ogre::Vector3 mOffset;
    };
    /** @} */
    /** @} */
}
#endif
