#pragma once
#include "Structure/ComponentBuilder.h"

namespace Tapioca {
class RigidBodyBuilder : public ComponentBuilder {
public:
    Component* createComponent() override;
};
}