#pragma once
#include "Structure/ComponentBuilder.h"

namespace Tapioca {
class TAPIOCA_API RigidBodyBuilder : public ComponentBuilder {
public:
    Component* createComponent() override;
};
}