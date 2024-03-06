#pragma once
#include "Utilities/defs.h"

namespace Ogre {
class Billboard;
}

namespace Tapioca {
class Vector3;
class Vector4;
class BillboardSet;

class TAPIOCA_API Billboard {
private:
    friend BillboardSet;

protected:
    //Billboard de Ogre
    Ogre::Billboard* mBillboard;
    //Crea un �nico Billboard
    //Billboard(Ogre::SceneManager* scnMgr, Node* node, std::string name, const Vector3& position, const Vector4& colour);

    //Crea un �nico Billboard si se le proporciona un puntero a Ogre::Billboard (a partir de un BillboardSet)
    Billboard(Ogre::Billboard* oBillboard);

    Ogre::Billboard* getBillboard();

public:
    //Destructora de la clase
    virtual ~Billboard();
    //Devuelve la posici�n del Billboard
    Vector3 getPosition() const;
    //Devuelve el color del Billboard
    Vector4 getColour() const;
    //Devuelve la altura del Billboard
    float getHeight() const;
    //Devuelve la anchura del Billboard
    float getWidth() const;
};
}
