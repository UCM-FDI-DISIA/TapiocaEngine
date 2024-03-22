#pragma once
#include "Structure/Component.h"
#include "Utilities/Vector3.h"

namespace Tapioca {
class Plane;
class RenderNode;
class Transform;

class TAPIOCA_API PlaneComponent : public Component {
private:
    RenderNode* node;
    Plane* plane;
    Transform* transform;
    Vector3 rkNormal;
    float fConstant;
    float width;
    float height;
    int xSegments;
    int ySegments;


#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    std::string planeName;
    std::string materialName;
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
    Plane* getPlane() const;
    Vector3 initialRotation;

public:
    COMPONENT_ID("PlaneComponent")
    PlaneComponent();
    ~PlaneComponent();

    bool initComponent(const CompMap& variables) override;
    void start() override;
    void handleEvent(std::string const& id, void* info) override;
};
}
