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

/**
* @brief Wrapper de la clase LightPoint de Ogre
* @brief Punto de luz que emite la misma cantidad de luz en todas las direcciones
*/
class TAPIOCA_API LightPoint : public Light {
private:
    friend GraphicsManager;

    /**
    * @brief Constructor de la clase LightPoint
    * @param scnMngr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param color Color de la luz
    */
    LightPoint(Ogre::SceneManager* const scnMngr, RenderNode* const node, const Vector4 color);

public:
    /**
    * @brief Cambia la atenuacion de la luz, es decir, como disminuye con la distancia
    * @brief Attenuation = Constant + Linear * Distance + Quadratic * Distance^2
    * @brief Por ejemplo, si constant es 0,5 quiere decir que es la mitad de brillante en todas las distancias
    * @param maximumRange Distancia maxima a la que la luz tiene efecto (en unidades de mundo)
    * @param constant Nivel base de la atenuacion de la luz (no varia con la distancia) (0-1)
    * @param linear Factor lineal en la atenuacion (0-1)
    * @param quadratic Factor cuadratico en la atenuacion (0-1)
    */
    void setAttenuation(float maximumRange, float constant, float linear, float quadratic);
};
}
