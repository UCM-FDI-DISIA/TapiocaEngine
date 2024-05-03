#pragma once
#include "defs.h"

namespace Ogre {
class RenderWindow;
class Viewport;
}

namespace Tapioca {
class Camera;
class Vector4;
class Vector3;

/**
* @brief Wrapper de la clase Mesh de Ogre
* Se utiliza para indicar al render target (la ventana de Ogre), que trozo de la ventana
* usar para renderizar lo que muestra la camara
*/
class TAPIOCA_API Viewport {
private:
    Ogre::Viewport* viewport;           // Viewport de Ogre
    Ogre::RenderWindow* renderWindow;   // RenderWindow de Ogre
    int zOrder;                         // Orden de renderizado
    Camera* camera;                     // Camara a la que esta asociado el viewport

public:
    /**
    * @brief Constructor de la clase Viewport
    * @param renderWindow Ventana de Ogre
    * @param camera Camara a la que esta asociado el viewport
    * @param zOrder Orden de renderizado
    */
    Viewport(Ogre::RenderWindow* const renderWindow, Camera* const camera, const int zOrder);

    /**
    * @brief Elimina el viewport del render window
    */
    virtual ~Viewport();

    /**
    * @brief Cambia las dimensiones del viewport
    * @param dimensions Dimensiones que se quieren asignar al plano
    */
    void setDimensions(const Vector4 dimensions);

    /**
    * @brief Cambia las dimension top del viewport. Va de arriba a abajo (0-1)
    * @param top Valor que se quiere asignar 
    */
    void setTop(const float top);

    /**
    * @brief Cambia las dimension left del viewport. Va de izquierda a derecha (0-1)
    * @param left Valor que se quiere asignar 
    */
    void setLeft(const float left);

    /**
    * @brief Cambia las dimension width del viewport (0-1)
    * @param width Valor que se quiere asignar 
    */
    void setWidth(const float width);

    /**
    * @brief Cambia las dimension height del viewport (0-1)
    * @param height Valor que se quiere asignar 
    */
    void setHeight(const float height);

    /**
    * @brief Devuelve la anchura del viewport en pixeles
    * @return height Valor que se quiere asignar 
    */
    int getWidthInPixels();

    /**
    * @brief Devuelve la altura del viewport en pixeles
    * @return height Valor que se quiere asignar 
    */
    int getHeightInPixels();

    /**
    * @brief Cambia el color de fondo
    * @param color Color al que se quiere cambiar
    */
    void setBackground(const Vector4 color);

    /**
    * @brief Establece el orden de renderizado
    * @param zOrder Orden de renderizado
    */
    void setZOrder(const int zOrder);
};
}