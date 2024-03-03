#pragma once

namespace Tapioca {
class Camera;
class Vector4;
class Vector3;
}

namespace Ogre {
class RenderWindow;
class Viewport;
}

namespace Tapioca {
class Viewport {
private:
    Ogre::Viewport* viewport;

public:
    /*
    Se utiliza para indicar al render target (la ventana de Ogre), que trozo de la ventana
    usar para renderizar lo que muestra la camara
    */
    Viewport(Ogre::RenderWindow* renderWindow, Tapioca::Camera* camera, int zOrder);

    void setDimensions(Tapioca::Vector4 dimensions);

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

    void setBackground(Tapioca::Vector3 color);
};
}