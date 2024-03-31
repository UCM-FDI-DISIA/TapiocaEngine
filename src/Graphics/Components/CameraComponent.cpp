#include "CameraComponent.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "../GraphicsManager.h"
#include "../RenderNode.h"
#include "../Camera.h"
#include "../Viewport.h"

namespace Tapioca {
CameraComponent::CameraComponent()
    : transform(nullptr), node(nullptr), camera(nullptr), viewport(nullptr), color(-1.0f, -1.0f, -1.0f), zOrder(),
      dimensions(0.0f, 0.0f, 1.0f, 1.0f), targetToLook(0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, -1.0f), nearPlane(-1.0f),
      farPlane(-1.0f) { }

CameraComponent::~CameraComponent() {
    delete node;
    delete viewport;
}

bool CameraComponent::initComponent(const CompMap& variables) {
    // Camera
    bool directionSet = setValueFromMap(direction.x, "directionX", variables) &&
                        setValueFromMap(direction.y, "directionY", variables) && 
                        setValueFromMap(direction.z, "directionZ", variables);
    if (!directionSet) {
        bool targetToLookSet = setValueFromMap(targetToLook.x, "targetToLookX", variables) &&
                               setValueFromMap(targetToLook.y, "targetToLookY", variables) &&
                               setValueFromMap(targetToLook.z, "targetToLookZ", variables);
        if (!targetToLookSet) {
#ifdef _DEBUG
            std::cout << "CameraComponent: la camara apunta hacia (0,0,-1) global.\n";
#endif
        }
    }

    if (!setValueFromMap(nearPlane, "nearPlane", variables)) {
#ifdef _DEBUG
        std::cout << "CameraComponent: la distancia por defecto al nearPlane es 1.0f.\n";
#endif
    }

    if (!setValueFromMap(nearPlane, "farPlane", variables)) {
#ifdef _DEBUG
        std::cout << "CameraComponent: la distancia por defecto al farPlane es 1000.0f.\n";
#endif
    }

    // Viewport
    if (!setValueFromMap(zOrder, "zOrder", variables)) {
#ifdef _DEBUG
        std::cerr << "Error: CameraComponent: no se pudo inicializar el zOrder.\n";
#endif
        return false;
    }

    bool colorSet = setValueFromMap(color.x, "bgColorR", variables) &&
        setValueFromMap(color.y, "bgColorG", variables) && setValueFromMap(color.z, "bgColorB", variables);
    if (!colorSet) {
#ifdef _DEBUG
        std::cout << "CameraComponent: el color predeterminado del fondo es negro.\n";
#endif
    }

    if (!setValueFromMap(dimensions.x, "left", variables)) {
#ifdef _DEBUG
        std::cout << "CameraComponent: la camara esta situada a la izquierda del todo.\n";
#endif
    }
    if (!setValueFromMap(dimensions.y, "top", variables)) {
#ifdef _DEBUG
        std::cout << "CameraComponent: la camara esta situada arriba del todo.\n";
#endif
    }
    if (!setValueFromMap(dimensions.z, "width", variables)) {
#ifdef _DEBUG
        std::cout << "CameraComponent: el ancho de la camara corresponde con el de la ventana.\n";
#endif
    }
    if (!setValueFromMap(dimensions.w, "height", variables)) {
#ifdef _DEBUG
        std::cout << "CameraComponent: la altura de la camara corresponde con la de la ventana.\n";
#endif
    }

    return true;
}

void CameraComponent::awake() {
    transform = getObject()->getComponent<Transform>();

    GraphicsManager* graphicsManager = GraphicsManager::instance();
    node = graphicsManager->createNode();
    camera = graphicsManager->createCamera(node, "Camera " + zOrder);
    viewport = graphicsManager->createViewport(camera, zOrder);

    if (direction != Vector3(0.0f, 0.0f, -1.0f)) {
        camera->setDirection(direction);
    }
    else {
        camera->setDirection(Vector3(0.0f, 0.0f, -1.0f));
    }
    
    //TODO: Poner targetToLook si se establece manualmente que hay que mirar a un punto siempre


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

void CameraComponent::handleEvent(std::string const& id, void* info) {
    if (id == "transformChanged") {
        node->setPosition(transform->getGlobalPosition());
        // Distinguir entre si esta mirando a un punto o esta mirando en una direccion
        //if (direction == Vector3(0.0f, 0.0f, 0.0f)) {
        //    // Mirar a un punto depende de la posicion de la camara
        //    // Por lo tanto, cada vez que se cambie la pos, hay que cambiarlo
        //    lookAt(targetToLook);
        //}
        /*node->setRotation(transform->getGlobalRotation());
        node->setScale(transform->getGlobalScale());*/
    }
}

void CameraComponent::lookAt(const Vector3 targetToLook) {
    // Indicar que se va a usar targetToLook y no direction
    direction = Vector3(0.0f, 0.0f, 0.0f);
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
    this->zOrder = zOrder;
    viewport->setZOrder(zOrder);
}
}