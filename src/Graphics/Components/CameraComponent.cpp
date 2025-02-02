#include "CameraComponent.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "GraphicsManager.h"
#include "Utilities/RenderNode.h"
#include "Utilities/Camera.h"
#include "Utilities/Viewport.h"
#include "checkML.h"

namespace Tapioca {
CameraComponent::CameraComponent()
    : node(nullptr), transform(nullptr), camera(nullptr), viewport(nullptr), color(-1.0f, -1.0f, -1.0f), zOrder(0),
      dimensions(0.0f, 0.0f, 1.0f, 1.0f), targetToLook(), direction(0.0f, 0.0f, 0.0f), nearPlane(-1.0f),
      farPlane(-1.0f), targetToLookSet(false), applyInitRot(false) { }

CameraComponent::~CameraComponent() {
    GraphicsManager::instance()->removeZOrder(zOrder);
    delete node;
    node = nullptr;
    delete viewport;
    viewport = nullptr;
    camera = nullptr;
    transform = nullptr;
}

bool CameraComponent::initComponent(const CompMap& variables) {
    // Camera
    bool directionSet = setValueFromMap(direction.x, "directionX", variables) &&
                        setValueFromMap(direction.y, "directionY", variables) && setValueFromMap(direction.z, "directionZ", variables);
    if (!directionSet) {
        targetToLookSet = setValueFromMap(targetToLook.x, "targetToLookX", variables) &&
                          setValueFromMap(targetToLook.y, "targetToLookY", variables) &&
                          setValueFromMap(targetToLook.z, "targetToLookZ", variables);
    }

    if (!setValueFromMap(nearPlane, "nearPlane", variables)) {
        logInfo("CameraComponent: La distancia por defecto al nearPlane es 1.0f.");
    }

    if (!setValueFromMap(nearPlane, "farPlane", variables)) {
        logInfo("CameraComponent: La distancia por defecto al farPlane es 1000.0f.");
    }

    // Viewport
    if (!setValueFromMap(zOrder, "zOrder", variables)) {
        logInfo("CameraComponent: No se pudo inicializar el zOrder. Se usa zOrder 0 por defecto.");
    }

    bool colorSet = setValueFromMap(color.x, "bgColorR", variables) &&
        setValueFromMap(color.y, "bgColorG", variables) && setValueFromMap(color.z, "bgColorB", variables);
    if (!colorSet) {
        logInfo("CameraComponent: El color predeterminado del fondo es negro.");
    }

    if (!setValueFromMap(dimensions.x, "left", variables)) {
        logInfo("CameraComponent: La camara esta situada a la izquierda del todo.");
    }
    if (!setValueFromMap(dimensions.y, "top", variables)) {
        logInfo("CameraComponent: La camara esta situada arriba del todo.");
    }
    if (!setValueFromMap(dimensions.z, "width", variables)) {
        logInfo("CameraComponent: El ancho de la camara corresponde con el de la ventana.");
    }
    if (!setValueFromMap(dimensions.w, "height", variables)) {
        logInfo("CameraComponent: La altura de la camara corresponde con la de la ventana.");
    }

    return true;
}

void CameraComponent::awake() {
    transform = object->getComponent<Transform>();

    GraphicsManager* graphicsManager = GraphicsManager::instance();
    int zOrderAux = graphicsManager->askForZOrder(zOrder);
    if (zOrderAux > -1) {
        if (zOrderAux != zOrder) {
            logWarn(("CameraComponent: El zOrder que se ha pedido no esta disponible. Se usa zOrder " +
                     std::to_string(zOrderAux))
                        .c_str());
        }
        zOrder = zOrderAux;

        node = graphicsManager->createNode(transform->getGlobalPositionWithoutRotation());
        camera = graphicsManager->createCamera(node, "Camera " + std::to_string(zOrder));
        viewport = graphicsManager->createViewport(camera, zOrder);

        if (direction != Vector3(0.0f, 0.0f, 0.0f)) camera->setDirection(direction);
        else {
            if (targetToLookSet) camera->lookAt(targetToLook);
            else {
                Vector3 globalRot = transform->getGlobalRotation().toEuler();
                if (globalRot == Vector3(0.0f, 0.0f, 0.0f)) setDirection(INITIAL_DIR);
                else
                    applyInitRot = true;
            }
        }

        if (nearPlane != -1.0f) camera->setNearClipDistance(nearPlane);
        if (farPlane != -1.0f) camera->setFarClipDistance(farPlane);

        // Viewport
        if (color != Vector3(-1.0f, -1.0f, -1.0f)) viewport->setBackground(Vector4(color, 1.0f));
        if (dimensions.x != 0.0f) viewport->setLeft(dimensions.x);
        if (dimensions.y != 0.0f) viewport->setTop(dimensions.y);
        if (dimensions.z != 1.0f) viewport->setWidth(dimensions.z);
        if (dimensions.w != 1.0f) viewport->setHeight(dimensions.z);
    }
    else {
        logError("CameraComponent: No queda nigun zOrder disponible");
        alive = false;
    }
}

void CameraComponent::handleEvent(std::string const& id, void* info) {
    if (node != nullptr) {
        if (id == "posChanged") node->setPosition(transform->getGlobalPositionWithoutRotation());
        else if (id == "rotChanged") {
            if (applyInitRot) node->setRotation(transform->getGlobalRotation());
            applyInitRot = true;
        }
    }
}

void CameraComponent::lookAt(const Vector3 targetToLook) {
    if (camera != nullptr) {
        this->targetToLook = targetToLook;
        camera->lookAt(targetToLook);
    }
}

void CameraComponent::setDirection(const Vector3 dir) {
    if (camera != nullptr) {
        direction = dir;
        camera->setDirection(dir);
    }
}

void CameraComponent::setNearClipDistance(const float dist) {
    if (camera != nullptr) {
        nearPlane = camera->getNearClipDistance();
        camera->setNearClipDistance(dist);
    }
}

void CameraComponent::setFarClipDistance(const float dist) {
    if (camera != nullptr) {
        farPlane = camera->getFarClipDistance();
        camera->setFarClipDistance(dist);
    }
}

void CameraComponent::setFOVYRadians(const float radians) {
    if (camera != nullptr) {
        nearPlane = camera->getNearClipDistance();
        camera->setFOVYRadians(radians);
    }
}

void CameraComponent::setFOVYDegrees(const float degrees) {
    if (camera != nullptr) {
        nearPlane = camera->getNearClipDistance();
        camera->setFOVYDegrees(degrees);
    }
}

void CameraComponent::setDimensions(const Vector4 dimensions) {
    if (viewport != nullptr) {
        this->dimensions = dimensions;
        viewport->setDimensions(dimensions);
    }
}

int CameraComponent::getWidthInPixels() const {
    if (viewport != nullptr) return viewport->getWidthInPixels();
    else return 0;
}

int CameraComponent::getHeightInPixels() const {
    if (viewport != nullptr) return viewport->getHeightInPixels();
    else
        return 0;
}

void CameraComponent::setBackground(const Vector3 color) {
    if (viewport != nullptr) {
        this->color = color;
        viewport->setBackground(Vector4(color, 1.0f));
    }
}

void CameraComponent::setZOrder(const int zOrder) {
    if (viewport != nullptr) {
        GraphicsManager* graphicsManager = GraphicsManager::instance();
        graphicsManager->removeZOrder(this->zOrder);
        this->zOrder = graphicsManager->askForZOrder(zOrder);
        if (this->zOrder != -1) {
            if (zOrder != this->zOrder) {
                logWarn(("CameraComponent: El zOrder que se ha pedido no esta disponible. Se usa zOrder " +
                         std::to_string(this->zOrder))
                            .c_str());
            }
            viewport->setZOrder(this->zOrder);
        }
        else {
            logError("CameraComponent: No queda nigun zOrder disponible");
            alive = false;
        }
    }
}
}