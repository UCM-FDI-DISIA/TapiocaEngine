#pragma once
#include "RenderObject.h"
#include "Utilities/Vector4.h"

namespace Ogre {
class SceneManager;
class Light;
}

namespace Tapioca {
class RenderNode;
/*
* @brief Wrapper de la clase Light de Ogre.
*/
class TAPIOCA_API Light : public RenderObject {
protected:
    Ogre::Light* mLight;        // Light de Ogre
    Vector4 mColor;            // Color de la luz

    /*
    * @brief Constructora de la clase Light.
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param colour Color de la luz
    */
    Light(Ogre::SceneManager* const scnMgr, RenderNode* const node, const Vector4 color);

public:
    /*
    * @brief Destructora de la clase Light.
    */
    virtual ~Light() { }

    /*
    * @brief Cambia el color.
    * @param colour Color de la luz
    */
    void setColor(const Vector4 color);

    /*
    * @brief Cambia la potencia de la luz. Por defecto es 1.0f
    * @param power Potencia
    */
    void setPowerScale(float power);
};
}