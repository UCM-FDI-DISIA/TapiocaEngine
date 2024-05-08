#include "LightDirComp.h"
#include "GraphicsManager.h"
#include "Utilities/RenderNode.h"
#include "Utilities/LightDirectional.h"
#include "checkML.h"

namespace Tapioca {
LightDirComp::LightDirComp()
    : node(nullptr), light(nullptr), direction(INITIAL_DIR), shadows(false), color(1.0f, 1.0f, 1.0f, 1.0f),
      powerScale(1.0f) { }

LightDirComp::~LightDirComp() {
    if (node != nullptr) delete node;
    node = nullptr;
    light = nullptr;
}

bool LightDirComp::initComponent(const CompMap& variables) {
    Vector3 directionAux;
    bool directionSet = setValueFromMap(directionAux.x, "directionX", variables) &&
                        setValueFromMap(directionAux.y, "directionY", variables) &&
                        setValueFromMap(directionAux.z, "directionZ", variables);
    directionSet = directionSet && directionAux != direction;
    if (!directionSet) {
        logInfo("LightDirComp: Apunta hacia (0,0,-1) global.");
    }
    else direction = directionAux;

    if (!setValueFromMap(shadows, "mainLight", variables)) {
        logInfo("LightDirComp: No produce sombras.");
    }

    Vector4 colorAux;
    bool colorSet = setValueFromMap(colorAux.x, "colorR", variables) &&
                    setValueFromMap(colorAux.y, "colorG", variables) && 
                    setValueFromMap(colorAux.z, "colorB", variables) &&
                    setValueFromMap(colorAux.w, "colorA", variables);
    colorSet = colorSet && colorAux != color;
    if (!colorSet) {
        logInfo("LightDirComp: Luz blanca.");
    }
    else color = colorAux;

    float powerAux;
    if (!setValueFromMap(powerAux, "powerScale", variables) || powerAux == powerScale) {
        logInfo("LightDirComp: La potencia de la luz por defecto es 1.0f.");
    }
    else powerScale = powerAux;

    return true;
}

void LightDirComp::start() {
    GraphicsManager* g = GraphicsManager::instance();
    node = g->createNode();
    light = g->createLightDirectional(node, direction, color);

    if (shadows) produceShadows(true);
    if (powerScale != 1.0f) setPowerScale(powerScale);
}

void LightDirComp::setColor(const Vector4 color) {
    this->color = color;
    light->setColor(color);
}

void LightDirComp::setPowerScale(const float power) {
    this->powerScale = power;
    light->setPowerScale(power);
}

void LightDirComp::setDirection(const Vector3 direction) {
    this->direction = direction;
    light->setDirection(direction);
}

void LightDirComp::produceShadows(bool enable) {
    this->shadows = enable;
    light->produceShadows(enable);
}
}
