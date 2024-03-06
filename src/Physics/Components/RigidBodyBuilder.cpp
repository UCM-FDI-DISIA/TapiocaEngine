#include "RigidBodyBuilder.h"
#include "RigidBody.h"

namespace Tapioca {
Component* RigidBodyBuilder::createComponent() { return new RigidBody(); }
}