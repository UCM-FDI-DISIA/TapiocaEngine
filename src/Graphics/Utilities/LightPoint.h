#pragma once
#include "Light.h"
#include "defs.h"

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
class RenderNode;
class GraphicsManager;
class Vector4;

/*
* @brief Wrapper de la clase LightPoint de Ogre.
*/
class TAPIOCA_API LightPoint : public Light {
private:
    friend GraphicsManager;

    // PUNTO DE LUZ QUE EMITE LA MISMA CANTIDAD DE LUZ EN TODAS LAS DIRECCIONES

    /*
    * @brief Constructora de la clase LightPoint.
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param colour Color de la luz
    */
    LightPoint(Ogre::SceneManager* const sceneManager, RenderNode* const node, const Vector4 color);

    // Attenuation = Constant + Linear * Distance + Quadratic * Distance^2
    // Por ejemplo, si constant es 0,5 quiere decir que es la mitad de brillante en todas las distancias
    /*
    * @brief Cambia la atenuacion de la luz, es decir, como disminuye con la distancia
    * @param maximumRange Distancia maxima a la que luz tiene efecto (en unidades de mundo)
    * @param constant Nivel base de la atenuacion de la luz (no varia con la distancia) (0-1)
    * @param linear Factor lineal en la atenuacion (0-1)
    * @param quadratic Factor cuadratico en la atenuacion (0-1)
    */
    void setAttenuation(float maximumRange, float constant, float linear, float quadratic);
};
}