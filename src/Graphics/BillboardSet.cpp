#include "BillboardSet.h"
#include <Ogre.h>

Tapioca::BillboardSet::BillboardSet(Ogre::SceneManager* scnMgr, Node* node, int poolSize)
    : RenderObject(node, scnMgr), mBillboardSet(scnMgr->createBillboardSet(poolSize)) {
    init(mBillboardSet);
}

void Tapioca::BillboardSet::setVisible(bool v) { mBillboardSet->setVisible(v); }