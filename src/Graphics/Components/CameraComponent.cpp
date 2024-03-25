#include "CameraComponent.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "../GraphicsManager.h"
#include "../RenderNode.h"
#include "../Camera.h"
#include "../Viewport.h"

namespace Tapioca {
CameraComponent::CameraComponent()
    : transform(nullptr), node(nullptr), camera(nullptr), viewport(nullptr), color(0.0f, 0.0f, 0.0f, 1.0f), zOrder(),
      dimensions(0.0f, 0.0f, 1.0f, 1.0f) { }

CameraComponent::~CameraComponent() {
    delete node;
    delete viewport;
}

bool CameraComponent::initComponent(const CompMap& variables) {
    // Viewport
    if (!setValueFromMap(zOrder, "zOrder", variables)) {
#ifdef _DEBUG
        std::cerr << "Error: CameraComponent: no se pudo inicializar el zOrder.\n";
#endif
        return false;
    }

    bool colorSet = setValueFromMap(color.x, "colorR", variables) && setValueFromMap(color.y, "colorG", variables) &&
        setValueFromMap(color.z, "colorB", variables) && setValueFromMap(color.w, "colorA", variables);
    if (!colorSet) {
#ifdef _DEBUG
        std::cerr << "CameraComponent: el color predeterminado del fondo es negro.\n";
#endif
    }

    if (!setValueFromMap(dimensions.x, "left", variables)) {
#ifdef _DEBUG
        std::cerr << "CameraComponent: la camara esta situdada a la izquierda del todo.\n";
#endif
    }

    if (!setValueFromMap(dimensions.y, "top", variables)) {
#ifdef _DEBUG
        std::cerr << "CameraComponent: la camara esta situada arriba del todo.\n";
#endif
    }

    if (!setValueFromMap(dimensions.z, "width", variables)) {
#ifdef _DEBUG
        std::cerr << "CameraComponent: el ancho de la camara corresponde con el de la ventana.\n";
#endif
    }

    if (!setValueFromMap(dimensions.w, "height", variables)) {
#ifdef _DEBUG
        std::cerr << "CameraComponent: la altura de la camara corresponde con la de la ventana.\n";
#endif
    }
}

void CameraComponent::awake() {
    transform = getObject()->getComponent<Transform>();

    GraphicsManager* graphicsManager = GraphicsManager::instance();
    node = graphicsManager->createNode();
    camera = graphicsManager->createCameraWithName(node);
    viewport = graphicsManager->createViewport(camera, zOrder);

    // Viewport
    if (color.x != 0.0f || color.y != 0.0f || color.z != 0.0f || color.w != 1.0f) {
        viewport->setBackground(color);
    }
    if (dimensions.x != 0.0f) {
        viewport->setLeft(dimensions.x);
    }
    if (dimensions.y != 0.0f) {
        viewport->setTop(dimensions.y);
    }
    if (dimensions.z != 1.0f) {
        viewport->setWidth(dimensions.z);
    }
    if (dimensions.w != 1.0f) {
        viewport->setHeight(dimensions.z);
    }
}

void CameraComponent::handleEvent(std::string const& id, void* info) {
    if (id == "transformChanged") {
        node->setPosition(transform->getGlobalPosition());
        // TODO: hacer que gire respecto de su padre y no de si mismo
        node->setRotation(transform->getGlobalRotation());
        // es tan grande que no se ve
        node->setScale(transform->getGlobalScale());
    }
}

void CameraComponent::setDimensions(const Vector4 dimensions) { viewport->setDimensions(dimensions); }

int CameraComponent::getWidthInPixels() const { return viewport->getWidthInPixels(); }

int CameraComponent::getHeightInPixels() const { return viewport->getHeightInPixels(); }

void CameraComponent::setBackground(const Vector4 color) {
    this->color = color;
    viewport->setBackground(color);
}

void CameraComponent::setZOrder(const int zOrder) {
    this->zOrder = zOrder;
    viewport->setZOrder(zOrder);
}
}