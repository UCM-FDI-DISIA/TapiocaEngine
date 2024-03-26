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
    RenderNode* node;
    Camera* camera;
    Viewport* viewport;
    Transform* transform;

    // Camera
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
    CameraComponent();
    ~CameraComponent();
    bool initComponent(const CompMap& variables) override;
    void awake() override;
    void handleEvent(std::string const& id, void* info) override;

    // Camera
    // Funcionan ambos con posiciones globales
    void lookAt(const Vector3 targetToLook);
    void setDirection(const Vector3 dir);
    // La camara renderiza lo que hay entre en la nearPlane y el farPlane
    void setNearClipDistance(const float dist);
    void setFarClipDistance(const float dist);
    // Otra forma de establecer el nearPlane
    // Se establece a partir del angulo de vision vertical (el horizonal se establece solo,
    // a partir del angulo vertical y del tam de la ventana, que corresponde con el del nearPlane)
    // A mayor angulo, el nearPlane esta mas cerca
    void setFOVYRadians(const float radians);
    void setFOVYDegrees(const float degrees);

    // Viewport
    void setDimensions(const Vector4 dimensions);
    int getWidthInPixels() const;
    int getHeightInPixels() const;
    void setBackground(const Vector3 color);
    void setZOrder(const int zOrder);
};
}