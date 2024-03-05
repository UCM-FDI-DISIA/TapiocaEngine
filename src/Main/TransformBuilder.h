#pragma once
#include "Structure/ComponentBuilder.h"

namespace Tapioca {
class TAPIOCA_API TransformBuilder : public ComponentBuilder {
public:
    Component* createComponent() override;
};
}