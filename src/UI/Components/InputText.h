#pragma once
#include "BaseWidget.h"
#include "Structure/Component.h"
#include <string>
#include "Utilities/Vector4.h"

struct ImFont;
typedef int ImGuiInputTextFlags;

namespace Tapioca {
/*
* @brief Clase que representa una caja de texto en la interfaz grafica con la que el usuario puede interactuar para realizar cierta accion
*/
class TAPIOCA_API InputText : public BaseWidget, public Component {
private:
    std::string placeHolderText;   // Texto que se muestra en la caja de texto
    char* buffer;                  // Buffer de la caja de texto
    int bufferSize;                // Tamano del buffer de la caja de texto
    std::string onTextEnteredId;   // Id de la funcion que se ejecuta cuando se hace ENTER en la caja de texto
    float textSize;                // Tamano de la fuente del texto de la caja de texto
    std::string textFontName;      // Nombre de la fuente del texto de la caja de texto
    ImFont* textFont;              // Fuente del texto de la caja de texto incluyendo tamano
    Vector4 textColor;             // Color del texto de la caja de texto
    Vector4 bgColor;               // Color de fondo de la caja de texto
    ImGuiInputTextFlags flags;     // Flags de la caja de texto

public:
    COMPONENT_ID("InputText")

    /*
    * @brief Constructor por defecto
    */
    InputText();

    /*
    * @brief Destructor por defecto
    */
    ~InputText();

    /*
    * @brief Inicializa el buffer de la caja de texto con el texto del placeholder
    */
    void startBuffer();

    /*
    * @brief Metodo que se usa para inicializar el componente. Se ejecuta antes que el start
    * @param variables unordered_map con los parametros iniciales
    */
    bool initComponent(const CompMap& variables) override;

    /*
    * @brief Asigna el transform del objeto a la caja de texto, inicializa la funcion que se ejecuta cuando se hacer ENTER en la caja de texto, el buffer y la fuente del texto
    */
    void start() override;

    /*
    * @brief Metodo que se usa para renderizar la caja de texto
    */
    void render() const override;

    /*
    * @brief Establece el texto de la caja de texto
    * @param placeHolderText Texto de la caja de texto
    */
    inline void setPlaceHolderText(const std::string& placeHolderText) {
        if (placeHolderText != this->placeHolderText) {
            this->placeHolderText = placeHolderText;
            startBuffer();
        }
    }

    /*
    * @brief Establece el buffer de la caja de texto
    * @param buffer Buffer de la caja de texto
    */
    inline void setBuffer(char* buffer) { this->buffer = buffer; }

    /*
    * @brief Establece el tamano del buffer de la caja de texto
    * @param bufferSize Tamano del buffer de la caja de texto
    */
    inline void setBufferSize(size_t bufferSize) {
        if (bufferSize != this->bufferSize) {
            this->bufferSize = bufferSize;
            startBuffer();
        }
    }

    /*
    * @brief Establece el id de la funcion que se ejecuta cuando se hace ENTER en la caja de texto
    * @param onTextEnterId Id de la funcion que se ejecuta cuando se hace ENTER en la caja de texto
    */
    inline void setOnTextEnterId(std::string onTextEnterId) { this->onTextEnteredId = onTextEnterId; }

    /*
    * @brief Establece el tamano del texto del boton
    * @param textSize Tamano del texto del boton
    */
    inline void setTextSize(float textSize) {
        if (textSize != this->textSize) {
            this->textSize = textSize;
            updateTextFont();
        }
    }

    /*
    * @brief Establece el nombre de la fuente del texto de la caja de texto
    * @param textFontName Nombre de la fuente del texto de la caja de texto
    */
    inline void setTextFontName(std::string textFontName) {
        if (textFontName != this->textFontName) {
            this->textFontName = textFontName;
            updateTextFont();
        }
    }

    /*
    * @brief Establece la fuente del texto de la caja de texto
    * @param textFont Fuente del texto de la caja de texto
    */
    inline void setFont(ImFont* textFont) { this->textFont = textFont; }

    /*
    * @brief Actualiza la fuente del texto de la caja de texto
    */
    void updateTextFont();

    /*
    * @brief Establece el color del texto de la caja de texto
    * @param textColor Color del texto de la caja de texto
    */
    inline void setTextColor(const Vector4& textColor) { this->textColor = textColor; }

    /*
    * @brief Establece el color de fondo de la caja de texto
    * @param bgColor Color de fondo de la caja de texto
    */
    inline void setBgColor(const Vector4& bgColor) { this->bgColor = bgColor; }

    /*
    * @brief Establece los flags de la caja de texto
    * @param flags Flags de la caja de texto
	*/
    inline void setFlags(ImGuiInputTextFlags flags) { this->flags = flags; }

    /*
    * @brief Devuelve el texto de la caja de texto
    * @return Texto de la caja de texto
    */
    inline std::string getPlaceHolderText() const { return placeHolderText; }

    /*
    * @brief Devuelve el buffer de la caja de texto
    * @return Buffer de la caja de texto
    */
    inline char* getBuffer() const { return buffer; }

    /*
    * @brief Devuelve el tamano del buffer de la caja de texto
    * @return Tamano del buffer de la caja de texto
    */
    inline size_t getBufferSize() const { return bufferSize; }

    /*
    * @brief Devuelve el id de la funcion que se ejecuta cuando se hace ENTER en la caja de texto
    * @return Id de la funcion que se ejecuta cuando se hace ENTER en la caja de texto
	*/
    inline std::string getOnTextEnteredId() const { return onTextEnteredId; }

    /*
    * @brief Devuelve el tamano de la fuente del texto de la caja de texto
    * @return Tamano de la fuente del texto de la caja de texto
    */
    inline float getTextSize() const { return textSize; }

    /*
    * @brief Devuelve el nombre de la fuente del texto de la caja de texto
    * @return Nombre de la fuente del texto de la caja de texto
    */
    inline std::string getTextFontName() const { return textFontName; }

    /*
    * @brief Devuelve la fuente del texto de la caja de texto
    * @return Fuente del texto de la caja de texto
    */
    inline ImFont* getTextFont() const { return textFont; }

    /*
    * @brief Devuelve el color del texto de la caja de texto
    * @return Color del texto de la caja de texto
    */
    inline Vector4 getTextColor() const { return textColor; }

    /*
    * @brief Devuelve el color de fondo de la caja de texto
    * @return Color de fondo de la caja de texto
    */
    inline Vector4 getBgColor() const { return bgColor; }

    /*
    * @brief Devuelve los flags de la caja de texto
    * @return Flags de la caja de texto
	*/
    inline ImGuiInputTextFlags getFlags() const { return flags; }
};
}
