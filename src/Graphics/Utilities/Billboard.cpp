#include "Billboard.h"
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"
#include "checkML.h"

namespace Tapioca {
Billboard::Billboard(Ogre::Billboard* const oBillboard) : mBillboard(oBillboard) { }

Vector3 Billboard::getPosition() const {
    return Vector3(mBillboard->getPosition().x, mBillboard->getPosition().y, mBillboard->getPosition().z);
}

Vector4 Billboard::getColour() const {
    return Vector4(mBillboard->getColour().r, mBillboard->getColour().g, mBillboard->getColour().b,
                            mBillboard->getColour().a);
}

float Billboard::getHeight() const { return mBillboard->getOwnHeight(); }

float Billboard::getWidth() const { return mBillboard->getOwnWidth(); }

void Billboard::setDimensions(float width, float height) {
    mBillboard->setDimensions(Ogre::Real(width), Ogre::Real(height));
}

void Billboard::setRotation(const float rotation) { mBillboard->setRotation(Ogre::Radian(rotation)); }

float Billboard::getRotation() const { return mBillboard->getRotation().valueRadians(); }
};
