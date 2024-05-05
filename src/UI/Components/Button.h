#pragma once
#include "BaseWidget.h"
#include "Structure/Component.h"
#include <string>
#include "Utilities/Vector4.h"

struct ImFont;

namespace Tapioca {
/**
* @brief Clase que representa un boton en la interfaz grafica con la que el usuario puede interactuar para realizar cierta accion
*/
class TAPIOCA_API Button : public BaseWidget, public Component {
protected:
    // warning C4251 'Tapioca::Button::onClickId' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita tener una interfaz DLL
    // para que la utilicen los clientes de class 'Tapioca::Button'
    // warning C4251 'Tapioca::Button::text' :
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita tener una interfaz DLL
    // para que la utilicen los clientes de class 'Tapioca::Button'
    // warning C4251 'Tapioca::Button::textFontName' : 
    // class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' necesita tener una interfaz DLL
    // para que la utilicen los clientes de class 'Tapioca::Button'
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    std::string onClickId;      // Id de la funcion que se ejecuta cuando se hace click en el boton
    std::string text;           // Texto que se muestra en el boton
    std::string textFontName;   // Nombre de la fuente del texto del boton
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
    
    float textSize;            // Tamano de la fuente del texto del boton
    float initialTextSize;     // Tamano inicial de la fuente del texto del boton
    ImFont* textFont;          // Fuente del texto del boton incluyendo tamano
    Vector4 textColorNormal;   // Color del texto del boton en estado "normal"
    Vector4 textColorHover;    // Color del texto del boton en estado "hover"
    Vector4 textColorActive;   // Color del texto del boton en estado "active"
    Vector4 normalColor;       // Color del boton para el estado "normal"
    Vector4 hoverColor;        // Color del boton para el estado "hover"
    Vector4 activeColor;       // Color del boton para el estado "active"

public:
    COMPONENT_ID("Button")

    /**
    * @brief Constructor por defecto
    */
    Button();

    /**
    * @brief Destructor por defecto
    */
    ~Button();

    /**
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * @param variables unordered_map con los parametros iniciales
    */
    virtual bool initComponent(const CompMap& variables) override;

    /**
    * @brief Asigna el transform del objeto al boton, inicializa la funcion que se ejecuta cuando se hace click en el boton y la fuente del texto
    */
    virtual void start() override;

    /**
    * @brief Actualiza el tamano de la fuente del texto del boton
    */
    virtual void updateUI() override;

    /**
    * @brief Metodo que se usa para renderizar el boton
    */
    virtual void render() const override;

    /**
    * @brief Establece el id de la funcion que se ejecuta cuando se hace click en el boton
    * @param onClickId Id de la funcion que se ejecuta cuando se hace click en el boton
    */
    inline void setOnClickId(const std::string& onClickId) { this->onClickId = onClickId; }

    /**
    * @brief Establece el texto del boton
    * @param text Texto del boton
    */
    inline void setText(const std::string& text) { this->text = text; }

    /**
    * @brief Establece el tamano del texto del boton
    */
    inline void setTextSize(float textSize) {
        if (textSize != this->textSize) {
            this->textSize = textSize;
            updateTextFont();
        }
    }

    /**
    * @brief Establece el tamano del texto del boton
    */
    inline void setTextFontName(std::string textFontName) {
        if (textFontName != this->textFontName) {
            this->textFontName = textFontName;
            updateTextFont();
        }
    }

    /**
    * @brief Establece la fuente del texto del boton
    * @param textFont Fuente del texto del boton
    */
    inline void setTextFont(ImFont* textFont) { this->textFont = textFont; }

    /**
    * @brief Actualiza la fuente del texto del boton
    */
    void updateTextFont();

    /**
    * @brief Establece el color del texto del boton en estado "normal"
    * @param textColor Color del texto del boton en estado "normal"
    */
    inline void setTextColorNormal(const Vector4& textColor) { this->textColorNormal = textColor; }

    /**
    * @brief Establece el color del texto del boton en estado "hover"
    * @param textColor Color del texto del boton en estado "hover"
    */
    inline void setTextColorHover(const Vector4& textColor) { this->textColorHover = textColor; }

    /**
    * @brief Establece el color del texto del boton en estado "active"
    * @param textColor Color del texto del boton en estado "active"
    */

    inline void setTextColorActive(const Vector4& textColor) { this->textColorActive = textColor; }
    /**
    * @brief Establece el color del boton para el estado "normal"
    * @param normalColor Color del boton para el estado "normal"
    */
    inline void setNormalColor(const Vector4& normalColor) { this->normalColor = normalColor; }

    /**
    * @brief Establece el color del boton para el estado "hover"
    * @param hoverColor Color del boton para el estado "hover"
    */
    inline void setHoverColor(const Vector4& hoverColor) { this->hoverColor = hoverColor; }

    /**
    * @brief Establece el color del boton para el estado "active"
    * @param activeColor Color del boton para el estado "active"
    */
    inline void setActiveColor(const Vector4& activeColor) { this->activeColor = activeColor; }

    /**
    * @brief Devuelve el id de la funcion que se ejecuta cuando se hace click en el boton
    * @return Id de la funcion que se ejecuta cuando se hace click en el boton
    */
    inline std::string getOnClickId() const { return onClickId; }

    /**
    * @brief Devuelve el texto del boton
    * @return Texto del boton
    */
    inline std::string getText() const { return text; }

    /**
    * @brief Devuelve el tamano de la fuente del texto del boton
    * @return Tamano de la fuente del texto del boton
    */
    inline float getTextSize() const { return textSize; }

    /**
    * @brief Devuelve el nombre de la fuente del texto del boton
    * @return Nombre de la fuente del texto del boton
    */
    inline std::string getTextFontName() const { return textFontName; }

    /**
    * @brief Devuelve la fuente del texto del boton
    * @return Fuente del texto del boton
    */
    inline ImFont* getTextFont() const { return textFont; }

    /**
    * @brief Devuelve el color del texto del boton en estado "normal"
    * @return Color del texto del boton en estado "normal"
    */
    inline Vector4 getTextColorNormal() const { return textColorNormal; }

    /**
    * @brief Devuelve el color del texto del boton en estado "hover"
    * @return Color del texto del boton en estado "hover"
    */
    inline Vector4 getTextColorHover() const { return textColorHover; }

    /**
    * @brief Devuelve el color del texto del boton en estado "active"
    * @return Color del texto del boton en estado "active"
    */
    inline Vector4 getTextColorActive() const { return textColorActive; }

    /**
    * @brief Devuelve el color del boton para el estado "normal"
    * @return Color del boton para el estado "normal"
    */
    inline Vector4 getNormalColor() const { return normalColor; }

    /**
    * @brief Devuelve el color del boton para el estado "hover"
    * @return Color del boton para el estado "hover"
    */
    inline Vector4 getHoverColor() const { return hoverColor; }

    /**
    * @brief Devuelve el color del boton para el estado "active"
    * @return Color del boton para el estado "active"
    */
    inline Vector4 getActiveColor() const { return activeColor; }
};
}
