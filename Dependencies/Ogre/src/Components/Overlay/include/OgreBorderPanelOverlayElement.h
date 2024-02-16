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

#ifndef __BorderPanelOverlayElement_H__
#define __BorderPanelOverlayElement_H__

#include "OgrePanelOverlayElement.h"

namespace Ogre {
    /** \addtogroup Optional
    *  @{
    */
    /** \addtogroup Overlays
    *  @{
    */

    class BorderRenderable;
    
    /** A specialisation of the PanelOverlayElement to provide a panel with a border.

        Whilst the standard panel can use a single tiled material, this class allows
        panels with a tileable backdrop plus a border texture. This is handy for large
        panels that are too big to use a single large texture with a border, or
        for multiple different size panels where you want the border a constant width
        but the center to repeat.
    @par
        In addition to the usual PanelOverlayElement properties, this class has a 'border
        material', which specifies the material used for the edges of the panel,
        a border width (which can either be constant all the way around, or specified
        per edge), and the texture coordinates for each of the border sections.
    */
    class _OgreOverlayExport BorderPanelOverlayElement : public PanelOverlayElement
    {
        friend class BorderRenderable;
    public:
        /** Constructor */
        BorderPanelOverlayElement(const String& name);
        virtual ~BorderPanelOverlayElement();

        void initialise(void) override;

        /** @copydoc OverlayElement::_releaseManualHardwareResources */
        void _releaseManualHardwareResources() override;
        /** @copydoc OverlayElement::_restoreManualHardwareResources */
        void _restoreManualHardwareResources() override;

        const String& getTypeName(void) const override;
        /** @name Border Size
            Remember that the dimensions specified here are in relation to the size of
            the screen, so 0.1 is 1/10th of the screen width or height. Also note that because
            most screen resolutions are 1.333:1 width:height ratio that using the same
            border size will look slightly bigger across than up.
         */
        /// @{
        /** Sets the size of the border.

            This method sets a constant size for all borders.
        @param size The size of the border as a factor of the screen dimensions ie 0.2 is one-fifth
            of the screen size.
        */
        void setBorderSize(Real size);

        /** Sets the size of the border, with different sizes for vertical and horizontal borders.

            This method sets a size for the side and top / bottom borders separately.
        @param sides The size of the side borders as a factor of the screen dimensions ie 0.2 is one-fifth
            of the screen size.
        @param topAndBottom The size of the top and bottom borders as a factor of the screen dimensions.
        */
        void setBorderSize(Real sides, Real topAndBottom);

        /** Sets the size of the border separately for all borders.

            This method sets a size all borders separately.
        @param left The size of the left border as a factor of the screen dimensions ie 0.2 is one-fifth
            of the screen size.
        @param right The size of the left border as a factor of the screen dimensions.
        @param top The size of the top border as a factor of the screen dimensions.
        @param bottom The size of the bottom border as a factor of the screen dimensions.
        */
        void setBorderSize(Real left, Real right, Real top, Real bottom);

        /** Gets the size of the left border. */
        Real getLeftBorderSize(void) const;
        /** Gets the size of the right border. */
        Real getRightBorderSize(void) const;
        /** Gets the size of the top border. */
        Real getTopBorderSize(void) const;
        /** Gets the size of the bottom border. */
        Real getBottomBorderSize(void) const;
        /// @}

        /** @name Border UV Coordinates
            The border panel uses 8 panels for the border (9 including the center). 
            Imagine a table with 3 rows and 3 columns. The corners are always the same size,
            but the edges stretch depending on how big the panel is. Those who have done
            resizable HTML tables will be familiar with this approach.

            We only require 2 sets of uv coordinates, one for the top-left and one for the
            bottom-right of the panel, since it is assumed the sections are aligned on the texture.
         */
        /// @{
        /** Sets the texture coordinates for the left edge of the border.
        */
        void setLeftBorderUV(Real u1, Real v1, Real u2, Real v2);
        /** Sets the texture coordinates for the right edge of the border.
        */
        void setRightBorderUV(Real u1, Real v1, Real u2, Real v2);
        /** Sets the texture coordinates for the top edge of the border.
        */
        void setTopBorderUV(Real u1, Real v1, Real u2, Real v2);
        /** Sets the texture coordinates for the bottom edge of the border.
        */
        void setBottomBorderUV(Real u1, Real v1, Real u2, Real v2);
        /** Sets the texture coordinates for the top-left corner of the border.
        */
        void setTopLeftBorderUV(Real u1, Real v1, Real u2, Real v2);
        /** Sets the texture coordinates for the top-right corner of the border.
        */
        void setTopRightBorderUV(Real u1, Real v1, Real u2, Real v2);
        /** Sets the texture coordinates for the bottom-left corner of the border.
        */
        void setBottomLeftBorderUV(Real u1, Real v1, Real u2, Real v2);
        /** Sets the texture coordinates for the bottom-right corner of the border.
        */
        void setBottomRightBorderUV(Real u1, Real v1, Real u2, Real v2);

        String getLeftBorderUVString() const;
        String getRightBorderUVString() const;
        String getTopBorderUVString() const;
        String getBottomBorderUVString() const;
        String getTopLeftBorderUVString() const;
        String getTopRightBorderUVString() const;
        String getBottomLeftBorderUVString() const;
        String getBottomRightBorderUVString() const;
        /// @}

        /** Sets the name of the material to use for the borders. */
        void setBorderMaterialName(const String& name, const String& group = DEFAULT_RESOURCE_GROUP );
        /** Gets the name of the material to use for the borders. */
        const String& getBorderMaterialName(void) const;

        /** @copydoc OverlayContainer::_updateRenderQueue */
        void _updateRenderQueue(RenderQueue* queue) override;
        /// @copydoc OverlayElement::visitRenderables
        void visitRenderables(Renderable::Visitor* visitor, 
            bool debugRenderables = false);

        /** @copydoc OverlayElement::setMetricsMode */
        void setMetricsMode(GuiMetricsMode gmm) override;

        /** @copydoc OverlayElement::_update */
        void _update(void) override;
    private:
        float mLeftBorderSize;
        float mRightBorderSize;
        float mTopBorderSize;
        float mBottomBorderSize;
        struct CellUV {
            float u1, v1, u2, v2;
        };
        CellUV mBorderUV[8];

        ushort mPixelLeftBorderSize;
        ushort mPixelRightBorderSize;
        ushort mPixelTopBorderSize;
        ushort mPixelBottomBorderSize;

        MaterialPtr mBorderMaterial;

        /// Render operation for the border area
        RenderOperation mRenderOp2;

        static String msTypeName;

        /// Internal method for setting up geometry, called by OverlayElement::update
        void updatePositionGeometry(void) override;
        /// Internal method for setting up geometry, called by OverlayElement::update
        void updateTextureGeometry(void) override;
        /// Internal method for setting up parameters
        void addBaseParameters(void) override;

        enum BorderCellIndex {
            BCELL_TOP_LEFT = 0,
            BCELL_TOP = 1,
            BCELL_TOP_RIGHT = 2,
            BCELL_LEFT = 3,
            BCELL_RIGHT = 4,
            BCELL_BOTTOM_LEFT = 5,
            BCELL_BOTTOM = 6,
            BCELL_BOTTOM_RIGHT = 7
        };
        String getCellUVString(BorderCellIndex idx) const;

        BorderRenderable* mBorderRenderable;
    };

    /** Class for rendering the border of a BorderPanelOverlayElement.

        We need this because we have to render twice, once with the inner panel's repeating
        material (handled by superclass) and once for the border's separate material. 
    */
    class _OgreOverlayExport BorderRenderable : public Renderable, public OverlayAlloc
    {
    private:
        BorderPanelOverlayElement* mParent;
    public:
        /** Constructed with pointers to parent. */
        BorderRenderable(BorderPanelOverlayElement* parent) : mParent(parent)
        {
            mUseIdentityProjection = true;
            mUseIdentityView = true;
            mPolygonModeOverrideable = parent->getPolygonModeOverrideable();
        }
        const MaterialPtr& getMaterial(void) const override { return mParent->mBorderMaterial; }
        void getRenderOperation(RenderOperation& op) override { op = mParent->mRenderOp2; }
        void getWorldTransforms(Matrix4* xform) const override { mParent->getWorldTransforms(xform); }
        unsigned short getNumWorldTransforms(void) const override { return 1; }
        Real getSquaredViewDepth(const Camera* cam) const override { return mParent->getSquaredViewDepth(cam); }
        const LightList& getLights(void) const override
        {
            // N/A, panels are not lit
            static LightList ll;
            return ll;
        }
    };
    /** @} */
    /** @} */

} // namespace Ogre

#endif // __BorderPanelOverlayElement_H__
