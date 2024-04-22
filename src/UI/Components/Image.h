#pragma once
#include "BaseWidget.h"
#include "Structure/Component.h"
#include <string>

typedef void* ImTextureID;

namespace Tapioca {
/*
* @brief Clase que representa una imagen en la interfaz de usuario
*/
class TAPIOCA_API Image : public BaseWidget, public Component {
private:
    std::string imagePath;   // Ruta de la imagen
    ImTextureID textureId;   // Identificador de la textura de la imagen

public:
    COMPONENT_ID("Image")

    /*
    * @brief Constructor por defecto
    */
    Image();

    /*
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * @param Variables unordered_map con los parametros iniciales
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
    * @brief Establece la ruta de la imagen
    * @param imagePath Ruta de la imagen
    */
    inline void setImagePath(const std::string& imagePath) { this->imagePath = imagePath; }

    /*
    * @brief Establece el identificador de la textura de la imagen
    * @param textureId Identificador de la textura de la imagen
    */
    inline void setTextureId(ImTextureID textureId) { this->textureId = textureId; }

    /*
    * @brief Devuelve la ruta de la imagen
    * @return Ruta de la imagen
    */
    inline std::string getImagePath() const { return imagePath; }

    /*
    * @brief Devuelve el identificador de la textura de la imagen
    * @return Identificador de la textura de la imagen
    */
    inline ImTextureID getTextureId() const { return textureId; }

    /*
    * @brief Actualiza la textura de la imagen
    */
    void updateTexture();
};
}
