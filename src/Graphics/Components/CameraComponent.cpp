#include "CameraComponent.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "GraphicsManager.h"
#include "Utilities/RenderNode.h"
#include "Utilities/Camera.h"
#include "Utilities/Viewport.h"

namespace Tapioca {
CameraComponent::CameraComponent()
    : transform(nullptr), node(nullptr), camera(nullptr), viewport(nullptr), color(-1.0f, -1.0f, -1.0f), zOrder(),
      dimensions(0.0f, 0.0f, 1.0f, 1.0f), targetToLook(), direction(0.0f, 0.0f, 0.0f), nearPlane(-1.0f),
      farPlane(-1.0f), targetToLookSet(false) { }

CameraComponent::~CameraComponent() { 
    //GraphicsManager::instance()->removeCameraCompByZOrder(zOrder, true);
    GraphicsManager::instance()->removeZOrder(zOrder);
    free();
}

bool CameraComponent::initComponent(const CompMap& variables) {
    // Camera
    bool directionSet = setValueFromMap(direction.x, "directionX", variables) &&
        setValueFromMap(direction.y, "directionY", variables) && setValueFromMap(direction.z, "directionZ", variables);
    if (!directionSet) {
        targetToLookSet = setValueFromMap(targetToLook.x, "targetToLookX", variables) &&
            setValueFromMap(targetToLook.y, "targetToLookY", variables) &&
            setValueFromMap(targetToLook.z, "targetToLookZ", variables);
        if (!targetToLookSet) {
            logInfo("CameraComponent: La camara apunta hacia (0,0,-1) global.");
        }
    }

    if (!setValueFromMap(nearPlane, "nearPlane", variables)) {
        logInfo("CameraComponent: La distancia por defecto al nearPlane es 1.0f.");
    }

    if (!setValueFromMap(nearPlane, "farPlane", variables)) {
        logInfo("CameraComponent: La distancia por defecto al farPlane es 1000.0f.");
    }

    // Viewport
    if (!setValueFromMap(zOrder, "zOrder", variables)) {
        logError("CameraComponent: No se pudo inicializar el zOrder.");
        return false;
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
    transform = getObject()->getComponent<Transform>();

    GraphicsManager* graphicsManager = GraphicsManager::instance();
    //graphicsManager->removeCameraCompByZOrder(zOrder, true);
    int zOrderAux = graphicsManager->askForZOrder(zOrder);
    if (zOrderAux != -1) {
        if (zOrderAux != zOrder) {
            std::string message = "CameraComponent: El zOrder que se ha pedido no esta disponible. Se usa zOrder " +
                std::to_string(zOrderAux);
            logWarn(message.c_str());
        }
        zOrder = zOrderAux;

        node = graphicsManager->createNode();
        camera = graphicsManager->createCamera(node, "Camera " + std::to_string(zOrder));
        viewport = graphicsManager->createViewport(camera, zOrder);
        //graphicsManager->saveCameraComp(this);

        if (direction != Vector3(0.0f, 0.0f, 0.0f)) {
            camera->setDirection(direction);
        }
        else if (!targetToLookSet) {
            setDirection(INITIAL_DIR);
        }

        if (nearPlane != -1.0f) {
            camera->setNearClipDistance(nearPlane);
        }
        if (farPlane != -1.0f) {
            camera->setFarClipDistance(farPlane);
        }

        // Viewport
        if (color != Vector3(-1.0f, -1.0f, -1.0f)) {
            viewport->setBackground(Vector4(color, 1.0f));
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
    else {
        logError("CameraComponent: No queda nigun zOrder disponible");
    }
}

void CameraComponent::handleEvent(std::string const& id, void* info) {
    /*
    if (id == "transformChanged") {
        node->setPosition(transform->getGlobalPositionWithoutRotation());

        if (targetToLookSet) {
            targetToLookSet = false;
            lookAt(targetToLook);
        }
    }
    */
    if (id == "posChanged") {
        node->setPosition(transform->getGlobalPositionWithoutRotation());

        // hacer que la camara mire al lugar correcto si se indica desde el .lua
        // que funcion con targetToLook
        // se hace de esta manera porque no se cambia su posicion hasta que llega el mensaje
        if (targetToLookSet) {
            targetToLookSet = false;
            lookAt(targetToLook);
        }
    }
}

void CameraComponent::free() {
    if (node != nullptr) {
        delete node;
        node = nullptr;
    }
    if (viewport != nullptr) {
        delete viewport;
        viewport = nullptr;
    }
}

void CameraComponent::lookAt(const Vector3 targetToLook) {
    this->targetToLook = targetToLook;
    camera->lookAt(targetToLook);
}

void CameraComponent::setDirection(const Vector3 dir) {
    direction = dir;
    camera->setDirection(dir);
}

void CameraComponent::setNearClipDistance(const float dist) {
    nearPlane = camera->getNearClipDistance();
    camera->setNearClipDistance(dist);
}

void CameraComponent::setFarClipDistance(const float dist) {
    farPlane = camera->getFarClipDistance();
    camera->setFarClipDistance(dist);
}

void CameraComponent::setFOVYRadians(const float radians) {
    nearPlane = camera->getNearClipDistance();
    camera->setFOVYRadians(radians);
}

void CameraComponent::setFOVYDegrees(const float degrees) {
    nearPlane = camera->getNearClipDistance();
    camera->setFOVYDegrees(degrees);
}

void CameraComponent::setDimensions(const Vector4 dimensions) {
    this->dimensions = dimensions;
    viewport->setDimensions(dimensions);
}

int CameraComponent::getWidthInPixels() const { return viewport->getWidthInPixels(); }

int CameraComponent::getHeightInPixels() const { return viewport->getHeightInPixels(); }

void CameraComponent::setBackground(const Vector3 color) {
    this->color = color;
    viewport->setBackground(Vector4(color, 1.0f));
}

void CameraComponent::setZOrder(const int zOrder) {
    GraphicsManager* graphicsManager = GraphicsManager::instance();
    graphicsManager->removeZOrder(this->zOrder);
    //graphicsManager->removeCameraCompByZOrder(this->zOrder, false);
    this->zOrder = graphicsManager->askForZOrder(zOrder);
    if (this->zOrder != -1) {
        if (zOrder != this->zOrder) {
            std::string message = "CameraComponent: El zOrder que se ha pedido no esta disponible. Se usa zOrder " +
                std::to_string(this->zOrder);
            logWarn(message.c_str());
        }
        //graphicsManager->removeCameraCompByZOrder(this->zOrder, true);
        viewport->setZOrder(this->zOrder);
        //graphicsManager->saveCameraComp(this);
    }
    else {
        logError("CameraComponent: No queda nigun zOrder disponible");
    }
}

int CameraComponent::getZOrder() const { return this->zOrder; }
}