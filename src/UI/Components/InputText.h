#pragma once
#include "BaseWidget.h"
#include "Structure/Component.h"
#include "Utilities/Vector2.h"
#include <string>
#include <functional>
#include <imgui.h>

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
/*
* @brief Enumeracion de las funciones de la caja de texto
*/
enum InputTextFunction { INPUT_TEXT_NONE, INPUT_TEXT_TEXT_ENTERED };

/*
* @brief Clase que representa una caja de texto en la interfaz grafica con la que el usuario puede interactuar para realizar cierta accion
*/
class TAPIOCA_API InputText : public BaseWidget, public Component {
private:
    std::string placeHolderText;           // Texto que se muestra en la caja de texto
    char* buffer;                          // Buffer de la caja de texto
    unsigned int bufferSize;               // Tamano del buffer de la caja de texto
    int onTextEnteredId;                   // Identificador de la funcion que se ejecuta cuando se hace ENTER en la caja de texto
    std::function<void()> onTextEntered;   // Funcion que se ejecuta cuando se hace ENTER en la caja de texto
    float textSize;                        // Tamano de la fuente del texto de la caja de texto
    std::string textFontName;              // Nombre de la fuente del texto de la caja de texto
    ImFont* textFont;                      // Fuente del texto de la caja de texto incluyendo tamano
    ImVec4 textColor;                      // Color del texto de la caja de texto
    ImVec4 bgColor;                        // Color de fondo de la caja de texto
    ImGuiInputTextFlags flags;             // Flags de la caja de texto

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
    * @brief Asigna el transform del objeto a la caja de texto, inicializa el buffer y la fuente del texto
    */
    void awake() override;

    /*
    * @brief Metodo que se usa para renderizar la caja de texto
    */
    void render() const override;

    /*
    * @brief Establece el texto de la caja de texto
    * @param placeHolderText Texto de la caja de texto
    */
    inline void setPlaceHolderText(const std::string& placeHolderText) {
        this->placeHolderText = placeHolderText;
        startBuffer();
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
    inline void setBufferSize(size_t bufferSize) { this->bufferSize = bufferSize; }

    /*
    * @brief Establece el identificador de la funcion que se ejecuta cuando se hace ENTER en la caja de texto
    * @param id Identificador de la funcion que se ejecuta cuando se hace ENTER en la caja de texto
    */
    inline void setOnTextEnterId(int id) { this->onTextEnteredId = id; }

    /*
    * @brief Establece la funcion que se ejecuta cuando se hace ENTER en la caja de texto
    * @param onTextEnter Funcion que se ejecuta cuando se hace ENTER en la caja de texto
    */
    inline void setOnTextEnter(std::function<void()> onTextEnter) { this->onTextEntered = onTextEnter; }

    /*
    * @brief Establece el tamano del texto del boton
    * @param textSize Tamano del texto del boton
    */
    inline void setTextSize(float textSize) { this->textSize = textSize; }

    /*
    * @brief Establece el nombre de la fuente del texto de la caja de texto
    * @param textFontName Nombre de la fuente del texto de la caja de texto
    */
    inline void setTextFontName(std::string textFontName) { this->textFontName = textFontName; }

    /*
    * @brief Establece la fuente del texto de la caja de texto
    * @param textFont Fuente del texto de la caja de texto
    */
    inline void setFont(ImFont* textFont) { this->textFont = textFont; }

    /*
    * @brief Establece el color del texto de la caja de texto
    * @param textColor Color del texto de la caja de texto
    */
    inline void setTextColor(const ImVec4& textColor) { this->textColor = textColor; }

    /*
    * @brief Establece el color de fondo de la caja de texto
    * @param bgColor Color de fondo de la caja de texto
    */
    inline void setBgColor(const ImVec4& bgColor) { this->bgColor = bgColor; }

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
    * @brief Devuelve el identificador de la funcion que se ejecuta cuando se hace ENTER en la caja de texto
    * @return Identificador de la funcion que se ejecuta cuando se hace ENTER en la caja de texto
	*/
    inline int getOnTextEnteredId() const { return onTextEnteredId; }

    /*
    * @brief Devuelve la funcion que se ejecuta cuando se hace ENTER en la caja de texto
    * @return Funcion que se ejecuta cuando se hace ENTER en la caja de texto
	*/
    inline std::function<void()> getOnTextEntered() const { return onTextEntered; }

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
    inline ImVec4 getTextColor() const { return textColor; }

    /*
    * @brief Devuelve el color de fondo de la caja de texto
    * @return Color de fondo de la caja de texto
    */
    inline ImVec4 getBgColor() const { return bgColor; }

    /*
    * @brief Devuelve los flags de la caja de texto
    * @return Flags de la caja de texto
	*/
    inline ImGuiInputTextFlags getFlags() const { return flags; }
};
}
