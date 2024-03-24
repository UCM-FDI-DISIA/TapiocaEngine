#pragma once
#include "BaseWidget.h"
#include <string>
#include <functional>
#include <imgui.h>
#include "Utilities/Vector2.h"

namespace Ogre {
class SceneManager;
}

namespace Tapioca {
/*
* @brief Clase que representa un texto en la interfaz grafica
*/
class TAPIOCA_API Text : public BaseWidget {
private:
    std::string text;               // Texto
    ImVec2 constSize;               // Tamano constante del texto
    ImFont* textFont;               // Fuente del texto
    ImVec4 textColor;               // Color del texto

public:
    /*
    * @brief Estructura que contiene las opciones para inicializar un texto
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
};
}