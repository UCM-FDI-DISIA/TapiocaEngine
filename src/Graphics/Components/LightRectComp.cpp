#include "LightRectComp.h"
#include "Utilities/RenderNode.h"
#include "Components/Transform.h"
#include "GraphicsManager.h"
#include "Utilities/LightRectlight.h"
#include "Structure/GameObject.h"
#include "checkML.h"

namespace Tapioca {
LightRectComp::LightRectComp() : node(nullptr), transform(nullptr), light(nullptr), color(1.0f, 1.0f, 1.0f, 1.0f), 
    powerScale(1.0f), attenuationFactor(), attenuationSet(false), width(), height(), direction() { }

LightRectComp::~LightRectComp() {
    if (node != nullptr) delete node;
    node = nullptr;
    transform = nullptr;
    light = nullptr;
}
bool LightRectComp::initComponent(const CompMap& variables) {
    bool directionSet = setValueFromMap(direction.x, "directionX", variables) &&
                        setValueFromMap(direction.y, "directionY", variables) &&
                        setValueFromMap(direction.z, "directionZ", variables);
    if (!directionSet) {
        logError("LightRectComp: Es necesario indicar una direccion a la que apunta.");
        return false;
    }

    bool sizeSet = setValueFromMap(width, "width", variables) && 
                   setValueFromMap(height, "height", variables);
    if (!sizeSet) {
        logError("LightRectComp: Es necesario indicar un tamano para definir el rectangulo.");
        return false;
    }

    Vector4 colorAux;
    bool colorSet = setValueFromMap(colorAux.x, "colorR", variables) &&
                    setValueFromMap(colorAux.y, "colorG", variables) && 
                    setValueFromMap(colorAux.z, "colorB", variables) &&
                    setValueFromMap(colorAux.w, "colorA", variables);
    colorSet = colorSet && colorAux != color;
    if (!colorSet) {
        logInfo("LightRectComp: Luz blanca.");
    }
    else color = colorAux;

    float powerAux;
    if (!setValueFromMap(powerAux, "powerScale", variables) || powerAux == powerScale) {
        logInfo("LightRectComp: La potencia de la luz por defecto es 1.0f.");
    }
    else powerScale = powerAux;

    attenuationSet = setValueFromMap(attenuationFactor, "attenuationFactor", variables);
    if (!attenuationSet) {
        logInfo("LightRectComp: La atenuacion de la luz es la de por defecto.");
    }

    return true;
}

void LightRectComp::awake() {
    transform = object->getComponent<Transform>();
    GraphicsManager* graphicsManager = GraphicsManager::instance();
    node = graphicsManager->createNode();
    light = graphicsManager->createLightRectlight(node, direction, width, height, color);

    if (powerScale != 1.0f) setPowerScale(powerScale);
    if (attenuationSet) setAttenuation(attenuationFactor);
}

void LightRectComp::handleEvent(std::string const& id, void* info) {
    if (id == "posChanged") node->setPosition(transform->getGlobalPosition());
}

void LightRectComp::setColor(const Vector4 color) {
    this->color = color;
    light->setColor(color);
}

void LightRectComp::setPowerScale(const float power) {
    this->powerScale = power;
    light->setPowerScale(power);
}

void LightRectComp::setAttenuation(const float attenuationFactor) {
    this->attenuationFactor = attenuationFactor;
    float maximumRange = 100.0f;
    float constant = 1.0f;
    float linear = 0.045f * attenuationFactor;
    float quadratic = 0.0075f * attenuationFactor * attenuationFactor;

    light->setAttenuation(maximumRange, constant, linear, quadratic);
}

void LightRectComp::setDirection(const Vector3 direction) {
    this->direction = direction;
    light->setDirection(direction);
}

void LightRectComp::setWidth(const float width) {
    this->width = width;
    light->setSize(this->width, height);
}

void LightRectComp::setHeight(const float height) {
    this->height = height;
    light->setSize(width, this->height);
}

void LightRectComp::setSize(const float width, const float height) {
    this->width = width;
    this->height = height;
    light->setSize(width, height);
}

void LightRectComp::setVisible(const bool enable) { light->setVisible(enable); }

bool LightRectComp::isVisible() const { return light->isVisible(); }
}