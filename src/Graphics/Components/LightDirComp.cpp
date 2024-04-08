#include "LightDirComp.h"
#include "../RenderNode.h"
#include "../GraphicsManager.h"
#include "../LightDirectional.h"

namespace Tapioca {
LightDirComp::LightDirComp()
    : light(nullptr), node(nullptr), direction(INITIAL_DIR), shadows(false), color(1.0f, 1.0f, 1.0f, 1.0f),
      powerScale(1.0f) { }

LightDirComp::~LightDirComp() { delete node; }

bool LightDirComp::initComponent(const CompMap& variables) {
    Vector3 directionAux;
    bool directionSet = setValueFromMap(directionAux.x, "directionX", variables) &&
        setValueFromMap(directionAux.y, "directionY", variables) &&
        setValueFromMap(directionAux.z, "directionZ", variables);
    directionSet = directionSet && directionAux != direction;
    if (!directionSet) {
#ifdef _DEBUG
        std::cout << "LightDirComp: apunta hacia (0,0,-1) global.\n";
#endif
    }
    else {
        direction = directionAux;
    }

    if (!setValueFromMap(shadows, "mainLight", variables)) {
#ifdef _DEBUG
        std::cout << "LightDirComp: no produce sombras.\n";
#endif
    }

    Vector4 colorAux;
    bool colorSet = setValueFromMap(colorAux.x, "colorR", variables) &&
        setValueFromMap(colorAux.y, "colorG", variables) && setValueFromMap(colorAux.z, "colorB", variables) &&
        setValueFromMap(colorAux.w, "colorA", variables);
    colorSet = colorSet && colorAux != color;
    if (!colorSet) {
#ifdef _DEBUG
        std::cout << "LightDirComp: luz blanca.\n";
#endif
    }
    else {
        color = colorAux;
    }

    float powerAux;
    if (!setValueFromMap(powerAux, "powerScale", variables) || powerAux == powerScale) {
#ifdef _DEBUG
        std::cout << "LightDirComp: la potencia de la luz por defecto es 1.0f.\n";
#endif
    }
    else {
        powerScale = powerAux;
    }

    return true;
}

void LightDirComp::start() {
    GraphicsManager* g = GraphicsManager::instance();
    node = g->createNode();
    light = g->createLightDirectional(node, direction, color);

    if (shadows) {
        produceShadows(true);
    }
    if (powerScale != 1.0f) {
        setPowerScale(powerScale);
    }
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