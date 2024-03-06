#pragma once
#include "Structure/ComponentBuilder.h"

namespace Tapioca {
class TAPIOCA_API ColliderBuilder : public ComponentBuilder {
public:
    Component* createComponent() override;
};
}