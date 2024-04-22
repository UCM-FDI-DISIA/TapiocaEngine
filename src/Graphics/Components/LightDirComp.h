#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"
#include "Utilities/Vector4.h"

namespace Tapioca {
class RenderNode;
class LightDirectional;
class Vector4;

class TAPIOCA_API LightDirComp : public Component {
private:
    const Vector3 INITIAL_DIR = Vector3(0.0f, 0.0f, -1.0f);

    RenderNode* node;
    LightDirectional* light;
    Vector3 direction;
    bool shadows;
    Vector4 color;
    float powerScale;

public:
    COMPONENT_ID("LightDirComp")

    LightDirComp();

    ~LightDirComp();

    bool initComponent(const CompMap& variables) override;

    // puede estar en el start porque no toma nada del transform
    void start() override;

    void setColor(const Vector4 color);

    void setPowerScale(const float power);

    void setDirection(const Vector3 direction);

    void produceShadows(const bool enable);
};
}