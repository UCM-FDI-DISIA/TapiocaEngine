// HAY QUE HACER QUE:
// Herede de Component
// Tenga un constructor vacio por defecto
// Funcione desde un archivo.lua

#pragma once
#include "BaseWidget.h"
#include "Core.h"

namespace Ogre {
class GLTexture;
}

namespace Tapioca {
class TAPIOCA_API Image : public BaseWidget {
private:
    //informacion de a textura que vamos a cargar
    std::string sourcefile;
    Ogre::GLTexture* myTexture;
    uint32_t textureID = 0;
    int textHeitgh=0;
    int textWidth=0;
    //informacion de nuestra imagen
    //ancho y alto
    Tapioca::Vector2 size;
    //x e y
    Tapioca::Vector2 position;

public:
    /*
    * @brief Constructora de imagen para interfaz que toma com parametros
    * @param file la ruta al archivo de la imagen qeu qeuremos qeu se vea
    * @param widthandheigth un vector con las dimensiones con las que deseamos mostrar la imagen
    * @param xandy un vector con la posicion en pantalla de la imagen 
    */
    Image(std::string file, Tapioca::Vector2 widthandheigth, Tapioca::Vector2 xandy);
    ~Image() { }

    uint32_t getID() { return textureID; }
    Tapioca::Vector2 getPosition() { return position; }
    Tapioca::Vector2 getSize(){return size;}
};
}
