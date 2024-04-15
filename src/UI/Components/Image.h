// HAY QUE HACER QUE:
// Herede de Component
// Tenga un constructor vacio por defecto
// Funcione desde un archivo.lua

#pragma once
#include "BaseWidget.h"
#include "Structure/Component.h"
#include "Core.h"

namespace Ogre {
class GLTexture;
}

namespace Tapioca {
class TAPIOCA_API Image : public BaseWidget, public Component {
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
    //nombre para la ventana de ImGui
    std::string name;

public:
    COMPONENT_ID("ImageUI")
    //@brief constructora vacia
    Image();
    //@brief destructora por defecto
    ~Image();
    /*
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * @param variables unordered_map con los parametros iniciales
    */
    virtual bool initComponent(const CompMap& variables) override;
    /*
    * @brief Asigna el transform del objeto al texto e inicializa la fuente del texto
    */
    virtual void start() override;
    /*
    * @brief Metodo que se usa para renderizar 
    */
    virtual void render() const override;
   
    /*
    * @brief devuelve el identificador de la textura en el buffer de OpenGl
    */
    uint32_t getID() { return textureID; }
   
};
}
