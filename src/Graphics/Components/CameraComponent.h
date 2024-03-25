#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector4.h"

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
    // x - left, y - top, z - width, w - height
    Vector4 dimensions;

    // Viewport
    int zOrder;
    Vector4 color;

public:
    COMPONENT_ID("CameraComponent")
    CameraComponent();
    ~CameraComponent();
    bool initComponent(const CompMap& variables) override;
    void awake() override;
    void handleEvent(std::string const& id, void* info) override;

    // Viewport
    void setDimensions(const Vector4 dimensions);
    int getWidthInPixels() const;
    int getHeightInPixels() const;
    void setBackground(const Vector4 color);
    void setZOrder(const int zOrder);
};
}