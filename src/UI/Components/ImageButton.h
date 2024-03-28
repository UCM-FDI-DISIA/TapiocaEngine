#pragma once
#include "Button.h"
#include <string>

typedef void* ImTextureID;

namespace Tapioca {
/*
* @brief Clase que representa un botón con imagen en la interfaz grafica con la que el usuario puede interactuar para realizar cierta accion
*/
class TAPIOCA_API ImageButton : public Button {
private:
    std::string imagePath;      // Ruta de la imagen que se muestra en el boton
    ImTextureID textureId;      // Identificador de la textura de la imagen que se muestra en el boton
    Vector2 uv0;                // Coordenadas de la esquina superior izquierda de la imagen
    Vector2 uv1;                // Coordenadas de la esquina inferior derecha de la imagen
    Vector4 imageBgColor;       // Color de fondo de la imagen
    Vector4 imageTint;          // Tinte de la imagen

public:
    COMPONENT_ID("ImageButton")

    /*
    * @brief Constructor por defecto
    */
    ImageButton();

    /*
    * @brief Destructor por defecto
    */
    ~ImageButton() { }

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
    * @brief Actualiza la textura de la imagen
    */
    void updateTexture();

    /*
    * @brief Establece la ruta de la imagen que se muestra en el boton
    * @param imagePath Ruta de la imagen
    */
    inline void setImagePath(const std::string& imagePath) {
        if (imagePath != this->imagePath) {
            this->imagePath = imagePath;
            updateTexture();
        }
    }

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
    * @brief Devuelve la ruta de la imagen que se muestra en el boton
    * @return Ruta de la imagen
    */
    inline std::string getImagePath() const { return imagePath; }

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