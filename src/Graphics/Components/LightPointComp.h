#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector4.h"

namespace Tapioca {
class RenderNode;
class LightPoint;
class Transform;

class TAPIOCA_API LightPointComp : public Component {
private:
    RenderNode* node;
    LightPoint* light;
    Transform* transform;
    Vector4 color;
    float powerScale;
    float attenuationFactor;
    bool attenuationSet;

public:
    COMPONENT_ID("LightPointComp")

    LightPointComp();
    ~LightPointComp();

    bool initComponent(const CompMap& variables) override;

    void awake() override;

    void handleEvent(std::string const& id, void* info) override;

    void setColor(const Vector4 color);

    void setPowerScale(const float power);

    void setAttenuation(const float attenuationFactor);

    void setVisible(const bool enable);
    bool isVisible() const;
};
}