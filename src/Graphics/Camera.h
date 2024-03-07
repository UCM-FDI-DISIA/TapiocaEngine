#pragma once
#include "Utilities/Vector3.h"
#include "RenderObject.h"

namespace Ogre {
class Camera;
class SceneManager;
}

namespace Tapioca {
class RenderNode;
class GraphicsEngine;
class Viewport;

class TAPIOCA_API Camera : public RenderObject {
private:
    friend GraphicsEngine;
    friend Viewport;

    Ogre::Camera* mCam;
    bool autoAspectRatio;

    // aspectRatio = width/height   1.3 es el por defecto de Ogre
    Camera(Ogre::SceneManager* const scnMgr, RenderNode* const node, std::string const& name,
           const Vector3 targetToLook = Vector3(0, 0, 0), const float nearDist = 1, const float farDist = 1000,
           const bool autoAspectRatio = true, const float aspectRatio = 1.33333333333333f);

    inline Ogre::Camera* getCamera() { return mCam; };

public:
    /*
    la camara mira hacia el eje z negativo
    El punto indicado es la que mira
    Por debajo funciona como un setDirection, por lo tanto, rota la camara
    (en este caso en torno al espacio mundial, pero se podria cambiar)
    */
    void lookAt(const Vector3 targetToLook);
    void setDirection(const Vector3 dir);

    /*
    el frustum representa el espacio en el mundo al alcance de nuestra vision
    Se trata de una piramide truncada definida por un plano mas cercano (nearPlano, que corresponde
    con la ventana grafica y su tam no cambia) y el plano mas lejano (farPlane)
    Se puede renderizar todo y mostrar solo lo que esta en esta piramida o solo renderizar lo que hay dentro (frustum culling)
    */

    /*
    1 forma de definir el near plane
    Se indica la distancia. Cuanto mas lejos este el near plane y mayor sea la distancia entre el near plane y el far plane,
    menos preciso sera el buffer Z de profundidad
    */
    void setNearClipDistance(const float dist);
    /*
    2 forma de definir el near plane
    Como el tam del near distance es constante, dependiendo de cuanto lo acerquemos o lo alejemos,
    el angulo que forma con la posicion de la camara es mayor o menor, respectivamente
    Se puede indicar este angulo para ajustar la distancia del near plane
    X ej, un angulo de 90º corresponde con que tenemos una vision de 90º en el eje vertical (el horizontal se calcula solo,
    a partir del anguno en el eje vertical y el tam de la ventana grafica/near plane)
    */
    void setFOVYRadians(const float radians);
    void setFOVYDegrees(const float degrees);

    /*
    Se indica la distancia. Si es 0 es infinita.
    Si es muy grande la distancia, puede haber problemas de tartamudeo (por precision), 
    por ejemplo con las luces o sombras si se encuentran muy lejos
    */
    void setFarClipDistance(const float dist);

    void setAspectRatio(const float aspectRatio);
};
}