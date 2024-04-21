#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector4.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class RenderNode;
class LightSpotlight;
class Transform;

class LightSpotComp : public Component {
private:
    RenderNode* node;
    LightSpotlight* light;
    Vector3 direction;
    Transform* transform;
    Vector4 color;
    float powerScale;
    float attenuationFactor;
    bool attenuationSet;
    float nearClipDist;
    float falloff;
    float innerAngle;
    bool innerAngleSet;
    float outerAngle;
    bool outerAngleSet;

public:
    COMPONENT_ID("LightSpotComp")

    LightSpotComp();
    ~LightSpotComp();

    bool initComponent(const CompMap& variables) override;

    void awake() override;

    void handleEvent(std::string const& id, void* info) override;

    void setColor(const Vector4 color);

    void setPowerScale(const float power);

    void setAttenuation(const float attenuationFactor);

    void setDirection(const Vector3 direction);

    void setNearClipDistance(float distance);

    void setInnerAngle(float degrees);

    void setOuterAngle(float degrees);

    void setFalloff(float falloff);

    void setVisible(const bool enable);

    bool isVisible() const;
};
}