#include "BillboardSet.h"
#include <Ogre.h>
#include "Node.h"
#include "Billboard.h"
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"

Tapioca::BillboardSet::BillboardSet(Ogre::SceneManager* scnMgr, Node* node)
    : RenderObject(node, scnMgr), mBillboardSet(scnMgr->createBillboardSet()) {
    init(mBillboardSet);
}

Tapioca::BillboardSet::BillboardSet(Ogre::SceneManager* scnMgr, Node* node, unsigned int poolSize)
    : RenderObject(node, scnMgr), mBillboardSet(scnMgr->createBillboardSet(poolSize)) {
    init(mBillboardSet);
}

Tapioca::BillboardSet::BillboardSet(Ogre::SceneManager* scnMgr, Node* node, std::string const& name,
    unsigned int poolSize) : RenderObject(node, scnMgr), mBillboardSet(scnMgr->createBillboardSet(name, poolSize)) 
{
    init(mBillboardSet);
}

Tapioca::BillboardSet::BillboardSet(Ogre::SceneManager* scnMgr, Node* node, std::string const& name)
    : RenderObject(node, scnMgr), mBillboardSet(scnMgr->createBillboardSet(name)) 
{
    init(mBillboardSet);
}

void Tapioca::BillboardSet::setVisible(bool v) { mBillboardSet->setVisible(v); }

bool Tapioca::BillboardSet::getVisible() const { return mBillboardSet->getVisible(); }

void Tapioca::BillboardSet::clear() { mBillboardSet->clear(); }

const std::string Tapioca::BillboardSet::getName() const { return mBillboardSet->getName(); }

void Tapioca::BillboardSet::setPoolSize(size_t size) { mBillboardSet->setPoolSize(size); }

int Tapioca::BillboardSet::getPoolSize() const { return mBillboardSet->getPoolSize(); }

int Tapioca::BillboardSet::getNumBillboards() const { return mBillboardSet->getNumBillboards(); }

void Tapioca::BillboardSet::removeBillboard(unsigned int index) { mBillboardSet->removeBillboard(index); }

void Tapioca::BillboardSet::createBillboard(const Vector3& position, const Vector4& colour = (255, 255, 255, 255)) {
    mBillboardSet->createBillboard(Ogre::Vector3(position.x, position.y, position.z),
                                   Ogre::ColourValue(colour.x, colour.y, colour.z, colour.w));
}