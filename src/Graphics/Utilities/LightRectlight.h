#pragma once
#include "Light.h"

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
class RenderNode;
class Vector4;
class Vector3;
class GraphicsManager;

class TAPIOCA_API LightRectlight : public Light {
private:
    friend GraphicsManager;

    // SIMULA UN AREA RECTANGULAR DE LUZ
    // REQUIERE DE UNA POSICION, UNA DIRECCION Y UN TAM DEL AREA

    LightRectlight(Ogre::SceneManager* const sceneManager, RenderNode* const node, Vector4 const color,
                   const float width, const float height, Vector3 direction);

public:
    void setSize(float width, float height);

    void setDirection(Vector3 direction);

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