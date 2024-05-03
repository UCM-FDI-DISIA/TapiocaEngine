#pragma once
#include "defs.h"

namespace Ogre {
class Billboard;
}

namespace Tapioca {
class Vector3;
class Vector4;
class BillboardSet;

/**
* @brief Wrapper de la clase Billboard de Ogre
*/
class TAPIOCA_API Billboard {
private:
    friend BillboardSet;

protected:
    Ogre::Billboard* mBillboard; // Billboard de Ogre

    /**
    * @brief Constructor de la clase Billboard.
    * Crea un unico Billboard si se le proporciona un puntero a Ogre::Billboard (a partir de un BillboardSet)
    * @param oBillboard puntero a un billboard de ogre
    */
    Billboard(Ogre::Billboard* const oBillboard);

    /**
    * @brief Devuelve el billboard al que esta asociado
    * @return puntero al billboard de ogre al que esta asociado
    */
    inline Ogre::Billboard* getBillboard() const { return mBillboard; }

public:
    /**
    * @brief Devuelve la posicion del billboard
    * @return Posicion del billboard
    */
    Vector3 getPosition() const;
    /**
    * @brief Devuelve el color del billboard
    * @return Color del billboard
    */
    Vector4 getColour() const;
    /**
    * @brief Devuelve la altura del billboard
    * @return Altura del billboard
    */
    float getHeight() const;
    /**
    * @brief Devuelve la anchura del billboard
    * @return Anchura del billboard
    */
    float getWidth() const;
    /**
    * @brief Establece las dimensiones del billboard
    * @param Anchura del billboard
    * @param Altura del billboard
    */
    void setDimensions(float width, float height);
    /**
    * @brief Establece la rotacion del Billboard en radianes
    * @param rotation Rotacion del billboard en radianes
    */
    void setRotation(const float rotation);
    /**
    * @brief Devuelve la rotacion del billboard en radianes
    * @param Rotacion del billboard en radianes
    */
    float getRotation() const;
};
}
