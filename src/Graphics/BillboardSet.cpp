#include "BillboardSet.h"
#include <Ogre.h>
#include "Node.h"
#include "Billboard.h"

namespace Tapioca {

BillboardSet::BillboardSet(Ogre::SceneManager* const scnMgr, Node* const node, std::string const& name,
                           const unsigned int poolSize)
    : RenderObject(node, scnMgr), mBillboardSet(scnMgr->createBillboardSet(name, poolSize)), mName(name) {
    init(mBillboardSet);
}

Tapioca::BillboardSet::~BillboardSet() {
    for (auto& billboard : billboards) {
        delete billboard;
    }
    // ya lo hace la destructora padre
    //if (mBillboardSet != nullptr) delete mBillboardSet;   // ??
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
}

const std::string BillboardSet::getName() const { return mBillboardSet->getName(); }

void BillboardSet::setPoolSize(const size_t size) { mBillboardSet->setPoolSize(size); }

int BillboardSet::getPoolSize() const { return mBillboardSet->getPoolSize(); }

int BillboardSet::getNumBillboards() const { return mBillboardSet->getNumBillboards(); }
};