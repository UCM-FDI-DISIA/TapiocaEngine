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
* @brief Wrapper de la clase LightSpotlight de Ogre
* @brief Simula un cono de luz desde una posicion que apunta en una direccion
* @brief (por lo tanto, requiere de una posicion y una direccion)
* @brief Un cono de luz esta definido por dos angulos:
* @brief       - Inner angle: la intensidad es mayor
* @brief       - Outer angler: la intensidad es menor. Abarca el angulo interior y define la zona que se ilumina
* @brief   Existe el parametro falloff, que indica la caida de la intensidad desde la zona interior hasta la exterior
* @brief   Por ejemplo, un valor de 1.0f corresponde con que esta caida es lineal
*/
class TAPIOCA_API LightSpotlight : public Light {
private:
    friend GraphicsManager;

    /**
    * @brief Constructor de la clase LightSpotlight
    * @param scnMngr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param color Color de la luz
    * @param direction Direccion de la luz
    */
    LightSpotlight(Ogre::SceneManager* const scnMngr, RenderNode* node, Vector4 color, Vector3 direction);

public:
    /**
    * @brief Cambia el tamano de la zona interior de la luz a partir de su angulo
    * @param degrees Grados del angulo
    */
    void setInnerAngle(float degrees);

    /**
    * @brief Cambia el tamano de la zona exterior de la luz a partir de su angulo
    * @param degrees Grados del angulo
    */
    void setOuterAngle(float degrees);

    /**
    * @brief Caida de la intensidad de la luz desde la zona interior hasta la exterior
    * @param falloff Define como es la caida. De forma predeterminda es 1.0f (lineal)
    */
    void setFalloff(float falloff);

    /**
    * @brief Desde que punto comienza a emitir la luz. Por ejemplo, si emite desde el punto de origen
    * o si se recorta la parte principal, de modo que quedaria como un cono truncado
    * @param distance Distancia desde la que comienza a emitir. De forma predeterminada es 0.0f
    */
    void setNearClipDistance(float distance);

    /**
    * @brief Cambia la atenuacion de la luz, es decir, como disminuye con la distancia
    * @brief Attenuation = Constant + Linear * Distance + Quadratic * Distance^2
    * @brief Por ejemplo, si constant es 0,5 quiere decir que es la mitad de brillante en todas las distancias
    * @param maximumRange Distancia maxima a la que luz tiene efecto (en unidades de mundo)
    * @param constant Nivel base de la atenuacion de la luz (no varia con la distancia) (0-1)
    * @param linear Factor lineal en la atenuacion (0-1)
    * @param quadratic Factor cuadratico en la atenuacion (0-1)
    */
    void setAttenuation(float maximumRange, float constant, float linear, float quadratic);

    /**
    * @brief Cambia la direccion de la luz
    * @param direction Nueva direccion de la luz
    */
    void setDirection(Vector3 direction);
};
}