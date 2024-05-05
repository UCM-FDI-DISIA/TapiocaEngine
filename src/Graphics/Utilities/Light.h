#pragma once
#include "RenderObject.h"
#include "Utilities/Vector4.h"

namespace Ogre {
class SceneManager;
class Light;
}

namespace Tapioca {
class RenderNode;
/**
* @brief Wrapper de la clase Light de Ogre
*/
class TAPIOCA_API Light : public RenderObject {
protected:
    Ogre::Light* mLight;   // Light de Ogre
    Vector4 mColor;        // Color de la luz

    /**
    * @brief Constructor de la clase Light
    * @param scnMngr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param color Color de la luz
    */
    Light(Ogre::SceneManager* const scnMngr, RenderNode* const node, const Vector4 color);

public:
    /**
    * @brief Destructor vacio
    */
    virtual ~Light() { }

    /**
    * @brief Establece el color de la luz
    * @param color Color de la luz
    */
    void setColor(const Vector4 color);

    /**
    * @brief Cambia la potencia de la luz. Por defecto es 1.0f
    * @param power Potencia
    */
    void setPowerScale(const float power);
};
}
