#pragma once
#include "Button.h"
#include "Utilities/Vector2.h"

typedef void* ImTextureID;

namespace Tapioca {
/*
* @brief Clase que representa un botón con imagen en la interfaz grafica con la que el usuario puede interactuar para realizar cierta accion
*/
class TAPIOCA_API ImageTextButton : public Button {
private:
    bool wantText;			          // Indica si se quiere mostrar el texto en el boton
    std::string imagePathNormal;      // Ruta de la imagen que se muestra en el boton en estado normal
    std::string imagePathHover;       // Ruta de la imagen que se muestra en el boton en estado hover
    std::string imagePathActive;      // Ruta de la imagen que se muestra en el boton en estado active
    ImTextureID textureIdNormal;      // Identificador de la textura de la imagen que se muestra en el boton en estado normal
    ImTextureID textureIdHover;       // Identificador de la textura de la imagen que se muestra en el boton en estado hover
    ImTextureID textureIdActive;      // Identificador de la textura de la imagen que se muestra en el boton en estado active
    Vector2 uv0;                // Coordenadas de la esquina superior izquierda de la imagen
    Vector2 uv1;                // Coordenadas de la esquina inferior derecha de la imagen
    Vector4 imageBgColor;       // Color de fondo de la imagen
    Vector4 imageTint;          // Tinte de la imagen

public:
    COMPONENT_ID("ImageTextButton")

    /*
    * @brief Constructor por defecto
    */
    ImageTextButton();

    /*
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * @param variables unordered_map con los parametros iniciales
    */
    bool initComponent(const CompMap& variables) override;

    /*
    * @brief Asigna el transform del objeto al boton, inicializa la funcion que se ejecuta cuando se hace click en el boton y la fuente del texto
    */
    void start() override;

    /*
    * @brief Metodo que se usa para renderizar el boton
    */
    void render() const override;

    /*
    * @brief Actualiza las texturas de las imagenes del boton en los diferentes estados
    */
    void updateTextures();

    /*
    * @brief Actualiza la textura de la imagen del boton en estado normal
    */
    void updateTextureNormal();

    /*
    * @brief Actualiza la textura de la imagen del boton en estado hover
    */
    void updateTextureHover();

    /*
    * @brief Actualiza la textura de la imagen del boton en estado active
    */
    void updateTextureActive();

    /*
    * @brief Establece la ruta de la imagen que se muestra en el boton en estado normal
    * @param imagePath Ruta de la imagen en estado normal
    */
    inline void setImagePathNormal(const std::string& imagePath) {
        if (imagePath != this->imagePathNormal) {
            this->imagePathNormal = imagePath;
            updateTextureNormal();
        }
    }

    /*
    * @brief Establece la ruta de la imagen que se muestra en el boton en estado hover
    * @param imagePath Ruta de la imagen en estado hover
    */
    inline void setImagePathHover(const std::string& imagePath) {
        if (imagePath != this->imagePathHover) {
            this->imagePathHover = imagePath;
            updateTextureHover();
        }
    }

    /*
    * @brief Establece la ruta de la imagen que se muestra en el boton en estado active
    * @param imagePath Ruta de la imagen en estado active
    */
    inline void setImagePathActive(const std::string& imagePath) {
        if (imagePath != this->imagePathNormal) {
            this->imagePathActive = imagePath;
            updateTextureActive();
        }
    }

    /*
    * @brief Establece el identificador de la textura de la imagen que se muestra en el boton en estado normal
    * @param textureIdNormal Identificador de la textura de la imagen en estado normal
    */
    inline void setTextureIdNormal(ImTextureID textureIdNormal) { this->textureIdNormal = textureIdNormal; }

    /*
    * @brief Establece el identificador de la textura de la imagen que se muestra en el boton en estado hover
    * @param textureIdHover Identificador de la textura de la imagen en estado hover
    */
    inline void setTextureIdHover(ImTextureID textureIdHover) { this->textureIdHover = textureIdHover; }

    /*
    * @brief Establece el identificador de la textura de la imagen que se muestra en el boton en estado active
    * @param textureIdActive Identificador de la textura de la imagen en estado active
    */
    inline void setTextureIdActive(ImTextureID textureIdActive) { this->textureIdActive = textureIdActive; }

    /*
    * @brief Establece las coordenadas de la esquina superior izquierda de la imagen
    * @param uv0 Coordenadas de la esquina superior izquierda
    */
    inline void setUV0(const Vector2& uv0) { this->uv0 = uv0; }

    /*
    * @brief Establece las coordenadas de la esquina inferior derecha de la imagen
    * @param uv1 Coordenadas de la esquina inferior derecha
    */
    inline void setUV1(const Vector2& uv1) { this->uv1 = uv1; }

    /*
    * @brief Establece el color de fondo de la imagen
    * @param imageBgColor Color de fondo de la imagen
    */
    inline void setImageBgColor(const Vector4& imageBgColor) { this->imageBgColor = imageBgColor; }

    /*
    * @brief Establece el tinte de la imagen
    * @param imageTint Tinte de la imagen
    */
    inline void setImageTint(const Vector4& imageTint) { this->imageTint = imageTint; }

    /*
    * @brief Devuelve la ruta de la imagen que se muestra en el boton en estado normal
    * @return Ruta de la imagen en estado normal
    */
    inline std::string getImagePathNormal() const { return imagePathNormal; }

    /*
    * @brief Devuelve la ruta de la imagen que se muestra en el boton en estado hover
    * @return Ruta de la imagen en estado hover
    */
    inline std::string getImagePathHover() const { return imagePathHover; }

    /*
    * @brief Devuelve la ruta de la imagen que se muestra en el boton en estado active
    * @return Ruta de la imagen en estado active
    */
    inline std::string getImagePathActive() const { return imagePathActive; }

    /*
    * @brief Devuelve el identificador de la textura de la imagen que se muestra en el boton en estado normal
    * @return Identificador de la textura de la imagen en estado normal
    */
    inline ImTextureID getTextureIdNormal() const { return textureIdNormal; }

    /*
    * @brief Devuelve el identificador de la textura de la imagen que se muestra en el boton en estado hover
    * @return Identificador de la textura de la imagen en estado hover
    */
    inline ImTextureID getTextureIdHover() const { return textureIdHover; }

    /*
    * @brief Devuelve el identificador de la textura de la imagen que se muestra en el boton en estado active
    * @return Identificador de la textura de la imagen en estado active
    */
    inline ImTextureID getTextureIdActive() const { return textureIdActive; }

    /*
    * @brief Devuelve las coordenadas de la esquina superior izquierda de la imagen
    * @return Coordenadas de la esquina superior izquierda
    */
    inline Vector2 getUV0() const { return uv0; }

    /*
    * @brief Devuelve las coordenadas de la esquina inferior derecha de la imagen
    * @return Coordenadas de la esquina inferior derecha
    */
    inline Vector2 getUV1() const { return uv1; }

    /*
    * @brief Devuelve el color de fondo de la imagen
    * @return Color de fondo de la imagen
    */
    inline Vector4 getImageBgColor() const { return imageBgColor; }

    /*
    * @brief Devuelve el tinte de la imagen
    * @return Tinte de la imagen
    */
    inline Vector4 getImageTint() const { return imageTint; }
};
}