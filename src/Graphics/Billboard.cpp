#include "Billboard.h"
#include <Ogre.h>
#include "Node.h"
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"

namespace Tapioca {

Tapioca::Billboard::Billboard(Ogre::SceneManager* scnMgr, Node* node, std::string name, const Vector3& position,
                              const Vector4& colour = Vector4(255.0f, 255.0f, 255.0f, 255.0f))
    : RenderObject(node, scnMgr),
      mBillboard(new Ogre::Billboard(Ogre::Vector3(position.x, position.y, position.z), new Ogre::BillboardSet(name),
                                     Ogre::ColourValue(colour.x, colour.y, colour.z, colour.w))) { }

Tapioca::Billboard::Billboard(Ogre::SceneManager* scnMgr, Node* node, Ogre::Billboard* oBillboard)
    : RenderObject(node, scnMgr), mBillboard(oBillboard) { }

Ogre::Billboard* Tapioca::Billboard::getBillboard() { return mBillboard; }

Tapioca::Billboard::~Billboard() {
    if (mBillboard != nullptr) delete mBillboard;
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

};