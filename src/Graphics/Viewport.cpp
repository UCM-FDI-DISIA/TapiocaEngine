#include "Viewport.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <Ogre.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include "Camera.h"
#include "Utilities/Vector4.h"

namespace Tapioca {

Viewport::Viewport(Ogre::RenderWindow* const renderWindow, Camera* const camera, const int zOrder)
    : renderWindow(renderWindow), zOrder(zOrder), camera(camera) {
    // el render target hace referencia a dodne se renderiza, en este caso, la ventana de Ogre
    viewport = renderWindow->addViewport(camera->getCamera(), zOrder);
}

Viewport::~Viewport() { renderWindow->removeViewport(zOrder); }

void Viewport::setDimensions(const Vector4 dimensions) {
    viewport->setDimensions(dimensions.x, dimensions.y, dimensions.z, dimensions.w);
}

// de arriba a abajo (0-1)
void Viewport::setTop(const float top) {
    viewport->setDimensions(viewport->getLeft(), top, viewport->getWidth(), viewport->getHeight());
}

// de izquierda a derecha (0-1)
void Viewport::setLeft(const float left) {
    viewport->setDimensions(left, viewport->getTop(), viewport->getWidth(), viewport->getHeight());
}

// (0-1)
void Viewport::setWidth(const float width) {
    viewport->setDimensions(viewport->getLeft(), viewport->getTop(), width, viewport->getHeight());
}

// (0-1)
void Viewport::setHeight(const float height) {
    viewport->setDimensions(viewport->getLeft(), viewport->getTop(), viewport->getWidth(), height);
}

int Viewport::getWidthInPixels() { return viewport->getActualWidth(); }

int Viewport::getHeightInPixels() { return viewport->getActualHeight(); }

void Viewport::setBackground(const Vector4 color) {
    viewport->setBackgroundColour(Ogre::ColourValue(color.x, color.y, color.z, color.w));
}

void Viewport::setZOrder(int zOrder) {
    renderWindow->removeViewport(this->zOrder);
    viewport = renderWindow->addViewport(camera->getCamera(), zOrder);
    this->zOrder = zOrder;
}
}
