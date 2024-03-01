#include "ComponentFactory.h"
#include <iostream>
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "Components/Collider.h"
namespace Tapioca {
Component* ColliderComponentFactory::createComponent() { return new Collider(); }
Component* RigidBodyComponentFactory::createComponent() { return new RigidBody(); }
}
