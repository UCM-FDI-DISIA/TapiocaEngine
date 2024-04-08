#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector4.h"

namespace Tapioca {
class Billboard;
class RenderNode;
class Transform;

class TAPIOCA_API SpriteRenderer : public Component {
private:
    RenderNode* node;
    Billboard* billboard;
    Transform* transform;
    Vector4 color;

#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    std::string billboardName;
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

public:
    COMPONENT_ID("SpriteRenderer");
    SpriteRenderer();
    ~SpriteRenderer();

    bool initComponent(const CompMap& variables) override;
    void start() override;
};
}