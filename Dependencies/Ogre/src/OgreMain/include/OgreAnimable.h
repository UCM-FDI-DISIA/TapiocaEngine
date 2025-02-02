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
#ifndef __ANIMABLE_H__
#define __ANIMABLE_H__

#include "OgrePrerequisites.h"
#include "OgreCommon.h"
#include "OgreVector.h"
#include "OgreColourValue.h"
#include "OgreStringVector.h"
#include "OgreException.h"
#include "OgreHeaderPrefix.h"

namespace Ogre {

    class Any;

    /** \addtogroup Core
    *  @{
    */

    /** \addtogroup Animation
    *  @{
    */

    /** Defines an object property which is animable, i.e. may be keyframed.

        Animable properties are those which can be altered over time by a 
        predefined keyframe sequence. They may be set directly, or they may
        be modified from their existing state (common if multiple animations
        are expected to apply at once). Implementors of this interface are
        expected to override the 'setValue', 'setCurrentStateAsBaseValue' and 
        'applyDeltaValue' methods appropriate to the type in question, and to 
        initialise the type.
    @par
        AnimableValue instances are accessible through any class which extends
        AnimableObject in order to expose it's animable properties.
    @note
        This class is an instance of the Adapter pattern, since it generalises
        access to a particular property. Whilst it could have been templated
        such that the type which was being referenced was compiled in, this would
        make it more difficult to aggregated generically, and since animations
        are often comprised of multiple properties it helps to be able to deal
        with all values through a single class.
    */
    class _OgreExport AnimableValue : public AnimableAlloc
    {
    public:
        /// The type of the value being animated
        enum ValueType
        {
            INT,
            REAL,
            VECTOR2,
            VECTOR3,
            VECTOR4,
            QUATERNION,
            COLOUR,
            RADIAN
        };
    protected:
        /// Value type
        ValueType mType;

        /// Base value data
        union
        {
#pragma warning(push)
#pragma warning(disable : 26495)
            int mBaseValueInt;
            Real mBaseValueReal[4];
#pragma warning(pop)
        };

        /// Internal method to set a value as base
        virtual void setAsBaseValue(int val) { mBaseValueInt = val; }
        /// Internal method to set a value as base
        virtual void setAsBaseValue(Real val) { mBaseValueReal[0] = val; }
        /// Internal method to set a value as base
        virtual void setAsBaseValue(const Vector2& val) 
        { memcpy(mBaseValueReal, val.ptr(), sizeof(Real)*2); }
        /// Internal method to set a value as base
        virtual void setAsBaseValue(const Vector3& val) 
        { memcpy(mBaseValueReal, val.ptr(), sizeof(Real)*3); }
        /// Internal method to set a value as base
        virtual void setAsBaseValue(const Vector4& val) 
        { memcpy(mBaseValueReal, val.ptr(), sizeof(Real)*4); }
        /// Internal method to set a value as base
        virtual void setAsBaseValue(const Quaternion& val)
        { memcpy(mBaseValueReal, val.ptr(), sizeof(Real)*4); }
        /// Internal method to set a value as base
        virtual void setAsBaseValue(const Any& val);
        /// Internal method to set a value as base
        virtual void setAsBaseValue(const ColourValue& val)
        { 
            mBaseValueReal[0] = val.r;
            mBaseValueReal[1] = val.g;
            mBaseValueReal[2] = val.b;
            mBaseValueReal[3] = val.a;
        }
        /// Internal method to set a value as base
        virtual void setAsBaseValue(const Radian& val)
        { 
            mBaseValueReal[0] = val.valueRadians();
        }
        /// Internal method to set a value as base
        virtual void setAsBaseValue(const Degree& val)
        { 
            mBaseValueReal[0] = val.valueRadians();
        }


    public:
#pragma warning(push)
#pragma warning(disable : 26495)
        AnimableValue(ValueType t) : mType(t) {}
#pragma warning(pop)
        virtual ~AnimableValue() {}

        /// Gets the value type of this animable value
        ValueType getType(void) const { return mType; }

        /// Sets the current state as the 'base' value; used for delta animation
        virtual void setCurrentStateAsBaseValue(void) = 0;

        /// Set value 
        virtual void setValue(int) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Set value 
        virtual void setValue(Real) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Set value 
        virtual void setValue(const Vector2&) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Set value 
        virtual void setValue(const Vector3&) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Set value 
        virtual void setValue(const Vector4&) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Set value 
        virtual void setValue(const Quaternion&) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Set value 
        virtual void setValue(const ColourValue&) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Set value 
        virtual void setValue(const Radian&) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Set value
        void setValue(const Degree& val) { setValue(Radian(val)); }
        /// Set value 
        virtual void setValue(const Any& val);

        // reset to base value
        virtual void resetToBaseValue(void);

        /// Apply delta value
        virtual void applyDeltaValue(int) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Set value 
        virtual void applyDeltaValue(Real) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Apply delta value 
        virtual void applyDeltaValue(const Vector2&) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Apply delta value 
        virtual void applyDeltaValue(const Vector3&) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Apply delta value 
        virtual void applyDeltaValue(const Vector4&) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Apply delta value 
        virtual void applyDeltaValue(const Quaternion&) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Apply delta value 
        virtual void applyDeltaValue(const ColourValue&) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Apply delta value
        void applyDeltaValue(const Degree& val) { applyDeltaValue(Radian(val)); }
        /// Apply delta value 
        virtual void applyDeltaValue(const Radian&) {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "", "");
        }
        /// Apply delta value 
        virtual void applyDeltaValue(const Any& val);


    };

    /** Defines an interface to classes which have one or more AnimableValue
        instances to expose.
    */
    class _OgreExport AnimableObject
    {
    public:
        AnimableObject() {}
        virtual ~AnimableObject() {}

        /** Gets a list of animable value names for this object. */
        virtual const StringVector& getAnimableValueNames(void) const
        {
            OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "Animable value list not implemented");
        }

        /** Create a reference-counted AnimableValuePtr for the named value.

            You can use the returned object to animate a value on this object,
            using AnimationTrack. Subclasses must override this if they wish 
            to support animation of their values.
        */
        virtual AnimableValuePtr createAnimableValue(const String& valueName)
        {
            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "No animable value named '" + valueName + "' present");
        }



    };

    /** @} */
    /** @} */

}

#include "OgreHeaderSuffix.h"

#endif

