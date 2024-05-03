#include "LightSpotComp.h"
#include "Utilities/RenderNode.h"
#include "Components/Transform.h"
#include "GraphicsManager.h"
#include "Utilities/LightSpotlight.h"
#include "Structure/GameObject.h"

namespace Tapioca {
LightSpotComp::LightSpotComp()
    : node(nullptr), transform(nullptr), light(nullptr), color(1.0f, 1.0f, 1.0f, 1.0f), powerScale(1.0f),
      attenuationFactor(), attenuationSet(false), direction(), nearClipDist(0.0f), falloff(1.0f), innerAngle(),
      innerAngleSet(false), outerAngle(), outerAngleSet(false) { }

LightSpotComp::~LightSpotComp() {
    if (node != nullptr) delete node;
    node = nullptr;
    transform = nullptr;
    light = nullptr;
}

bool LightSpotComp::initComponent(const CompMap& variables) {
    bool directionSet = setValueFromMap(direction.x, "directionX", variables) &&
        setValueFromMap(direction.y, "directionY", variables) && setValueFromMap(direction.z, "directionZ", variables);
    if (!directionSet) {
        logError("LightSpotComp: Es necesario indicar una direccion a la que apunta.");
        return false;
    }

    Vector4 colorAux;
    bool colorSet = setValueFromMap(colorAux.x, "colorR", variables) &&
        setValueFromMap(colorAux.y, "colorG", variables) && setValueFromMap(colorAux.z, "colorB", variables) &&
        setValueFromMap(colorAux.w, "colorA", variables);
    colorSet = colorSet && colorAux != color;
    if (!colorSet) {
        logInfo("LightSpotComp: Luz blanca.");
    }
    else
        color = colorAux;

    float powerAux;
    if (!setValueFromMap(powerAux, "powerScale", variables) || powerAux == powerScale) {
        logInfo("LightSpotComp: La potencia de la luz por defecto es 1.0f.");
    }
    else
        powerScale = powerAux;

    attenuationSet = setValueFromMap(attenuationFactor, "attenuationFactor", variables);
    if (!attenuationSet) {
        logInfo("LightSpotComp: La atenuacion de la luz es la de por defecto.");
    }

    float nearClipDistAux;
    if (!setValueFromMap(nearClipDistAux, "nearClipDistance", variables) || nearClipDistAux == nearClipDist) {
        logInfo("LightSpotComp: El punto desde el que se comienza a emitir luz es el origen.");
    }
    else
        nearClipDist = nearClipDistAux;

    float falloffAux;
    if (!setValueFromMap(falloffAux, "falloff", variables) || falloffAux == falloff) {
        logInfo(
            "LightSpotComp: La caida de la intensidad de la luz desde la zona interior hasta la exterior es lineal.");
    }
    else
        falloff = falloffAux;

    innerAngleSet = setValueFromMap(innerAngle, "innerAngle", variables);
    if (!innerAngleSet) {
        logInfo("LightSpotComp: El tam. de la zona interior es el de por defecto.");
    }

    outerAngleSet = setValueFromMap(outerAngle, "outerAngle", variables);
    if (!outerAngleSet) {
        logInfo("LightSpotComp: El tam. de la zona exterior es el de por defecto.");
    }

    return true;
}

void LightSpotComp::awake() {
    transform = object->getComponent<Transform>();
    GraphicsManager* graphicsManager = GraphicsManager::instance();
    node = graphicsManager->createNode();
    light = graphicsManager->createLightSpotlight(node, direction, color);

    if (powerScale != 1.0f) setPowerScale(powerScale);
    if (attenuationSet) setAttenuation(attenuationFactor);
    if (nearClipDist != 0.0f) setNearClipDistance(nearClipDist);
    if (falloff != 1.0f) setFalloff(falloff);
    if (innerAngleSet) setInnerAngle(innerAngle);
    if (outerAngleSet) setOuterAngle(outerAngle);
}

void LightSpotComp::handleEvent(std::string const& id, void* info) {
    if (id == "posChanged") node->setPosition(transform->getGlobalPosition());
}

void LightSpotComp::setColor(const Vector4 color) {
    this->color = color;
    light->setColor(color);
}

void LightSpotComp::setPowerScale(const float power) {
    this->powerScale = power;
    light->setPowerScale(power);
}

void LightSpotComp::setAttenuation(const float attenuationFactor) {
    this->attenuationFactor = attenuationFactor;
    float maximumRange = 100.0f;
    float constant = 1.0f;
    float linear = 0.045f * attenuationFactor;
    float quadratic = 0.0075f * attenuationFactor * attenuationFactor;

    light->setAttenuation(maximumRange, constant, linear, quadratic);
}

void LightSpotComp::setDirection(const Vector3 direction) {
    this->direction = direction;
    light->setDirection(direction);
}

void LightSpotComp::setNearClipDistance(float distance) {
    this->nearClipDist = distance;
    light->setNearClipDistance(distance);
}

void LightSpotComp::setInnerAngle(float degrees) {
    this->innerAngle = degrees;
    light->setInnerAngle(degrees);
}

void LightSpotComp::setOuterAngle(float degrees) {
    this->outerAngle = degrees;
    light->setOuterAngle(degrees);
}

void LightSpotComp::setFalloff(float falloff) {
    this->falloff = falloff;
    light->setFalloff(falloff);
}

void LightSpotComp::setVisible(const bool enable) { light->setVisible(enable); }

bool LightSpotComp::isVisible() const { return light->isVisible(); }
}
