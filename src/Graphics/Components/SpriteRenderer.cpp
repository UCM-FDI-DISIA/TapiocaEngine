#include "SpriteRenderer.h"
#include "../GraphicsManager.h"
#include "Components/Transform.h"
#include "Structure/GameObject.h"
#include "../RenderNode.h"
#include "../Billboard.h"

namespace Tapioca {
SpriteRenderer::SpriteRenderer() : billboard(nullptr), node(nullptr), transform(nullptr), billboardName() { }
SpriteRenderer ::~SpriteRenderer() { delete node; }

bool SpriteRenderer::initComponent(const CompMap& variables) {
    // No se ha podido establecer o no hay nombre para el billboard
    if (!setValueFromMap(billboardName, "billboardName", variables) || billboardName == "") {
#ifdef _DEBUG
        std::cerr << "Error: SpriteRenderer: no se pudo inicializar el nombre del billboard.\n";
#endif
        return false;
    }

    bool colorSet = setValueFromMap(color.x, "colorX", variables) && setValueFromMap(color.y, "colorY", variables) &&
        setValueFromMap(color.z, "colorZ", variables) && setValueFromMap(color.w, "colorW", variables);
    if (!colorSet) {
#ifdef _DEBUG
            std::cerr << "No se ha definido un color para el billboard. Se usar� un color por defecto.\n";
#endif
            color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }

    void SpriteRenderer::start() {
        GameObject* gameobject = getObject();
        transform = gameobject->getComponent<Transform>();
        GraphicsManager* g = GraphicsManager::instance();
        node = g->createNode();
        billboard = g->createBillboard(node, billboardName, transform->getGlobalPosition(), color);
    }
    void SpriteRenderer::handleEvent(std::string const& id, void* info) { }
}