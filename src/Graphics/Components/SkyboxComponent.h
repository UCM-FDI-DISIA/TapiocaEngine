#pragma once
#include "Structure/Component.h"

namespace Tapioca {

class TAPIOCA_API SkyboxComponent : public Component {
private:
public:
    COMPONENT_ID("Skybox")
    SkyboxComponent();
    ~SkyboxComponent();
};

}
