#include "LightRectlight.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "Utilities/Vector3.h"
#include "RenderNode.h"

namespace Tapioca {
// int* const --> no se puede cambiar el puntero, pero si su valor
// const int* --> el entero al que apunta no se puede cambiar
// const int* const --> no se puede cambiar ni el puntero ni el entero
LightRectlight::LightRectlight(Ogre::SceneManager* const sceneManager, RenderNode* const node, Vector4 const color,
                               const float width, const float height, Vector3 direction)
    : Light(sceneManager, node, color) {

    mLight->setType(Ogre::Light::LightTypes::LT_RECTLIGHT);
    mLight->setSourceSize(width, height);
    node->setDirection(direction);
}

void LightRectlight::setSize(float width, float height) { mLight->setSourceSize(width, height); }

void LightRectlight::setDirection(Vector3 direction) { node->setDirection(direction); }

void LightRectlight::setAttenuation(float maximumRange, float constant, float linear, float quadratic) {
    mLight->setAttenuation(maximumRange, constant, linear, quadratic);
}
}