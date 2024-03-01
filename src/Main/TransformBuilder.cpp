#include "TransformBuilder.h"
#include "Components/Transform.h"
#include "Node.h"
#include "GraphicsEngine.h"

namespace Tapioca {
Component* TransformBuilder::createComponent() {
    Transform* tr = new Transform();
    tr->node = GraphicsEngine::instance()->createNode();
    tr->node->transform = tr;
    return tr;
}
}