#pragma once
#include "BaseWidget.h"
#include "Structure/Component.h"
#include "Utilities/Vector2.h"
#include <string>
#include <imgui.h>

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
/*
* @brief Clase que representa un texto en la interfaz grafica
*/
class TAPIOCA_API Text : public BaseWidget, public Component {
private:
    std::string text;           // Texto
    float textSize;             // Tamano del texto
    std::string textFontName;   // Nombre de la fuente del texto
    ImFont* textFont;           // Fuente del texto
    ImVec4 textColor;           // Color del texto

public:
    COMPONENT_ID("Text")

    /*
    * @brief Constructor por defecto
    */
    Text();

    /*
    * @brief Destructor por defecto
    */
    ~Text() { }

    /*
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * @param variables unordered_map con los parametros iniciales
    */
    bool initComponent(const CompMap& variables) override;

    /*
    * @brief Asigna el transform del objeto al texto e inicializa la fuente del texto
    */
    void awake() override;

    /*
    * @brief Metodo que se usa para renderizar el texto
    */
    void render() const override;

    /*
    * @brief Establece el texto
    * @param text Texto
    */
    inline void setText(const std::string& text) { this->text = text; }

    /*
    * @brief Establece el tamano de la fuente del texto
    * @param textSize Tamano de la fuente del texto
    */
    inline void setTextSize(float textSize) { this->textSize = textSize; }

    /*
    * @brief Establece el nombre de la fuente del texto
    * @param textFontName Nombre de la fuente del texto
    */
    inline void setTextFontName(std::string textFontName) { this->textFontName = textFontName; }

    /*
    * @brief Establece la fuente del texto
    * @param textFont Fuente del texto
    */
    inline void setTextFont(ImFont* textFont) { this->textFont = textFont; }

    /*
    * @brief Establece el color del texto
    * @param textColor Color del texto
    */
    inline void setTextColor(const ImVec4& textColor) { this->textColor = textColor; }

    /*
    * @brief Devuelve el texto
    * @return Texto
    */
    inline std::string getText() const { return text; }

    /*
    * @brief Devuelve el tamano de la fuente del texto
    * @return Tamano de la fuente del texto
    */
    inline float getTextSize() const { return textSize; }

    /*
    * @brief Devuelve el nombre de la fuente del texto
    * @return Nombre de la fuente del texto
    */
    inline std::string getTextFontName() const { return textFontName; }

    /*
    * @brief Devuelve la fuente del texto
    * @return Fuente del texto
    */
    inline ImFont* getTextFont() const { return textFont; }

    /*
    * @brief Devuelve el color del texto
    * @return Color del texto
    */
    inline ImVec4 getTextColor() const { return textColor; }
};
}