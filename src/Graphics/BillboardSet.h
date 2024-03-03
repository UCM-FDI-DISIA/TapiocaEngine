#pragma once
#include "RenderObject.h"

namespace Ogre {
class SceneManager;
class BillboardSet;
}
namespace Tapioca {
class Node;
}

namespace Tapioca {
class BillboardSet : public RenderObject {
protected:
    Ogre::BillboardSet* mBillboardSet;

    BillboardSet(Ogre::SceneManager* scnMgr, Node* node, int poolSize);

public:
    virtual ~BillboardSet() { }

    void setVisible(bool v);
};
}