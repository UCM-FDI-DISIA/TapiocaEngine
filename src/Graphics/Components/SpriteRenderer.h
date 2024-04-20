#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector4.h"

namespace Tapioca {
class Billboard;
class BillboardSet;
class RenderNode;
class Transform;

class TAPIOCA_API SpriteRenderer : public Component {
private:
    RenderNode* node;
    BillboardSet* billboardSet;
    Billboard* billboard;
    Transform* transform;
    Vector4 color;

// FALTA PONER EL WARNING
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

public:
    COMPONENT_ID("SpriteRenderer");
    SpriteRenderer();
    ~SpriteRenderer();

    bool initComponent(const CompMap& variables) override;
    void start() override;
    void handleEvent(std::string const& id, void* info) override;

    void setVisible(const bool enable);
    bool isVisible() const;
};
}