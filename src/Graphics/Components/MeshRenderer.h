#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class Mesh;
class RenderNode;
class Transform;
class Animator;

class TAPIOCA_API MeshRenderer : public Component {
    friend Animator;

private:
    RenderNode* node;
    Mesh* mesh;
    Transform* transform;
    std::string meshName, materialName;
    Mesh* getMesh() const;
    Vector3 initialRotation;

public:
    COMPONENT_ID("MeshRenderer")
    MeshRenderer();
    ~MeshRenderer();

    bool initComponent(const CompMap& variables) override;
    // se necesita una awake para que el animator puede obtener el mesh
    void awake() override;
    void handleEvent(std::string const& id, void* info) override;
};
}