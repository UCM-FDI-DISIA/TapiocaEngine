#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector4.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class RenderNode;
class Camera;
class Viewport;
class Transform;

class TAPIOCA_API CameraComponent : public Component {
private:
    const Vector3 INITIAL_DIR = Vector3(0.0f, 0.0f, -1.0f);

    RenderNode* node;
    Camera* camera;
    Viewport* viewport;
    Transform* transform;

    // Camera
    bool targetToLookSet;
    Vector3 targetToLook;   // la camara de Ogre apunta por defecto a (0,0,0) global
    Vector3 direction;
    float nearPlane;
    float farPlane;

    // Viewport
    // x - left, y - top, z - width, w - height
    Vector4 dimensions;
    int zOrder;
    Vector3 color;

public:
    COMPONENT_ID("CameraComponent")
    /*
    * @brief Constructora de la clase CameraComponent
    */
    CameraComponent();
    /*
    * @brief Destructora de la clase CameraComponent
    */
    ~CameraComponent();

    /*
    * @brief Metodo que se usa para recibir los parametros iniciales y guardarlos.
    * No garantiza que todos los componentes iniciales esten creados
    * @param variables unordered_map con los parametros iniciales
    * @return Devuelve true si se ha inicializado correctamente, false en caso contrario
    */
    bool initComponent(const CompMap& variables) override;
    /*
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * Garantiza que todos los componentes iniciales esten creados
    */
    void awake() override;
    /*
    * @brief Metodo que se usa para recibir eventos.
    * Se llama nada mas se recibe el evento, si es un evento de ventana se recibe en el pushEvent.
    * @param Id indica el tipo de mensaje
    * @param info puntero a void para pasar parametros
    */
    void handleEvent(std::string const& id, void* info) override;

    void free();

    // Camera
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
    // Otra forma de establecer el nearPlane
    // Se establece a partir del angulo de vision vertical (el horizonal se establece solo,
    // a partir del angulo vertical y del tam de la ventana, que corresponde con el del nearPlane)
    // A mayor angulo, el nearPlane esta mas cerca
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

    // Viewport
    void setDimensions(const Vector4 dimensions);
    int getWidthInPixels() const;
    int getHeightInPixels() const;
    void setBackground(const Vector3 color);
    void setZOrder(const int zOrder);
    int getZOrder() const;
};
}