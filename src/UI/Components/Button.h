#pragma once
#include "BaseWidget.h"
#include "Structure/Component.h"
#include <string>
#include <functional>
#include <unordered_map>
#include "Utilities/Vector2.h"
#include "Utilities/Vector4.h"

struct ImFont;

namespace Tapioca {
/*
* @brief Enumeracion que representa las funciones que puede realizar un boton
*/
enum ButtonFunction { BUTTON_NONE, BUTTON_INIT_GAME };

/*
* @brief Clase que representa un botón en la interfaz grafica con la que el usuario puede interactuar para realizar cierta accion
*/
class TAPIOCA_API Button : public BaseWidget, public Component {
private:
    std::string text;                // Texto que se muestra en el boton
    int onClickId;                   // Identificador de la funcion que se ejecuta cuando se hace click en el boton
    std::function<void()> onClick;   // Funcion que se ejecuta cuando se hace click en el boton
    Vector2 padding;                 // Tamano del padding del boton
    std::string textFontName;        // Nombre de la fuente del texto del boton
    float textSize;                  // Tamano de la fuente del texto del boton
    ImFont* textFont;                // Fuente del texto del boton incluyendo tamano
    Vector4 textColor;               // Color del texto del boton
    Vector4 normalColor;             // Color del boton para el estado "normal"
    Vector4 hoverColor;              // Color del boton para el estado "hover"
    Vector4 activeColor;             // Color del boton para el estado "active"

    /*
    * @brief Diccionario que contiene las funciones que se ejecutan cuando se hace click en el boton
    */
    std::unordered_map<int, std::function<void()>> buttonFunctions;

public:
    COMPONENT_ID("Button")

    /*
    * @brief Constructor por defecto
    */
    Button();

    /*
    * @brief Destructor por defecto
    */
    ~Button() { }

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
    * @brief Crea las funciones que se ejecutan cuando se hace click en el boton
    */
    void createButtonFunctions();

    /*
    * @brief Establece el texto del boton
    * @param text Texto del boton
    */
    inline void setText(const std::string& text) { this->text = text; }

    /*
    * @brief Establece el identificador de la funcion que se ejecuta cuando se hace click en el boton
    * @param onClickId Identificador de la funcion que se ejecuta cuando se hace click en el boton
    */
    inline void setOnClickId(int onClickId) {
        if (onClickId != this->onClickId) {
            this->onClickId = onClickId;
            updateOnClick();
        }
    }

    /*
    * @brief Establece la funcion que se ejecuta cuando se hace click en el boton
    * @param onClick Funcion que se ejecuta cuando se hace click en el boton
    */
    inline void setOnClick(std::function<void()> onClick) { this->onClick = onClick; }

    /*
    * @brief Actualiza la funcion que se ejecuta cuando se hace click en el boton
    */
    void updateOnClick();

    /*
    * @brief Establece el tamano del padding del boton
    * @param padding Tamano del padding del boton
    */
    inline void setPadding(const Vector2& padding) { this->padding = padding; }

    /*
    * @brief Establece el tamano del texto del boton
    */
    inline void setTextSize(float textSize) {
        if (textSize != this->textSize) {
            this->textSize = textSize;
            updateTextFont();
        }
    }

    /*
    * @brief Establece el tamano del texto del boton
    */
    inline void setTextFontName(std::string textFontName) {
        if (textFontName != this->textFontName) {
            this->textFontName = textFontName;
			updateTextFont();
        }
    }

    /*
    * @brief Establece la fuente del texto del boton
    * @param textFont Fuente del texto del boton
    */
    inline void setTextFont(ImFont* textFont) { this->textFont = textFont; }

    /*
    * @brief Actualiza la fuente del texto del boton
    */
    void updateTextFont();

    /*
    * @brief Establece el color del texto del boton
    * @param textColor Color del texto del boton
    */
    inline void setTextColor(const Vector4& textColor) { this->textColor = textColor; }

    /*
    * @brief Establece el color del boton para el estado "normal"
    * @param normalColor Color del boton para el estado "normal"
    */
    inline void setNormalColor(const Vector4& normalColor) { this->normalColor = normalColor; }

    /*
    * @brief Establece el color del boton para el estado "hover"
    * @param hoverColor Color del boton para el estado "hover"
    */
    inline void setHoverColor(const Vector4& hoverColor) { this->hoverColor = hoverColor; }

    /*
    * @brief Establece el color del boton para el estado "active"
    * @param activeColor Color del boton para el estado "active"
    */
    inline void setActiveColor(const Vector4& activeColor) { this->activeColor = activeColor; }

    /*
    * @brief Devuelve el texto del boton
    * @return Texto del boton
    */
    inline std::string getText() const { return text; }

    /*
    * @brief Devuelve el identificador de la funcion que se ejecuta cuando se hace click en el boton
    * @return Identificador de la funcion que se ejecuta cuando se hace click en el boton
	*/
    inline int getOnClickId() const { return onClickId; }

    /*
    * @brief Devuelve la funcion que se ejecuta cuando se hace click en el boton
    * @return Funcion que se ejecuta cuando se hace click en el boton
	*/
    inline std::function<void()> getOnClick() const { return onClick; }

    /*
    * @brief Devuelve el tamano del padding del boton
    * @return Tamano del padding del boton
    */
    inline Vector2 getPadding() const { return padding; }

    /*
    * @brief Devuelve el tamano de la fuente del texto del boton
    * @return Tamano de la fuente del texto del boton
    */
    inline float getTextSize() const { return textSize; }

    /*
    * @brief Devuelve el nombre de la fuente del texto del boton
    * @return Nombre de la fuente del texto del boton
    */
    inline std::string getTextFontName() const { return textFontName; }

    /*
    * @brief Devuelve la fuente del texto del boton
    * @return Fuente del texto del boton
    */
    inline ImFont* getTextFont() const { return textFont; }

    /*
    * @brief Devuelve el color del texto del boton
    * @return Color del texto del boton
    */
    inline Vector4 getTextColor() const { return textColor; }

    /*
    * @brief Devuelve el color del boton para el estado "normal"
    * @return Color del boton para el estado "normal"
    */
    inline Vector4 getNormalColor() const { return normalColor; }

    /*
    * @brief Devuelve el color del boton para el estado "hover"
    * @return Color del boton para el estado "hover"
    */
    inline Vector4 getHoverColor() const { return hoverColor; }

    /*
    * @brief Devuelve el color del boton para el estado "active"
    * @return Color del boton para el estado "active"
    */
    inline Vector4 getActiveColor() const { return activeColor; }
};
}