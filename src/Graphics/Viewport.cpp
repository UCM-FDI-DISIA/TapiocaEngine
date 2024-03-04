#include "Viewport.h"
#include <Ogre.h>
#include "Camera.h"
#include "Utilities/Vector4.h"

namespace Tapioca { 

Viewport::Viewport(Ogre::RenderWindow* renderWindow, Camera* camera, int zOrder) {
    // el render target hace referencia a dodne se renderiza, en este caso, la ventana de Ogre
    viewport = renderWindow->addViewport(camera->getCamera(), zOrder);
}

void Viewport::setDimensions(Vector4 dimensions) {
    viewport->setDimensions(dimensions.x, dimensions.y, dimensions.z, dimensions.w);
}

// de arriba a abajo (0-1)
void Viewport::setTop(float top) {
    viewport->setDimensions(viewport->getLeft(), top, viewport->getWidth(), viewport->getHeight());
}

// de izquierda a derecha (0-1)
void Viewport::setLeft(float left) {
    viewport->setDimensions(left, viewport->getTop(), viewport->getWidth(), viewport->getHeight());
}

// (0-1)
void Viewport::setWidth(float width) {
    viewport->setDimensions(viewport->getLeft(), viewport->getTop(), width, viewport->getHeight());
}

// (0-1)
void Viewport::setHeight(float height) {
    viewport->setDimensions(viewport->getLeft(), viewport->getTop(), viewport->getWidth(), height);
}

int Viewport::getWidthInPixels() { return viewport->getActualWidth(); }

int Viewport::getHeightInPixels() { return viewport->getActualHeight(); }

void Viewport::setBackground(Vector3 color) {
    viewport->setBackgroundColour(Ogre::ColourValue(color.x, color.y, color.z));
}
}
