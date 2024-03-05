#include "BillboardSet.h"
#include <Ogre.h>
#include "Node.h"
#include "Billboard.h"
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"

namespace Tapioca {

BillboardSet::BillboardSet(Ogre::SceneManager* scnMgr, Node* node, std::string const& name, unsigned int poolSize)
    : RenderObject(node, scnMgr), mBillboardSet(scnMgr->createBillboardSet(name, poolSize)), mName(name) {
    init(mBillboardSet);
}

Tapioca::BillboardSet::~BillboardSet() {
    if (mBillboardSet != nullptr) delete mBillboardSet;
    mBillboardUnorderedMap.clear();
}

void Tapioca::BillboardSet::clear() {
    //Vacía el UnorderedMap de Tapioca::Billboards
    mBillboardUnorderedMap.clear();
    //Vacía el Ogre::BillboardSet
    mBillboardSet->clear();
}

void Tapioca::BillboardSet::removeBillboard(unsigned int index) {
    //Elimina el Tapioca::Billboard del UnorderedMap
    mBillboardUnorderedMap.erase(mBillboardSet->getBillboard(index));
    //Elimina el Ogre::Billboard del Ogre::BillboardSet
    mBillboardSet->removeBillboard(index);
}

void Tapioca::BillboardSet::removeBillboard(Billboard* bb) {
    //Elimina el Tapioca::Billboard del UnorderedMap
    mBillboardUnorderedMap.erase(bb->getBillboard());
    //Elimina el Ogre::Billboard del Ogre::BillboardSet
    mBillboardSet->removeBillboard(bb->getBillboard());
}

Tapioca::Billboard* Tapioca::BillboardSet::createBillboard(const Vector3& position,
                                                           const Vector4& colour = (float(255), float(255), float(255),
                                                                                    float(255))) {
    //Crea el Ogre::Billboard
    Ogre::Billboard* oBillboard = mBillboardSet->createBillboard(
        Ogre::Vector3(position.x, position.y, position.z), Ogre::ColourValue(colour.x, colour.y, colour.z, colour.w));
    //Crea un Tapioca::Billboard a partir del Ogre::Billboard creado previamente
    Billboard* mBillboard = new Tapioca::Billboard(sceneManager, node, oBillboard);
    //Inserta el Billboard en el UnorderedMap
    mBillboardUnorderedMap.insert({oBillboard, mBillboard});

    return mBillboard;
}

Tapioca::Billboard* Tapioca::BillboardSet::getBillboard(unsigned int index) const {
    return (*mBillboardUnorderedMap.find(mBillboardSet->getBillboard(index))).second;
}

const std::string BillboardSet::getName() const { return mBillboardSet->getName(); }

void BillboardSet::setPoolSize(size_t size) { mBillboardSet->setPoolSize(size); }

int BillboardSet::getPoolSize() const { return mBillboardSet->getPoolSize(); }

int BillboardSet::getNumBillboards() const { return mBillboardSet->getNumBillboards(); }

void Tapioca::BillboardSet::setVisible(bool v) { mBillboardSet->setVisible(v); }

bool Tapioca::BillboardSet::getVisible() const { return mBillboardSet->getVisible(); }

};