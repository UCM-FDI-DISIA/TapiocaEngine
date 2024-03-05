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
    Viewport(Ogre::RenderWindow* renderWindow, Camera* camera, int zOrder);

    void setDimensions(Vector4 dimensions);

    // de arriba a abajo (0-1)
    void setTop(float top);

    // de izquierda a derecha (0-1)
    void setLeft(float left);

    // (0-1)
    void setWidth(float width);

    // (0-1)
    void setHeight(float height);

    int getWidthInPixels();

    int getHeightInPixels();

    void setBackground(Vector3 color);
};
}