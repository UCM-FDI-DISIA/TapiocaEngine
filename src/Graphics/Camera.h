#pragma once
#include <string>
#include "Utilities/Vector3.h"
#include "RenderObject.h"

namespace Ogre {
class Camera;
class SceneManager;
}
namespace Tapioca {
class Node;
class GraphicsEngine;
class Viewport;
}
namespace Tapioca {
class Camera : public RenderObject {
public:
    friend GraphicsEngine;
    friend Viewport;

private:
    Ogre::Camera* mCam;
    bool autoAspectRatio;

    // aspectRatio = width/height   1.3 es el por defecto de Ogre
    Camera(Ogre::SceneManager* scnMgr, Node* node, std::string name, Vector3 targetToLook = Vector3(0, 0, 0),
        float nearDist = 1, float farDist = 1000, bool autoAspectRatio = true, float aspectRatio = 1.33333333333333f);

    Ogre::Camera* getCamera() { return mCam; };

public:
    /*
    * LO HE PUESTO YO DE MIS APUNTES, PERO SE PUEDE BORRAR
    la camara mira hacia el eje z negativo
    El punto indicado es la que mira
    Por debajo funciona como un setDirection, por lo tanto, rota la camara
    (en este caso en torno al espacio mundial, pero se podria cambiar)
    */
    void lookAt(Vector3 targetToLook);
    void setDirection(Vector3 dir);

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
    void setNearClipDistance(float dist);
    /*
    2 forma de definir el near plane
    Como el tam del near distance es constante, dependiendo de cuanto lo acerquemos o lo alejemos,
    el angulo que forma con la posicion de la camara es mayor o menor, respectivamente
    Se puede indicar este angulo para ajustar la distancia del near plane
    X ej, un angulo de 90º corresponde con que tenemos una vision de 90º en el eje vertical (el horizontal se calcula solo,
    a partir del anguno en el eje vertical y el tam de la ventana grafica/near plane)
    */
    void setFOVYRadians(float radians);
    void setFOVYDegrees(float degrees);

    /*
    Se indica la distancia. Si es 0 es infinita.
    Si es muy grande la distancia, puede haber problemas de tartamudeo (por precision), 
    por ejemplo con las luces o sombras si se encuentran muy lejos
    */
    void setFarClipDistance(float dist);

    void setAspectRatio(float aspectRatio);
};
}