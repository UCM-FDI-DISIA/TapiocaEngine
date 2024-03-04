#pragma once
#include "RenderObject.h"

namespace Ogre {
class SceneManager;
class Billboard;
}

namespace Tapioca {
class Node;
class GraphicsEngine;

class Billboard : public RenderObject {
private:
    friend GraphicsEngine;

protected:
    Ogre::Billboard* mBillboard;

    Billboard(Ogre::SceneManager* scnMgr, Node* node);

public:
    //Destructora de la clase
    virtual ~Billboard() {
        if (mBillboard != nullptr) delete mBillboard;
    }
};
}
