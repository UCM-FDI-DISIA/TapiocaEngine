#include "BillboardSet.h"
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "RenderNode.h"
#include "Billboard.h"
#include "checkML.h"

namespace Tapioca {
BillboardSet::BillboardSet(Ogre::SceneManager* const scnMngr, RenderNode* const node, std::string const& name,
    const unsigned int poolSize) : RenderObject(node, scnMngr), mBillboardSet(scnMngr->createBillboardSet(name, poolSize)), 
    mName(name) 
{
    init(mBillboardSet);
}

BillboardSet::~BillboardSet() {
    for (auto& billboard : billboards) delete billboard;
    billboards.clear();
}

void BillboardSet::clear() {
    billboards.clear();
    mBillboardSet->clear();
}

void BillboardSet::removeBillboard(const int index) {
    if (index < billboards.size()) {
        // Elimina el Billboard del UnorderedMap
        Billboard* billboard = billboards[index];
        billboards.erase(billboards.begin() + index);
        // Elimina el Ogre::Billboard del Ogre::BillboardSet
        mBillboardSet->removeBillboard(billboard->getBillboard());
        delete billboard;
    }
}

void BillboardSet::removeBillboard(Billboard* const bb) {
    auto it = std::find(billboards.begin(), billboards.end(), bb);
    if (it != billboards.end()) {
        // Elimina el Billboard del UnorderedMap
        billboards.erase(it);
        // Elimina el Ogre::Billboard del Ogre::BillboardSet
        mBillboardSet->removeBillboard(bb->getBillboard());
    }
}

Billboard* BillboardSet::addBillboard(const Vector3& position, const Vector4& colour) {
    // Crea el Ogre::Billboard
    Ogre::Billboard* oBillboard = mBillboardSet->createBillboard(Ogre::Vector3(position.x, position.y, position.z), 
                                                                 Ogre::ColourValue(colour.x, colour.y, colour.z, colour.w));
    // Crea un Billboard a partir del Ogre::Billboard creado previamente
    Billboard* mBillboard = new Billboard(oBillboard);
    // Inserta el Billboard en el UnorderedMap
    billboards.push_back(mBillboard);

    return mBillboard;
}

Billboard* BillboardSet::getBillboard(const int index) const {
    if (index < billboards.size()) return billboards[index];
    return nullptr;
}

const std::string BillboardSet::getName() const { return mBillboardSet->getName(); }

void BillboardSet::setPoolSize(const size_t size) { mBillboardSet->setPoolSize(size); }

int BillboardSet::getPoolSize() const { return mBillboardSet->getPoolSize(); }

int BillboardSet::getNumBillboards() const { return mBillboardSet->getNumBillboards(); }

void BillboardSet::setBillboardType(BillboardType billboardType) {
    switch (billboardType) {
    case BBT_POINT: mBillboardSet->setBillboardType(Ogre::BBT_POINT); break;
    case BBT_ORIENTED_COMMON: mBillboardSet->setBillboardType(Ogre::BBT_ORIENTED_COMMON); break;
    case BBT_ORIENTED_SELF: mBillboardSet->setBillboardType(Ogre::BBT_ORIENTED_SELF); break;
    case BBT_PERPENDICULAR_COMMON: mBillboardSet->setBillboardType(Ogre::BBT_PERPENDICULAR_COMMON); break;
    case BBT_PERPENDICULAR_SELF: mBillboardSet->setBillboardType(Ogre::BBT_PERPENDICULAR_SELF); break;
    default: break;
    }
}

BillboardType BillboardSet::getBillboardType() const {
    switch (mBillboardSet->getBillboardType()) {
    case Ogre::BillboardType::BBT_POINT: return BBT_POINT; break;
    case Ogre::BillboardType::BBT_ORIENTED_COMMON: return BBT_ORIENTED_COMMON; break;
    case Ogre::BillboardType::BBT_ORIENTED_SELF: return BBT_ORIENTED_SELF; break;
    case Ogre::BillboardType::BBT_PERPENDICULAR_COMMON: return BBT_PERPENDICULAR_COMMON; break;
    case Ogre::BillboardType::BBT_PERPENDICULAR_SELF: return BBT_PERPENDICULAR_SELF; break;
    default: return BBT_POINT; break;
    }
}

void BillboardSet::setMaterialName(const std::string& name) { mBillboardSet->setMaterialName(name); }

const std::string& BillboardSet::getMaterialName(void) const { return mBillboardSet->getMaterialName(); }
};
