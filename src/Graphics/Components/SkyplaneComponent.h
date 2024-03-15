#pragma once
#include "Structure/Component.h"

namespace Tapioca {

class TAPIOCA_API SkyplaneComponent : public Component {
private:
public:
    COMPONENT_ID("Skyplane")
    SkyplaneComponent();
    ~SkyplaneComponent();
};
}
