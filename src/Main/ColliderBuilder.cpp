#include "ColliderBuilder.h"
#include "Components/Collider.h"

namespace Tapioca {
Component* ColliderBuilder::createComponent() {
    return new Collider(); }
}