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

    //Crea un �nico Billboard si se le proporciona un puntero a Ogre::Billboard (a partir de un BillboardSet)
    Billboard(Ogre::Billboard* const oBillboard);

    Ogre::Billboard* getBillboard();

public:
    //Destructora de la clase
    virtual ~Billboard();
    //Devuelve la posici�n del Billboard
    Vector3 getPosition() const;
    //Define la posición del Billboard
    void setPosition(const Vector3& position);
    //Devuelve el color del Billboard
    Vector4 getColour() const;
    //Define el color del Billboard
    void setColour(Vector4 colour);
    //Devuelve la altura del Billboard
    float getHeight() const;
    //Devuelve la anchura del Billboard
    float getWidth() const;
    //Define las dimensiones del Billboard
    void setDimensions(float width, float height);
    //Define la rotación del Billboard en radianes
    void setRotation(const float rotation);
    //Devuelve la rotación del Billboard en radianes
    float getRotation() const;
};
}
