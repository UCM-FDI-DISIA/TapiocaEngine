#pragma once
#include "Structure/Component.h"

namespace Tapioca {
class Mesh;
class RenderNode;
class Transform;

class TAPIOCA_API MeshRenderer : public Component {
private:
    RenderNode* node;
    Mesh* mesh;
    Transform* transform;
    std::string meshName, materialName;

public:
    COMPONENT_ID("MeshRenderer")
    MeshRenderer();
    ~MeshRenderer();

    bool initComponent(const CompMap& variables) override;
    void start() override;
    void update(uint64_t delt) override;
};
}