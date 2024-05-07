#include "LightPointComp.h"
#include "Utilities/RenderNode.h"
#include "Components/Transform.h"
#include "GraphicsManager.h"
#include "Utilities/LightPoint.h"
#include "Structure/GameObject.h"
#include "checkML.h"

namespace Tapioca {
LightPointComp::LightPointComp()
    : node(nullptr), transform(nullptr), light(nullptr), color(1.0f, 1.0f, 1.0f, 1.0f), powerScale(1.0f),
      attenuationFactor(), attenuationSet(false) { }

LightPointComp::~LightPointComp() {
    if (node != nullptr) delete node;
    node = nullptr;
    transform = nullptr;
    light = nullptr;
}

bool LightPointComp::initComponent(const CompMap& variables) {
    Vector4 colorAux;
    bool colorSet = setValueFromMap(colorAux.x, "colorR", variables) &&
        setValueFromMap(colorAux.y, "colorG", variables) && setValueFromMap(colorAux.z, "colorB", variables) &&
        setValueFromMap(colorAux.w, "colorA", variables);
    colorSet = colorSet && colorAux != color;
    if (!colorSet) {
        logInfo("LightPointComp: Luz blanca.");
    }
    else
        color = colorAux;

    float powerAux;
    if (!setValueFromMap(powerAux, "powerScale", variables) || powerAux == powerScale) {
        logInfo("LightPointComp: La potencia de la luz por defecto es 1.0f.");
    }
    else
        powerScale = powerAux;

    attenuationSet = setValueFromMap(attenuationFactor, "attenuationFactor", variables);
    if (!attenuationSet) {
        logInfo("LightPointComp: La atenuacion de la luz es la de por defecto.");
    }

    return true;
}

void LightPointComp::awake() {
    transform = object->getComponent<Transform>();
    GraphicsManager* graphicsManager = GraphicsManager::instance();
    node = graphicsManager->createNode();
    light = graphicsManager->createLightPoint(node, color);

    if (powerScale != 1.0f) setPowerScale(powerScale);
    if (attenuationSet) setAttenuation(attenuationFactor);
}

void LightPointComp::handleEvent(std::string const& id, void* info) {
    if (id == "posChanged") {
        node->setPosition(transform->getGlobalPosition());
    }
}

void LightPointComp::setColor(const Vector4 color) {
    this->color = color;
    light->setColor(color);
}

void LightPointComp::setPowerScale(const float power) {
    this->powerScale = power;
    light->setPowerScale(power);
}

void LightPointComp::setAttenuation(const float attenuationFactor) {
    this->attenuationFactor = attenuationFactor;
    float maximumRange = 100.0f;
    float constant = 1.0f;
    float linear = 0.045f * attenuationFactor;
    float quadratic = 0.0075f * attenuationFactor * attenuationFactor;

    light->setAttenuation(maximumRange, constant, linear, quadratic);
}

void LightPointComp::setVisible(const bool enable) { light->setVisible(enable); }

bool LightPointComp::isVisible() const { return light->isVisible(); }
}
