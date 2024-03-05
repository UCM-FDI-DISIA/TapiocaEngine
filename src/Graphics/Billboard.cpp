#include "Billboard.h"
#include "BillboardSet.h"
#include <Ogre.h>
#include "Node.h"
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"

Tapioca::Billboard::Billboard(Ogre::SceneManager* scnMgr, Node* node, std::string name, const Vector3& position,
                              const Vector4& colour = Vector4(255, 255, 255, 255))
    : RenderObject(node, scnMgr),
      mBillboard(new Ogre::Billboard(Ogre::Vector3(position.x, position.y, position.z), new Ogre::BillboardSet(name),
                                     Ogre::ColourValue(colour.x, colour.y, colour.z, colour.w))) { }

Tapioca::Billboard::Billboard(Ogre::SceneManager* scnMgr, Node* node, Ogre::Billboard* oBillboard)
    : RenderObject(node, scnMgr), mBillboard(oBillboard) { }