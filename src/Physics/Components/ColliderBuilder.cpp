#include "ColliderBuilder.h"
#include "Collider.h"

namespace Tapioca {
Component* ColliderBuilder::createComponent() {
    return new Collider(); }
}