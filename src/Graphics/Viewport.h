#pragma once
#include "Utilities/defs.h"

namespace Ogre {
class RenderWindow;
class Viewport;
}

namespace Tapioca {
    class Camera;
    class Vector4;
    class Vector3;

class TAPIOCA_API Viewport {
private:
    Ogre::Viewport* viewport;

public:
    /*
    Se utiliza para indicar al render target (la ventana de Ogre), que trozo de la ventana
    usar para renderizar lo que muestra la camara
    */
    Viewport(Ogre::RenderWindow* const renderWindow, Camera* const camera, const int zOrder);

    void setDimensions(const Vector4 dimensions);

    // de arriba a abajo (0-1)
    void setTop(const float top);

    // de izquierda a derecha (0-1)
    void setLeft(const float left);

    // (0-1)
    void setWidth(const float width);

    // (0-1)
    void setHeight(const float height);

    int getWidthInPixels();

    int getHeightInPixels();

    void setBackground(const Vector3 color);
};
}