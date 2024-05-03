#pragma once
#include "defs.h"

namespace Ogre {
class Billboard;
}

namespace Tapioca {
class Vector3;
class Vector4;
class BillboardSet;

/*
* @brief Wrapper de la clase Billboard de Ogre.
*/
class TAPIOCA_API Billboard {
private:
    friend BillboardSet;

protected:
    //Billboard de Ogre
    Ogre::Billboard* mBillboard;

    /*
    * @brief Constructor de la clase Billboard.
    * Crea un unico Billboard si se le proporciona un puntero a Ogre::Billboard (a partir de un BillboardSet)
    * @param oBillboard puntero a un billboard de ogre
    */
    Billboard(Ogre::Billboard* const oBillboard);

    /*
    * @brief Devuelve el billboard al que esta asociado
    * @return puntero al billboard de ogre al que esta asociado
    */
    Ogre::Billboard* getBillboard();

public:
    /*
    * @brief Devuelve la posicion del billboard
    * @param Posicion del billboard
    */
    Vector3 getPosition() const;
    /*
    * @brief Devuelve el color del billboard
    * @param Color del billboard
    */
    Vector4 getColour() const;
    /*
    * @brief Devuelve la altura del billboard
    * @param Altura del billboard
    */
    float getHeight() const;
    /*
    * @brief Devuelve la anchura del billboard
    * @param Anchura del billboard
    */
    float getWidth() const;
    //Define las dimensiones del Billboard
    void setDimensions(float width, float height);
    //Define la rotación del Billboard en radianes
    void setRotation(const float rotation);
    //Devuelve la rotación del Billboard en radianes
    float getRotation() const;
};
}
