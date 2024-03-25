//#pragma once
//#include "BaseWidget.h"
//#include <string>
//#include <functional>
//#include <imgui.h>
//#include "Utilities/Vector2.h"
//
//namespace Ogre {
//class SceneManager;
//}
//
//namespace Tapioca {
///*
//* @brief Clase que representa una caja de texto en la interfaz grafica con la que el usuario puede interactuar para realizar cierta accion
//*/
//class TAPIOCA_API InputText : public BaseWidget {
//private:
//    std::string placeHolderText;           // Texto que se muestra en la caja de texto
//    char* buffer;                          // Buffer de la caja de texto
//    size_t bufferSize;                     // Tamano del buffer de la caja de texto
//    std::function<void()> onTextEntered;   // Funcion que se ejecuta cuando se hace ENTER en la caja de texto
//    float constWidth;                      // Ancho constante de la caja de texto
//    ImFont* textFont;                      // Fuente del texto de la caja de texto incluyendo tamano
//    ImVec4 textColor;                      // Color del texto de la caja de texto
//    ImVec4 bgColor;                        // Color de fondo de la caja de texto
//    ImGuiInputTextFlags flags;             // Flags de la caja de texto
//    ImGuiInputTextCallback callback;       // Callback de la caja de texto
//    void* userData;                        // Datos del callback de la caja de texto
//
//public:
//    /*
//    * @brief Estructura que contiene las opciones para inicializar una caja de texto
//    * @param name Nombre de la caja de texto
//    * @param position Posicion de la caja de texto
//    * @param placeHolderText Texto que se muestra en la caja de texto
//    * @param bufferSize Tamano del buffer de la caja de texto
//    * @param onTextEntered Funcion que se ejecuta cuando se hace ENTER en la caja de texto
//    * @param constWidth Ancho de la caja de texto
//    * @param textFont Fuente del texto de la caja de texto
//    * @param textColor Color del texto de la caja de texto
//    * @param bgColor Color de fondo de la caja de texto
//    * @param flags Flags de la caja de texto
//    * @param callback Callback de la caja de texto
//    * @param userData Datos del callback de la caja de texto
//    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
//    * @param windowFlags Flags de la ventana
//    */
//    struct InputTextOptions {
//        std::string name;
//        ImVec2 position = ImVec2(0, 0);
//        std::string placeHolderText = "Inserta texto...";
//        size_t bufferSize = 256;
//        std::function<void()> onTextEntered = []() {};
//        float constWidth = 130.0f;
//        ImFont* textFont;
//        ImVec4 textColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
//        ImVec4 bgColor = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
//        ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue;
//        ImGuiInputTextCallback callback = ImGuiInputTextCallback();
//        void* userData = nullptr;
//        bool* canCloseWindow = nullptr;
//        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
//            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings;
//    };
//
//    /*
//    * @brief Inicializa la caja de texto con los parametros dados
//    * @param options Opciones de la caja de texto
//    */
//    InputText(const InputTextOptions& options);
//
//    /*
//    * @brief Inicializa una caja de texto con los parametros dados
//    * @param name Nombre de la caja de texto
//    * @param position Posicion de la caja de texto
//    * @param placeHolderText Texto que se muestra al principio en la caja de texto
//    * @param bufferSize Tamano del buffer de la caja de texto
//    * @param onTextEntered Funcion que se ejecuta cuando se hace ENTER en la caja de texto
//    * @param constWidth Ancho de la caja de texto
//    * @param textFont Fuente del texto de la caja de texto
//    * @param textColor Color del texto de la caja de texto
//    * @param bgColor Color de fondo de la caja de texto
//    * @param flags Flags de la ventana de la caja de texto
//    * @param callback Callback de la caja de texto
//    * @param userData Datos del callback de la caja de texto
//    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
//    * @param windowFlags Flags de la ventana
//    */
//    InputText(const std::string& name, const ImVec2& position, const std::string& placeHolderText,
//              const size_t bufferSize, std::function<void()> onTextEntered, const float constWidth,
//              ImFont* const textFont, const ImVec4& textColor, const ImVec4& bgColor, const ImGuiInputTextFlags& flags,
//              const ImGuiInputTextCallback& callback, void* userData, bool* canCloseWindow,
//              ImGuiWindowFlags windowFlags);
//
//    virtual ~InputText();
//
//    /*
//    * @brief Inicializa el buffer de la caja de texto con el texto del placeholder
//    */
//    void startBuffer();
//
//    /*
//    * @brief Establece el texto de la caja de texto
//    * @param placeHolderText Texto de la caja de texto
//    */
//    inline void setPlaceHolderText(const std::string& placeHolderText) {
//        this->placeHolderText = placeHolderText;
//        startBuffer();
//    }
//
//    /*
//    * @brief Establece el buffer de la caja de texto
//    * @param buffer Buffer de la caja de texto
//    */
//    inline void setBuffer(char* buffer) { this->buffer = buffer; }
//
//    /*
//    * @brief Establece el tamano del buffer de la caja de texto
//    * @param bufferSize Tamano del buffer de la caja de texto
//    */
//    inline void setBufferSize(size_t bufferSize) { this->bufferSize = bufferSize; }
//
//    /*
//    * @brief Establece la funcion que se ejecuta cuando se hace ENTER en la caja de texto
//    * @param onTextEnter Funcion que se ejecuta cuando se hace ENTER en la caja de texto
//    */
//    inline void setOnTextEnter(std::function<void()> onTextEnter) { this->onTextEntered = onTextEnter; }
//
//    /*
//    * @brief Establece el tamano constante de la caja de texto
//    * @param constWidth Tamano constante de la caja de texto
//    */
//    inline void setConstWidth(const float constWidth) { this->constWidth = constWidth; }
//
//    /*
//    * @brief Establece la fuente del texto de la caja de texto
//    * @param font Fuente del texto de la caja de texto
//    */
//    inline void setFont(ImFont* textFont) { this->textFont = textFont; }
//
//    /*
//    * @brief Establece el color del texto de la caja de texto
//    * @param textColor Color del texto de la caja de texto
//    */
//    inline void setTextColor(const ImVec4& textColor) { this->textColor = textColor; }
//
//    /*
//    * @brief Establece el color de fondo de la caja de texto
//    * @param textColor Color de fondo de la caja de texto
//    */
//    inline void setBgColor(const ImVec4& bgColor) { this->bgColor = bgColor; }
//
//    /*
//    * @brief Establece los flags de la caja de texto
//    * @param flags Flags de la caja de texto
//	*/
//    inline void setFlags(ImGuiInputTextFlags flags) { this->flags = flags; }
//
//    /*
//    * @brief Establece el callback de la caja de texto
//    * @param callback Callback de la caja de texto
//    */
//    inline void setCallback(const ImGuiInputTextCallback& callback) { this->callback = callback; }
//
//    /*
//    * @brief Establece los datos del callback de la caja de texto
//    * @param userData Datos del callback de la caja de texto
//    */
//    inline void setUserData(void* userData) { this->userData = userData; }
//
//    /*
//    * @brief Devuelve el texto de la caja de texto
//    * @return Texto de la caja de texto
//    */
//    inline std::string getPlaceHolderText() const { return placeHolderText; }
//
//    /*
//    * @brief Devuelve el buffer de la caja de texto
//    * @return Buffer de la caja de texto
//    */
//    inline char* getBuffer() const { return buffer; }
//
//    /*
//    * @brief Devuelve el tamano del buffer de la caja de texto
//    * @return Tamano del buffer de la caja de texto
//    */
//    inline size_t getBufferSize() const { return bufferSize; }
//
//    /*
//    * @brief Devuelve la funcion que se ejecuta cuando se hace ENTER en la caja de texto
//    * @return Funcion que se ejecuta cuando se hace ENTER en la caja de texto
//	*/
//    inline std::function<void()> getOnTextEntered() const { return onTextEntered; }
//
//    /*
//    * @brief Devuelve el tamano constante de la caja de texto
//    * @return Tamano constante de la caja de texto
//    */
//    inline float getConstWidth() const { return constWidth; }
//
//    /*
//    * @brief Devuelve la fuente del texto de la caja de texto
//    * @return Fuente del texto de la caja de texto
//    */
//    inline ImFont* getFont() const { return textFont; }
//
//    /*
//    * @brief Devuelve el color del texto de la caja de texto
//    * @return Color del texto de la caja de texto
//    */
//    inline ImVec4 getTextColor() const { return textColor; }
//
//    /*
//    * @brief Devuelve el color de fondo de la caja de texto
//    * @return Color de fondo de la caja de texto
//    */
//    inline ImVec4 getBgColor() const { return bgColor; }
//
//    /*
//    * @brief Devuelve los flags de la caja de texto
//    * @return Flags de la caja de texto
//	*/
//    inline ImGuiInputTextFlags getFlags() const { return flags; }
//
//    /*
//    * @brief Devuelve el callback de la caja de texto
//    * @return Callback de la caja de texto
//    */
//    inline ImGuiInputTextCallback getCallback() const { return callback; }
//
//    /*
//    * @brief Devuelve los datos del callback de la caja de texto
//    * @return Datos del callback de la caja de texto
//    */
//    inline void* getUserData() const { return userData; }
//};
//}