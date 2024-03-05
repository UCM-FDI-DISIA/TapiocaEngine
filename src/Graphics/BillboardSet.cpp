#include "BillboardSet.h"
#include <Ogre.h>
#include "Node.h"
#include "Billboard.h"
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"

namespace Tapioca {

BillboardSet::BillboardSet(Ogre::SceneManager* scnMgr, Node* node, std::string const& name, unsigned int poolSize)
    : RenderObject(node, scnMgr), mBillboardSet(scnMgr->createBillboardSet(name, poolSize)), mName(name) 
{
    init(mBillboardSet);
}

void BillboardSet::setVisible(bool v) { mBillboardSet->setVisible(v); }

bool BillboardSet::getVisible() const { return mBillboardSet->getVisible(); }

void BillboardSet::clear() { mBillboardSet->clear(); }

const std::string BillboardSet::getName() const { return mBillboardSet->getName(); }

void BillboardSet::setPoolSize(size_t size) { mBillboardSet->setPoolSize(size); }

int BillboardSet::getPoolSize() const { return mBillboardSet->getPoolSize(); }

int BillboardSet::getNumBillboards() const { return mBillboardSet->getNumBillboards(); }

void BillboardSet::removeBillboard(unsigned int index) { mBillboardSet->removeBillboard(index); }

Billboard* BillboardSet::createBillboard(const Vector3& position, const Vector4& colour = (255.0f, 255.0f, 255.0f, 255.0f))
{
    Ogre::Billboard* bb = mBillboardSet->createBillboard(Ogre::Vector3(position.x, position.y, position.z),
                                                          Ogre::ColourValue(colour.x, colour.y, colour.z, colour.w));
    Billboard* mBillboard = new Billboard(sceneManager, node, bb);

    mBillboardUnorderedSet.insert(mBillboard);

    return mBillboard;
}

}