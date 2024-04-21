#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector4.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class RenderNode;
class LightRectlight;
class Transform;

class LightRectComp : public Component {
private:
    RenderNode* node;
    LightRectlight* light;
    Vector3 direction;
    Transform* transform;
    Vector4 color;
    float powerScale;
    float attenuationFactor;
    bool attenuationSet;
    float width;
    float height;

public:
    COMPONENT_ID("LightRectComp")

    LightRectComp();
    ~LightRectComp();

    bool initComponent(const CompMap& variables) override;

    void awake() override;

    void handleEvent(std::string const& id, void* info) override;

    void setColor(const Vector4 color);

    void setPowerScale(const float power);

    void setAttenuation(const float attenuationFactor);

    void setDirection(const Vector3 direction);

    void setWidth(const float width);
    
    void setHeight(const float height);

    void setSize(const float width, const float height);

    void setVisible(const bool enable);

    bool isVisible() const;
};
}