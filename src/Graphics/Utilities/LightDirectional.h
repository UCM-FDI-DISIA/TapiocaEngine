#pragma once
#include "Light.h"

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
class RenderNode;
class GraphicsManager;
class Vector3;
class Vector4;

/*
* @brief Wrapper de la clase LightDirectional de Ogre
*/
/*
* @brief Simula rayos de luz desde una posicion lejana (como el sol)
* por lo tanto, solo necesita una direccion, pero no una posicion
* tiene un rango infinito y una intensidad constante
*/
class TAPIOCA_API LightDirectional : public Light {
private:
    friend GraphicsManager;

    /*
    * @brief Constructor de la clase LightDirectional
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param color Color de la luz
    * @param direction Direccion a la que apunta
    */
    LightDirectional(Ogre::SceneManager* const scnManager, RenderNode* const node, const Vector4 color,
                     const Vector3 direction);
    /*
    * @brief Se elimina de la escena
    */
    ~LightDirectional();

public:
    /*
    * @brief Cambia la direccion a la que apunta la luz
    * @param direction Direccion a la que se quiere apuntar
    */
    void setDirection(const Vector3 direction);

    /*
    * @brief Activa o desactiva la produccion de sombras
    * @param enable True para activar, false para desactivar
    */
    void produceShadows(const bool enable);
};
}
