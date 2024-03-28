#pragma once
#include <vector>
#include <string>
#include "Utilities/Vector3.h"
#include "RenderObject.h"

namespace Ogre {
class Camera;
class SceneManager;
}

namespace Tapioca {
class RenderNode;
class GraphicsManager;
class Viewport;

/*
* @brief Wrapper de la clase Camera de Ogre.
*/
class TAPIOCA_API Camera : public RenderObject {
private:
    friend GraphicsManager;
    friend Viewport;

    Ogre::Camera* mCam;         // Camara de Ogre
    bool autoAspectRatio;

    /*
    * @brief Constructora de la clase BillboardSet.
    * @param scnMgr Puntero al manager de escenas de ogre
    * @param node Nodo para renderizado
    * @param name Nombre de la camara
    * @param targetToLook Direccion a la que apunta 
    * @param nearDist Distancia a la que se encuentra el primer rectangulo que define el frustum
    * @param farDist Distancia a la que se encuentra el segundo rectangulo que define el frustum
    * @param autoAspectRatio True si se quiere establecer el aspectRatio de forma automatica, false en caso contrario
    * @param aspectRatio Relacion de aspecto que se quiere que tenga la camara (aspectRatio = width/height, 1.3 es el por defecto de Ogre)
    */
    Camera(Ogre::SceneManager* const scnMgr, RenderNode* const node, std::string const& name,
           const Vector3 targetToLook = Vector3(0, 0, 0), const float nearDist = 1, const float farDist = 1000,
           const bool autoAspectRatio = true, const float aspectRatio = 1.33333333333333f);

    /*
    * @brief Devuelve un puntero a la camara de Ogre
    * @return Puntero a la camara de Ogre
    */
    inline Ogre::Camera* getCamera() { return mCam; };

public:
    /*
    * @brief Cambia la direccion a la que mira la camara.
    * Por debajo funciona como un setDirection, por lo tanto, rota la camara
    * (en este caso en torno al espacio mundial, pero se podria cambiar)
    * @param targetToLook Direccion a la que se quiere que mire la camara
    */
    void lookAt(const Vector3 targetToLook);
    /*
    * @brief Cambia la direccion de la camara.
    * @param targetToLook Direccion a la que se quiere que poner la camara
    */
    void setDirection(const Vector3 dir);

    /*
    el frustum representa el espacio en el mundo al alcance de nuestra vision
    Se trata de una piramide truncada definida por un plano mas cercano (nearPlano, que corresponde
    con la ventana grafica y su tam no cambia) y el plano mas lejano (farPlane)
    Se puede renderizar todo y mostrar solo lo que esta en esta piramida o solo renderizar lo que hay dentro (frustum culling)
    */

    /*
    * @brief Cambia el valor del rectangulo cercano que define el frustum. 
    * Se indica la distancia. Cuanto mas lejos este el near plane y mayor sea la distancia entre el near plane y el far plane,
    * menos preciso sera el buffer Z de profundidad
    * @param dist Distancia a la que se quiere poner de la camara el regtangulo
    */
    void setNearClipDistance(const float dist);
    /*
    * @brief Cambia el valor del rectangulo lejano que define el frustum. 
    * Si es muy grande la distancia, puede haber problemas de tartamudeo (por precision), 
    * por ejemplo con las luces o sombras si se encuentran muy lejos
    * @param dist Distancia a la que se quiere poner de la camara el regtangulo. Si es 0 es infinita.
    */
    void setFarClipDistance(const float dist);
    /*
    * @brief Cambia el valor del rectangulo cercano que define el frustum. 
    * Como el tam del near distance es constante, dependiendo de cuanto lo acerquemos o lo alejemos,
    * el angulo que forma con la posicion de la camara es mayor o menor, respectivamente
    * Se puede indicar este angulo para ajustar la distancia del near plane
    * X ej, un angulo de 90º corresponde con que tenemos una vision de 90º en el eje vertical (el horizontal se calcula solo,
    * a partir del anguno en el eje vertical y el tam de la ventana grafica/near plane)
    * @param radians Angulo en radianes al que se cambia el trangulo que forma la camara con el rectangulo
    */
    void setFOVYRadians(const float radians);
    /*
    * @brief Cambia el valor del rectangulo cercano que define el frustum. 
    * Como el tam del near distance es constante, dependiendo de cuanto lo acerquemos o lo alejemos,
    * el angulo que forma con la posicion de la camara es mayor o menor, respectivamente
    * Se puede indicar este angulo para ajustar la distancia del near plane
    * X ej, un angulo de 90º corresponde con que tenemos una vision de 90º en el eje vertical (el horizontal se calcula solo,
    * a partir del anguno en el eje vertical y el tam de la ventana grafica/near plane)
    * @param radians Angulo en grados al que se cambia el trangulo que forma la camara con el rectangulo
    */
    void setFOVYDegrees(const float degrees);
    /*
    * @brief Cambia el valor de la relacion de aspecto que tiene la camara
    * @param aspectRatio Relacion de aspecto a la que se quiere poner de la camara
    */
    void setAspectRatio(const float aspectRatio);

    float getNearClipDistance() const;

    float getFarClipDistance() const;
};
}