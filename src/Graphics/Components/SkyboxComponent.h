#pragma once
#include "Structure/Component.h"

namespace Tapioca {

class TAPIOCA_API SkyboxComponent : public Component {
private:
public:
    COMPONENT_ID("Skybox")
    SkyboxComponent();
    ~SkyboxComponent();

    bool initComponent(const CompMap& variables) override;
    void start() override;
    void update(uint64_t delt) override;
};
}
