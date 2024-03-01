#pragma once
#include "Structure/ComponentBuilder.h"

namespace Tapioca {
class ColliderBuilder : public ComponentBuilder {
public:
    Component* createComponent() override;
};
}