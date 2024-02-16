/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2008 Renato Araujo Oliveira Filho <renatox@gmail.com>
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

#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreRoot.h"

#include "OgreX11EGLSupport.h"
#include "OgreX11EGLWindow.h"
#include "OgreX11.h"

#include "OgreGLUtil.h"

namespace Ogre {
    GLNativeSupport* getGLSupport(int profile)
    {
        return new X11EGLSupport(profile);
    }

    X11EGLSupport::X11EGLSupport(int profile) : EGLSupport(profile)
    {
        // A connection that might be shared with the application for GL rendering:
        mGLDisplay = getGLDisplay();

        // A connection that is NOT shared to enable independent event processing:
        mNativeDisplay = getNativeDisplay();


        if(mNativeDisplay == EGL_DEFAULT_DISPLAY)
        {
            // fake video mode
            mCurrentMode.width = 0;
            mCurrentMode.height = 0;
            mCurrentMode.refreshRate = 0;
            mVideoModes.push_back(mCurrentMode);
        }
        else
        {
            getXVideoModes(mNativeDisplay, mCurrentMode, mVideoModes);
        }

        if(mVideoModes.empty()) // none of the above worked
        {
            mCurrentMode.width = DisplayWidth(mNativeDisplay, DefaultScreen(mNativeDisplay));
            mCurrentMode.height = DisplayHeight(mNativeDisplay, DefaultScreen(mNativeDisplay));
            mCurrentMode.refreshRate = 0;
            mVideoModes.push_back(mCurrentMode);
        }

        mOriginalMode = mCurrentMode;

        EGLConfig *glConfigs;
        int config, nConfigs = 0;

        glConfigs = chooseGLConfig(NULL, &nConfigs);

        for (config = 0; config < nConfigs; config++)
        {
            int caveat, samples;

            getGLConfigAttrib(glConfigs[config], EGL_CONFIG_CAVEAT, &caveat);

            if (caveat != EGL_SLOW_CONFIG)
            {
                getGLConfigAttrib(glConfigs[config], EGL_SAMPLES, &samples);
                mFSAALevels.push_back(samples);
            }
        }

        free(glConfigs);
    }

    X11EGLSupport::~X11EGLSupport()
    {
        if (mNativeDisplay)
        {
            XCloseDisplay((Display*)mNativeDisplay);
        }

        if (mGLDisplay)
        {
            eglTerminate(mGLDisplay);
        }
    }

    NativeDisplayType X11EGLSupport::getNativeDisplay()
    {
        if (!mNativeDisplay)
        {
            mNativeDisplay = getXDisplay(NULL, mAtomDeleteWindow, mAtomFullScreen, mAtomState);

            if (mNativeDisplay == EGL_DEFAULT_DISPLAY)
            {
                LogManager::getSingleton().logWarning("Couldn't open X display");
                return mNativeDisplay;
            }
        }

        return mNativeDisplay;
    }

    XVisualInfo *X11EGLSupport::getVisualFromFBConfig(::EGLConfig glConfig)
    {
        XVisualInfo *vi, tmp;
        int vid, n;
        ::EGLDisplay glDisplay;

        glDisplay = getGLDisplay();
        mNativeDisplay = getNativeDisplay();

        if (eglGetConfigAttrib(glDisplay, glConfig, EGL_NATIVE_VISUAL_ID, &vid) == EGL_FALSE)
        {
            OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "Fail to get VISUAL_ID from glConfig");
            return 0;
        }
        EGL_CHECK_ERROR

        if (vid == 0)
        {
            const int screen_number = DefaultScreen(mNativeDisplay);
            Visual *v = DefaultVisual((Display*)mNativeDisplay, screen_number);
            vid = XVisualIDFromVisual(v);
        }

        tmp.visualid = vid;
        vi = 0;
        vi = XGetVisualInfo((Display*)mNativeDisplay,
                            VisualIDMask,
                            &tmp, &n);
        if (vi == 0)
        {
            OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "Fail to get X11 VISUAL");
            return 0;
        }

        return vi;
    }

    RenderWindow* X11EGLSupport::newWindow(const String &name,
                                        unsigned int width, unsigned int height,
                                        bool fullScreen,
                                        const NameValuePairList *miscParams)
    {
        EGLWindow* window =
            mNativeDisplay == EGL_DEFAULT_DISPLAY ? new EGLWindow(this) : new X11EGLWindow(this);
        window->create(name, width, height, fullScreen, miscParams);

        return window;
    }

    //X11EGLSupport::getGLDisplay sets up the native variable
    //then calls EGLSupport::getGLDisplay
    EGLDisplay X11EGLSupport::getGLDisplay()
    {
        if (!mGLDisplay)
        {
            if(!mNativeDisplay)
                mNativeDisplay = getNativeDisplay();
            return EGLSupport::getGLDisplay();
        }
        return mGLDisplay;
    }

}

