#pragma once
#include "Structure/ComponentFactory.h"

namespace Tapioca {
class TransformBuilder : public ComponentFactory {
public:
    Component* createComponent() override;
};
}