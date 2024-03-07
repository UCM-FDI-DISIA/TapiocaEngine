#include "Billboard.h"
#include <Ogre.h>
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"

namespace Tapioca {

Billboard::Billboard(Ogre::Billboard* const oBillboard) : mBillboard(oBillboard) { }

Ogre::Billboard* Tapioca::Billboard::getBillboard() { return mBillboard; }

Tapioca::Billboard::~Billboard() {
    // NO HACE FALTA, PRODUCE ERROR (CON SACARLO ES SUFICENTE)
    //if (mBillboard != nullptr) delete mBillboard;
}

Tapioca::Vector3 Tapioca::Billboard::getPosition() const {
    return Tapioca::Vector3(mBillboard->getPosition().x, mBillboard->getPosition().y, mBillboard->getPosition().z);
}

Tapioca::Vector4 Tapioca::Billboard::getColour() const {
    return Tapioca::Vector4(mBillboard->getColour().r, mBillboard->getColour().g, mBillboard->getColour().b,
                            mBillboard->getColour().a);
}

float Tapioca::Billboard::getHeight() const { return mBillboard->getOwnHeight(); }

float Tapioca::Billboard::getWidth() const { return mBillboard->getOwnWidth(); }

void Tapioca::Billboard::setColour(Vector4 colour) {
    mBillboard->setColour(Ogre::ColourValue(colour.x, colour.y, colour.z, colour.w));
}

void Tapioca::Billboard::setDimensions(float width, float height) {
    mBillboard->setDimensions(Ogre::Real(width), Ogre::Real(height));
}

void Tapioca::Billboard::setPosition(const Vector3& position) {
    mBillboard->setPosition(Ogre::Vector3(position.x, position.y, position.z));
}

void Tapioca::Billboard::setRotation(const float rotation) { mBillboard->setRotation(Ogre::Radian(rotation)); }

float Tapioca::Billboard::getRotation() const { return mBillboard->getRotation().valueRadians(); }
};