#pragma once
#include "RenderObject.h"

namespace Ogre {
class SceneManager;
class Billboard;
}

namespace Tapioca {
class Node;
class GraphicsEngine;
class Vector3;
class Vector4;
class BillboardSet;

class TAPIOCA_API Billboard : public RenderObject {
private:
    friend GraphicsEngine;
    friend BillboardSet;

protected:
    //Billboard de Ogre
    Ogre::Billboard* mBillboard;
    //Crea un único Billboard
    Billboard(Ogre::SceneManager* scnMgr, Node* node, std::string name, const Vector3& position, const Vector4& colour);
    
    //Crea un único Billboard si se le proporciona un puntero a Ogre::Billboard (a partir de un BillboardSet)
    Billboard(Ogre::SceneManager* scnMgr, Node* node, Ogre::Billboard* oBillboard);

    Ogre::Billboard* getBillboard();

public:
    //Destructora de la clase
    virtual ~Billboard();
    //Devuelve la posición del Billboard
    Tapioca::Vector3 getPosition() const;
    //Devuelve el color del Billboard
    Tapioca::Vector4 getColour() const;
    //Devuelve la altura del Billboard
    float getHeight() const;
    //Devuelve la anchura del Billboard
    float getWidth() const;
};
}
