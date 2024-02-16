/*-------------------------------------------------------------------------
This source file is a part of OGRE
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
THE SOFTWARE
-------------------------------------------------------------------------*/

#ifndef _TextAreaOverlayElement_H__
#define _TextAreaOverlayElement_H__

#include "OgreOverlayElement.h"
#include "OgreRenderOperation.h"
#include "OgreFont.h"

namespace Ogre
{
    /** \addtogroup Optional
    *  @{
    */
    /** \addtogroup Overlays
    *  @{
    */
    /** This class implements an overlay element which contains simple unformatted text.
    */
    class _OgreOverlayExport TextAreaOverlayElement : public OverlayElement
    {
    public:
        enum Alignment
        {
            Left,
            Right,
            Center
        };

    public:
        /** Constructor. */
        TextAreaOverlayElement(const String& name);
        virtual ~TextAreaOverlayElement();

        void initialise(void) override;

        /** @copydoc OverlayElement::_releaseManualHardwareResources */
        void _releaseManualHardwareResources() override;
        /** @copydoc OverlayElement::_restoreManualHardwareResources */
        void _restoreManualHardwareResources() override;

        void setCaption(const DisplayString& text) override;

        void setCharHeight( Real height );
        Real getCharHeight() const;

        void setSpaceWidth( Real width );
        Real getSpaceWidth() const;

        void setFontName( const String& font, const String& group = DEFAULT_RESOURCE_GROUP );
        /// @deprecated use getFont()
        OGRE_DEPRECATED const String& getFontName() const;

        const FontPtr& getFont() const {
            return mFont;
        }

        const String& getTypeName(void) const override;
        const MaterialPtr& getMaterial(void) const override;
        void getRenderOperation(RenderOperation& op) override;

        /** Sets the colour of the text. 

            This method establishes a constant colour for 
            the entire text. Also see setColourBottom and 
            setColourTop which allow you to set a colour gradient.
        */
        void setColour(const ColourValue& col) override;

        /** Gets the colour of the text. */
        const ColourValue& getColour(void) const override;
        /** Sets the colour of the bottom of the letters.

            By setting a separate top and bottom colour, you
            can create a text area which has a graduated colour
            effect to it.
        */
        void setColourBottom(const ColourValue& col);
        /** Gets the colour of the bottom of the letters. */
        const ColourValue& getColourBottom(void) const;
        /** Sets the colour of the top of the letters.

            By setting a separate top and bottom colour, you
            can create a text area which has a graduated colour
            effect to it.
        */
        void setColourTop(const ColourValue& col);
        /** Gets the colour of the top of the letters. */
        const ColourValue& getColourTop(void) const;

        inline void setAlignment( Alignment a )
        {
            mAlignment = a;
            mGeomPositionsOutOfDate = true;
        }
        inline Alignment getAlignment() const
        {
            return mAlignment;
        }

        /** Overridden from OverlayElement */
        void setMetricsMode(GuiMetricsMode gmm) override;

        /** Overridden from OverlayElement */
        void _update(void) override;
    private:
        /// The text alignment
        Alignment mAlignment;

        /// Flag indicating if this panel should be visual or just group things
        bool mTransparent;

        /// Render operation
        RenderOperation mRenderOp;

        /// Method for setting up base parameters for this class
        void addBaseParameters(void) override;

        static String msTypeName;

        FontPtr mFont;
        Real mCharHeight;
        ushort mPixelCharHeight;
        Real mSpaceWidth;
        ushort mPixelSpaceWidth;
        size_t mAllocSize;
        Real mViewportAspectCoef;

        /// Colours to use for the vertices
        ColourValue mColourBottom;
        ColourValue mColourTop;
        bool mColoursChanged;


        /// Internal method to allocate memory, only reallocates when necessary
        void checkMemoryAllocation( size_t numChars );
        /// Inherited function
        void updatePositionGeometry() override;
        /// Inherited function
        void updateTextureGeometry() override;
        /// Updates vertex colours
        virtual void updateColours(void);
    };
    /** @} */
    /** @} */
}

#endif

