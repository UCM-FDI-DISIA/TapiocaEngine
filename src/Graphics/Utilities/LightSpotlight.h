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

class TAPIOCA_API LightSpotlight : public Light {
private:
    friend GraphicsManager;

    // SIMULA UN CONO DE LUZ DESDE UNA POSICION QUE APUNTA EN UNA DIRECCION
    // (POR LO TANTO, REQUIERE DE UNA POSICION Y UNA DIRECCION)
    LightSpotlight(Ogre::SceneManager* sceneManager, RenderNode* node, Vector4 color, Vector3 direction);

public:
    /*
    Un cono de luz esta definido por dos angulos:
        - Inner angle: la intensidad es mayor
        - Outer angler: la intensidad es menor. Abarca el angulo interior y define la zona que se ilumina
    Existe el parametro falloff, que indica la caida de la intensidad desde la zona interior hasta la exterior
    Por ejemplo, un valor de 1.0f corresponde con que esta caida es lineal
    */

    /*
    * @brief Cambia el tam de la zona interior de la luz a partir de su angulo
    * @param degrees Grados del angulo
    */
    void setInnerAngle(float degrees);

    /*
    * @brief Cambia el tam de la zona exterior de la luz a partir de su angulo
    * @param degrees Grados del angulo
    */
    void setOuterAngle(float degrees);

    /*
    * @brief Caida de la intensidad de la luz desde la zona interior hasta la exterior
    * @param falloff Define como es la caida. De forma predeterminda es 1.0f (lineal)
    */
    void setFalloff(float falloff);

    /*
    * @brief Desde que punto comienza a emitir la luz. Por ejemplo, si emite desde el punto de origen
    * o si se recorta la parte principal, de modo que quedaria como un cono truncado
    * @param distance Distancia desde la que comienza a emitir. De forma predeterminada es 0.0f
    */
    void setNearClipDistance(float distance);

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

    void setDirection(Vector3 direction);
};
}