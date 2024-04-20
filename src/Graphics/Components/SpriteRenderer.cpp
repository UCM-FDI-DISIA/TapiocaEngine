#include "SpriteRenderer.h"
#include "GraphicsManager.h"
#include "Components/Transform.h"
#include "Structure/GameObject.h"
#include "Utilities/RenderNode.h"
#include "Utilities/Billboard.h"
#include "Utilities/BillboardSet.h"

namespace Tapioca {
SpriteRenderer::SpriteRenderer() : billboardSet(nullptr), billboard(nullptr), node(nullptr), transform(nullptr) { }
SpriteRenderer ::~SpriteRenderer() { delete node; }

bool SpriteRenderer::initComponent(const CompMap& variables) {
    bool colorSet = setValueFromMap(color.x, "colorX", variables) && setValueFromMap(color.y, "colorY", variables) &&
        setValueFromMap(color.z, "colorZ", variables) && setValueFromMap(color.w, "colorW", variables);
    if (!colorSet) {
        logInfo("SpriteRenderer: No se ha definido un color para el billboard. Se usará un color por defecto.");
        color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    return true;
}

void SpriteRenderer::start() {
    GameObject* gameobject = getObject();
    transform = gameobject->getComponent<Transform>();
    GraphicsManager* g = GraphicsManager::instance();
    node = g->createNode();
    billboardSet = g->createBillboardSetWithName(node, 1);
    billboard = billboardSet->addBillboard(Vector3(0.0f, 0.0f, 0.0f), color);
    //billboard = g->createBillboard(node, billboardName, transform->getGlobalPosition(), color);
}

void SpriteRenderer::handleEvent(std::string const& id, void* info) {
    if (id == "posChanged") {
        node->setPosition(transform->getGlobalPositionWithoutRotation());
    }
}

void SpriteRenderer::setVisible(const bool enable) { billboardSet->setVisible(enable); }
bool SpriteRenderer::isVisible() const { return billboardSet->isVisible(); }
}