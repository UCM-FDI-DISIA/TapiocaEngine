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

/**
* @brief Wrapper de la clase LightRectLight de Ogre
* @brief Simula un area rectangular de luz
* requiere de una posicion, una direccion y un tam del area
*/
class TAPIOCA_API LightRectlight : public Light {
private:
    friend GraphicsManager;

    /**
    * @brief Constructor de la clase LightRectlight
    * @param scnMngr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param color Color de la luz
    */
    LightRectlight(Ogre::SceneManager* const scnMngr, RenderNode* const node, Vector4 const color,
                   const float width, const float height, Vector3 direction);

public:
    /**
    * @brief Establece el tamano del area de luz
    * @param width Ancho del area de luz
    * @param height Alto del area de luz
    */
    void setSize(float width, float height);

    /**
    * @brief Establece la direccion de la luz
    * @param direction Direccion de la luz
    */
    void setDirection(Vector3 direction);

    /**
    * @brief Cambia la atenuacion de la luz, es decir, como disminuye con la distancia.
    * @brief Attenuation = Constant + Linear * Distance + Quadratic * Distance^2
    * @brief Por ejemplo, si constant es 0,5 quiere decir que es la mitad de brillante en todas las distancias
    * @param maximumRange Distancia maxima a la que luz tiene efecto (en unidades de mundo)
    * @param constant Nivel base de la atenuacion de la luz (no varia con la distancia) (0-1)
    * @param linear Factor lineal en la atenuacion (0-1)
    * @param quadratic Factor cuadratico en la atenuacion (0-1)
    */
    void setAttenuation(float maximumRange, float constant, float linear, float quadratic);
};
}