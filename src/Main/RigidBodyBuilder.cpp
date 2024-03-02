#include "RigidBodyBuilder.h"
#include "Components/RigidBody.h"

namespace Tapioca {
Component* RigidBodyBuilder::createComponent() { return new RigidBody(); }
}