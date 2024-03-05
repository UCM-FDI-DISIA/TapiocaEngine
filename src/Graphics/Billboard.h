#pragma once
#include "RenderObject.h"
#include <string>

namespace Ogre {
class SceneManager;
class Billboard;
}

namespace Tapioca {
class Node;
class GraphicsEngine;
class BillboardSet;
class Vector3;
class Vector4;

class Billboard : public RenderObject {
private:
    friend GraphicsEngine;
    friend BillboardSet;

protected:
    Ogre::Billboard* mBillboard;
    
    //Crea un único Billboard
    Billboard(Ogre::SceneManager* scnMgr, Node* node, std::string name, const Vector3& position,
              const Vector4& colour);

    //Crea un único Billboard si se le proporciona un puntero a Ogre::Billboard (a partir de un BillboardSet)
    Billboard(Ogre::SceneManager* scnMgr, Node* node, Ogre::Billboard* oBillboard);

public:
    //Destructora de la clase
    virtual ~Billboard() {
        if (mBillboard != nullptr) delete mBillboard;
    }
};
}
