#pragma once
#include "Light.h"

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
class RenderNode;
class GraphicsEngine;
class Vector4;
class Vector3;

/*
* @brief Wrapper de la clase LightDirectional de Ogre.
*/
class TAPIOCA_API LightDirectional : public Light {
private:
    friend GraphicsEngine;

    // SOLO NECESITA UNA DIRECCION, PERO NO UNA POSICION (LA POSICION NO AFECTA PARA NADA)
    // SIMULA RAYOS DE LUZ DESDE UNA POSICION LEJANA (COMO EL SOL)
    /*
    * @brief Constructora de la clase LightDirectional.
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param colour Color de la luz
    * @param direction Direccion a la que apunta
    */
    LightDirectional(Ogre::SceneManager* const scnManager, RenderNode* const node, const Vector4 color,
                     const Vector3 direction);

public:
    /*
    * @brief Cambia la direccion a la que apunta la luz
    * @param direction Direccion a la que se quiere apuntar
    */
    void setDirection(const Vector3 direction);
};
}