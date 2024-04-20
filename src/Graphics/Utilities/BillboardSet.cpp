#include "BillboardSet.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "RenderNode.h"
#include "Billboard.h"

namespace Tapioca {

BillboardSet::BillboardSet(Ogre::SceneManager* const scnMgr, RenderNode* const node, std::string const& name,
                           const unsigned int poolSize)
    : RenderObject(node, scnMgr), mBillboardSet(scnMgr->createBillboardSet(name, poolSize)), mName(name) {
    init(mBillboardSet);
}

Tapioca::BillboardSet::~BillboardSet() {
    for (auto& billboard : billboards) {
        delete billboard;
    }
    billboards.clear();
}

void Tapioca::BillboardSet::clear() {
    //Vacía el UnorderedMap de Tapioca::Billboards
    billboards.clear();
    //Vacía el Ogre::BillboardSet
    mBillboardSet->clear();
}

void Tapioca::BillboardSet::removeBillboard(const int index) {
    if (index < billboards.size()) {
        //Elimina el Tapioca::Billboard del UnorderedMap
        Tapioca::Billboard* billboard = billboards[index];
        billboards.erase(billboards.begin() + index);
        //Elimina el Ogre::Billboard del Ogre::BillboardSet
        mBillboardSet->removeBillboard(billboard->getBillboard());
        delete billboard;
    }
}

void Tapioca::BillboardSet::removeBillboard(Billboard* const bb) {
    auto it = std::find(billboards.begin(), billboards.end(), bb);
    if (it != billboards.end()) {
        //Elimina el Tapioca::Billboard del UnorderedMap
        billboards.erase(it);
        //Elimina el Ogre::Billboard del Ogre::BillboardSet
        mBillboardSet->removeBillboard(bb->getBillboard());
    }
}

Tapioca::Billboard* Tapioca::BillboardSet::addBillboard(const Vector3& position, const Vector4& colour) {
    //Crea el Ogre::Billboard
    Ogre::Billboard* oBillboard = mBillboardSet->createBillboard(
        Ogre::Vector3(position.x, position.y, position.z), Ogre::ColourValue(colour.x, colour.y, colour.z, colour.w));
    //Crea un Tapioca::Billboard a partir del Ogre::Billboard creado previamente
    Billboard* mBillboard = new Tapioca::Billboard(oBillboard);
    //Inserta el Billboard en el UnorderedMap
    billboards.push_back(mBillboard);

    return mBillboard;
}

Tapioca::Billboard* Tapioca::BillboardSet::getBillboard(const int index) const {
    if (index < billboards.size()) {
        return billboards[index];
    }

    return nullptr;
}

const std::string Tapioca::BillboardSet::getName() const { return mBillboardSet->getName(); }

void Tapioca::BillboardSet::setPoolSize(const size_t size) { mBillboardSet->setPoolSize(size); }

int Tapioca::BillboardSet::getPoolSize() const { return mBillboardSet->getPoolSize(); }

int Tapioca::BillboardSet::getNumBillboards() const { return mBillboardSet->getNumBillboards(); }

void Tapioca::BillboardSet::setBillboardType(BillboardType billboardType) {
    switch (billboardType) {
    case Tapioca::BBT_POINT: mBillboardSet->setBillboardType(Ogre::BBT_POINT); break;
    case Tapioca::BBT_ORIENTED_COMMON: mBillboardSet->setBillboardType(Ogre::BBT_ORIENTED_COMMON); break;
    case Tapioca::BBT_ORIENTED_SELF: mBillboardSet->setBillboardType(Ogre::BBT_ORIENTED_SELF); break;
    case Tapioca::BBT_PERPENDICULAR_COMMON: mBillboardSet->setBillboardType(Ogre::BBT_PERPENDICULAR_COMMON); break;
    case Tapioca::BBT_PERPENDICULAR_SELF: mBillboardSet->setBillboardType(Ogre::BBT_PERPENDICULAR_SELF); break;
    default: break;
    }
}

BillboardType Tapioca::BillboardSet::getBillboardType() {
    switch (mBillboardSet->getBillboardType()) {
    case Ogre::BillboardType::BBT_POINT: return BBT_POINT; break;
    case Ogre::BillboardType::BBT_ORIENTED_COMMON: return BBT_ORIENTED_COMMON; break;
    case Ogre::BillboardType::BBT_ORIENTED_SELF: return BBT_ORIENTED_SELF; break;
    case Ogre::BillboardType::BBT_PERPENDICULAR_COMMON: return BBT_PERPENDICULAR_COMMON; break;
    case Ogre::BillboardType::BBT_PERPENDICULAR_SELF: return BBT_PERPENDICULAR_SELF; break;
    default: return BBT_POINT; break;
    }
}

void Tapioca::BillboardSet::setMaterialName(const std::string& name) { mBillboardSet->setMaterialName(name); }

const std::string& Tapioca::BillboardSet::getMaterialName(void) const { return mBillboardSet->getMaterialName(); }
};