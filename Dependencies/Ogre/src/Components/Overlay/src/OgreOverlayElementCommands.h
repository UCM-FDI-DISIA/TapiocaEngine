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
#ifndef __OverlayElementEmitterCommands_H__
#define __OverlayElementEmitterCommands_H__

#include "OgreOverlayPrerequisites.h"
#include "OgreStringInterface.h"

#include "OgreOverlayElement.h"

namespace Ogre  {

    /** \addtogroup Optional
    *  @{
    */
    /** \addtogroup Overlays
    *  @{
    */

    namespace OverlayElementCommands {
    typedef SimpleParamCommand<OverlayElement, Real, &OverlayElement::getLeft, &OverlayElement::setLeft>
        CmdLeft;

    typedef SimpleParamCommand<OverlayElement, Real, &OverlayElement::getTop, &OverlayElement::setTop>
        CmdTop;

    typedef SimpleParamCommand<OverlayElement, Real, &OverlayElement::getWidth, &OverlayElement::setWidth>
        CmdWidth;

    typedef SimpleParamCommand<OverlayElement, Real, &OverlayElement::getHeight, &OverlayElement::setHeight>
        CmdHeight;

        /// Command object for OverlayElement  - see ParamCommand 
        class CmdMaterial : public ParamCommand
        {
        public:
            String doGet(const void* target) const override;
            void doSet(void* target, const String& val) override;
        };
        /// Command object for OverlayElement  - see ParamCommand 
        class CmdCaption : public ParamCommand
        {
        public:
            String doGet(const void* target) const override;
            void doSet(void* target, const String& val) override;
        };
        /// Command object for OverlayElement  - see ParamCommand 
        class CmdMetricsMode : public ParamCommand
        {
        public:
            String doGet(const void* target) const override;
            void doSet(void* target, const String& val) override;
        };
        /// Command object for OverlayElement  - see ParamCommand 
        class CmdHorizontalAlign : public ParamCommand
        {
        public:
            String doGet(const void* target) const override;
            void doSet(void* target, const String& val) override;
        };
        /// Command object for OverlayElement  - see ParamCommand 
        class CmdVerticalAlign : public ParamCommand
        {
        public:
            String doGet(const void* target) const override;
            void doSet(void* target, const String& val) override;
        };
        /// Command object for OverlayElement  - see ParamCommand 
        class CmdVisible : public ParamCommand
        {
        public:
            String doGet(const void* target) const override;
            void doSet(void* target, const String& val) override;
        };

    }
    /** @} */
    /** @} */
}

#endif


