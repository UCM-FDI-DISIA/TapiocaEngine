#pragma once
#include <string>
#include <functional>
#include <imgui.h>
#include "Utilities/Vector2.h"

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
/*
* @brief Clase que representa un botón en la interfaz grafica con la que el usuario puede interactuar para realizar cierta accion
*/
class TAPIOCA_API Button {
private:
    ImVec2 position;                 // Posicion del boton
    std::string text;                // Texto que se muestra en el boton
    std::function<void()> onClick;   // Funcion que se ejecuta cuando se hace click en el boton
    ImVec2 constSize;                // Tamano constante del boton
    ImVec2 padding;                  // Tamano del padding del boton
    ImFont* textFont;                // Fuente del texto del boton incluyendo tamano
    ImVec4 textColor;                // Color del texto del boton
    ImVec4 normalColor;              // Color del boton para el estado "normal"
    ImVec4 hoverColor;               // Color del boton para el estado "hover"
    ImVec4 activeColor;              // Color del boton para el estado "active"
    bool* canCloseWindow;            // Puntero a booleano que indica si se puede cerrar la ventana
    ImGuiWindowFlags windowFlags;    // Flags de la ventana

public:
    /*
    * @brief Estructura que contiene las opciones para inicializar un boton
    * @param position Posicion del boton
    * @param text Texto que se muestra en el boton
    * @param onClick Funcion que se ejecuta cuando se hace click en el boton
    * @param constSize Tamano del boton
    * @param padding Padding del boton
    * @param textFont Fuente del texto del boton
    * @param textColor Color del texto del boton
    * @param normalColor Color del boton para el estado "normal"
    * @param hoverColor Color del boton para el estado "hover"
    * @param activeColor Color del boton para el estado "active"
    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
    * @param windowFlags Flags de la ventana
    */
    struct ButtonOptions {
        ImVec2 position = ImVec2(0, 0);
        std::string text = "Button";
        std::function<void()> onClick = []() {};
        ImVec2 constSize = ImVec2(130, 40);
        ImVec2 padding = ImVec2(10, 5);
        ImFont* textFont;
        ImVec4 textColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
        ImVec4 normalColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
        ImVec4 hoverColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
        ImVec4 activeColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
        bool* canCloseWindow = nullptr;
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings;
    };

    /*
    * @brief Inicializa un boton con los parametros dados
    * @param options Opciones del boton
    */
    Button(const ButtonOptions& options);

    /*
    * @brief Inicializa un boton con los parametros dados
    * @param position Posicion del boton
    * @param text Texto que se muestra en el boton
    * @param onClick Funcion que se ejecuta cuando se hace click en el boton
    * @param constSize Tamano del boton
    * @param padding Padding del boton
    * @param textFont Fuente del texto del boton
    * @param textColor Color del texto del boton
    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
    * @param windowFlags Flags de la ventana
    */
    Button(const ImVec2& position, const std::string& text, std::function<void()> onClick, const ImVec2& constSize,
           const ImVec2& padding, ImFont* const textFont, const ImVec4& textColor, const ImVec4& normalColor,
           const ImVec4& hoverColor, const ImVec4& activeColor, bool* canCloseWindow, ImGuiWindowFlags windowFlags);

    virtual ~Button() { }

    /*
    * @brief Establece la posicion del boton
    * @param position Posicion del boton
    */
    inline void setPosition(const ImVec2 position) { this->position = position; }

    /*
    * @brief Establece el texto del boton
    * @param text Texto del boton
    */
    inline void setText(const std::string& text) { this->text = text; }

    /*
    * @brief Establece la funcion que se ejecuta cuando se hace click en el boton
    * @param onClick Funcion que se ejecuta cuando se hace click en el boton
    */
    inline void setOnClick(std::function<void()> onClick) { this->onClick = onClick; }

    /*
    * @brief Establece el tamano constante del boton
    * @param constSize Tamano constante del boton
    */
    inline void setConstSize(const ImVec2& constSize) { this->constSize = constSize; }

    /*
    * @brief Establece el tamano del padding del boton
    * @param padding Tamano del padding del boton
    */
    inline void setPadding(const ImVec2& padding) { this->padding = padding; }

    /*
    * @brief Establece la fuente del texto del boton
    * @param textFont Fuente del texto del boton
    */
    inline void setFont(ImFont* textFont) { this->textFont = textFont; }

    /*
    * @brief Establece el color del texto del boton
    * @param textColor Color del texto del boton
    */
    inline void setTextColor(const ImVec4& textColor) { this->textColor = textColor; }

    /*
    * @brief Establece el color del boton para el estado "normal"
    * @param normalColor Color del boton para el estado "normal"
    */
    inline void setNormalColor(const ImVec4& normalColor) { this->normalColor = normalColor; }

    /*
    * @brief Establece el color del boton para el estado "hover"
    * @param hoverColor Color del boton para el estado "hover"
    */
    inline void setHoverColor(const ImVec4& hoverColor) { this->hoverColor = hoverColor; }

    /*
    * @brief Establece el color del boton para el estado "active"
    * @param activeColor Color del boton para el estado "active"
    */
    inline void setActiveColor(const ImVec4& activeColor) { this->activeColor = activeColor; }

    /*
    * @brief Establece el puntero a booleano que indica si se puede cerrar la ventana
    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
	*/
    inline void setCanCloseWindow(bool* canCloseWindow) { this->canCloseWindow = canCloseWindow; }

    /*
    * @brief Establece los flags de la ventana
    * @param windowFlags Flags de la ventana
	*/
    inline void setWindowFlags(ImGuiWindowFlags windowFlags) { this->windowFlags = windowFlags; }

    /*
    * @brief Devuelve la posicion del boton
    * @return Posicion del boton
	*/
    inline ImVec2 getPosition() const { return position; }

    /*
    * @brief Devuelve el texto del boton
    * @return Texto del boton
    */
    inline std::string getText() const { return text; }

    /*
    * @brief Devuelve la funcion que se ejecuta cuando se hace click en el boton
    * @return Funcion que se ejecuta cuando se hace click en el boton
	*/
    inline std::function<void()> getOnClick() const { return onClick; }

    /*
    * @brief Devuelve el tamano constante del boton
    * @return Tamano constante del boton
    */
    inline ImVec2 getConstSize() const { return constSize; }

    /*
    * @brief Devuelve el tamano del padding del boton
    * @return Tamano del padding del boton
    */
    inline ImVec2 getPadding() const { return padding; }

    /*
    * @brief Devuelve la fuente del texto del boton
    * @return Fuente del texto del boton
    */
    inline ImFont* getFont() const { return textFont; }

    /*
    * @brief Devuelve el color del texto del boton
    * @return Color del texto del boton
    */
    inline ImVec4 getTextColor() const { return textColor; }

    /*
    * @brief Devuelve el color del boton para el estado "normal"
    * @return Color del boton para el estado "normal"
    */
    inline ImVec4 getNormalColor() const { return normalColor; }

    /*
    * @brief Devuelve el color del boton para el estado "hover"
    * @return Color del boton para el estado "hover"
    */
    inline ImVec4 getHoverColor() const { return hoverColor; }

    /*
    * @brief Devuelve el color del boton para el estado "active"
    * @return Color del boton para el estado "active"
    */
    inline ImVec4 getActiveColor() const { return activeColor; }

    /*
    * @brief Devuelve el puntero a booleano que indica si se puede cerrar la ventana
    * @return Puntero a booleano que indica si se puede cerrar la ventana
    */
    inline bool* getCanCloseWindow() const { return canCloseWindow; }

    /*
    * @brief Devuelve los flags de la ventana
    * @return Flags de la ventana
	*/
    inline ImGuiWindowFlags getWindowFlags() const { return windowFlags; }
};
}