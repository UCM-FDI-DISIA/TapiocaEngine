#pragma once
#include "imgui.h"
#include "Core.h"
#include "OgreGLTexture.h"

namespace Tapioca {
class TAPIOCA_API ImageUI {
private:
    //informacion de a textura que vamos a cargar
    std::string sourcefile;
    Ogre::GLTexture* myTexture;
    uint32_t textureID = 0;
    int textHeitgh=0;
    int textWidth=0;
    //informacion de nuestra imagen
    ImGuiWindowFlags myflags;
    //ancho y alto
    Tapioca::Vector2 size;
    //x e y
    Tapioca::Vector2 position;

    bool* canClose;

public:
    /*
    * @brief Constructora de imagen para interfaz que toma com parametros
    * @param file la ruta al archivo de la imagen qeu qeuremos qeu se vea
    * @param widthandheigth un vector con las dimensiones con las que deseamos mostrar la imagen
    * @param xandy un vector con la posicion en pantalla de la imagen 
    */
    ImageUI(std::string file, Tapioca::Vector2 widthandheigth, Tapioca::Vector2 xandy,
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings);
    ~ImageUI();

    uint32_t getID() { return textureID; }
    ImGuiWindowFlags getFlags() { return myflags; }
    bool*  getCanCloseWindow() { return canClose; }
    Tapioca::Vector2 getScaledDimnsions();
    Tapioca::Vector2 getPosition() { return position; }
    Tapioca::Vector2 getSize(){return size;}
};
}
