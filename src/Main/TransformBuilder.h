#pragma once
#include "Structure/ComponentBuilder.h"

namespace Tapioca {
class TransformBuilder : public ComponentBuilder {
public:
    Component* createComponent() override;
};
}