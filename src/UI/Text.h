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
* @brief Clase que representa una caja de texto en la interfaz grafica con la que el usuario puede interactuar para realizar cierta accion
*/
class TAPIOCA_API Text {
private:
    std::string name;               // Nombre del texto
    ImVec2 position;                // Posicion del texto
    std::string text;               // Texto
    ImVec2 constSize;               // Tamano constante del texto
    ImFont* textFont;               // Fuente del texto
    ImVec4 textColor;               // Color del texto
    bool* canCloseWindow;           // Puntero a booleano que indica si se puede cerrar la ventana
    ImGuiWindowFlags windowFlags;   // Flags de la ventana

public:
    /*
    * @brief Estructura que contiene las opciones para inicializar una caja de texto
    * @param name Nombre del texto
    * @param position Posicion del texto
    * @param text Texto
    * @param constSize Tamano constante del texto
    * @param textFont Fuente del texto
    * @param textColor Color del texto
    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
    * @param windowFlags Flags de la ventana
    */
    struct TextOptions {
        std::string name;
        ImVec2 position = ImVec2(0, 0);
        std::string text = "Texto";
        ImVec2 constSize = ImVec2(130, 40);
        ImFont* textFont;
        ImVec4 textColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
        bool* canCloseWindow = nullptr;
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings;
    };

    /*
    * @brief Inicializa el texto con los parametros dados
    * @param options Opciones del texto
    */
    Text(const TextOptions& options);

    /*
    * @brief Inicializa un texto
    * @param name Nombre del texto
    * @param position Posicion del texto
    * @param text Texto
    * @param constSize Tamano constante del texto
    * @param textFont Fuente del texto
    * @param textColor Color del texto
    * @param canCloseWindow Puntero a booleano que indica si se puede cerrar la ventana
    * @param windowFlags Flags de la ventana
    */
    Text(const std::string& name, const ImVec2& position, const std::string& text, const ImVec2& constSize, ImFont* const textFont,
         const ImVec4& textColor, bool* canCloseWindow, ImGuiWindowFlags windowFlags);

    virtual ~Text() { }

    /*
    * @brief Establece el nombre del texto
    * @param name Nombre del texto
    */
    inline void setName(const std::string& name) { this->name = name; }

    /*
    * @brief Establece la posicion del texto
    * @param position Posicion del texto
    */
    inline void setPosition(const ImVec2 position) { this->position = position; }

    /*
    * @brief Establece el texto
    * @param text Texto
    */
    inline void setText(const std::string& text) { this->text = text; }

    /*
    * @brief Establece el tamano constante del texto
    */
    inline void setConstSize(ImVec2 constSize) { this->constSize = constSize; }

    /*
    * @brief Establece la fuente del texto
    * @param font Fuente del texto
    */
    inline void setFont(ImFont* textFont) { this->textFont = textFont; }

    /*
    * @brief Establece el color del texto
    * @param textColor Color del texto
    */
    inline void setTextColor(const ImVec4& textColor) { this->textColor = textColor; }

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
    * @brief Devuelve el nombre del texto
    * @return Nombre del texto
    */
    inline std::string getName() const { return name; }

    /*
    * @brief Devuelve la posicion del texto
    * @return Posicion del texto
	*/
    inline ImVec2 getPosition() const { return position; }

    /*
    * @brief Devuelve el texto
    * @return Texto
    */
    inline std::string getText() const { return text; }

    /*
    * @brief Devuelve el tamano constante del texto
    * @return Tamano constante del texto
    */
    inline ImVec2 getConstSize() const { return constSize; }

    /*
    * @brief Devuelve la fuente del texto
    * @return Fuente del texto
    */
    inline ImFont* getFont() const { return textFont; }

    /*
    * @brief Devuelve el color del texto
    * @return Color del texto
    */
    inline ImVec4 getTextColor() const { return textColor; }

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