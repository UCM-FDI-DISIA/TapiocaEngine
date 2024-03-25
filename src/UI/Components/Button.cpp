#include "Button.h"
#include <OgreSceneManager.h>
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "UIManager.h"

namespace Tapioca {
Button::Button()
    : BaseWidget(), Component(), text(""), onClick([]() {}), padding(ImVec2(10, 5)), textFont(nullptr),
      textFontName(""), textSize(0.0f), textColor(ImGui::GetStyle().Colors[ImGuiCol_Text]),
      normalColor(ImGui::GetStyle().Colors[ImGuiCol_Button]),
      hoverColor(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]),
      activeColor(ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]) { }

bool Button::initComponent(const CompMap& variables) {
    bool nameSet = setValueFromMap(name, "name", variables);
    if (!nameSet) {
#ifdef _DEBUG
        std::cerr << "Error: Button: no se pudo inicializar el nombre.\n";
#endif
        return false;
    }

    bool textSet = setValueFromMap(text, "text", variables);
    if (!textSet) {
#ifdef _DEBUG
        std::cerr << "Error: Button: no se pudo inicializar el texto.\n";
#endif
        return false;
    }

    //bool onClickSet = setValueFromMap(onClick, "onClick", variables);

    bool paddingSet =
        setValueFromMap(padding.x, "paddingX", variables) && setValueFromMap(padding.y, "paddingY", variables);
    if (!paddingSet) {
#ifdef _DEBUG std::cerr << "Error: Button: no se pudo inicializar el padding.\n";
#endif
        return false;
    }

    bool textFontNameSet = setValueFromMap(textFontName, "textFontName", variables);
    if (!textFontNameSet) {
#ifdef _DEBUG
        std::cerr << "Error: Button: no se pudo inicializar el nombre de la fuente del texto.\n";
#endif
        return false;
    }

    bool textSizeSet = setValueFromMap(textSize, "textSize", variables);
    if (!textSizeSet) {
#ifdef _DEBUG
        std::cerr << "Error: Button: no se pudo inicializar el tamano de la fuente del texto.\n";
#endif
        return false;
    }

    bool textColorSet = setValueFromMap(textColor.x, "textColorR", variables) &&
        setValueFromMap(textColor.y, "textColorG", variables) &&
        setValueFromMap(textColor.z, "textColorB", variables) && setValueFromMap(textColor.w, "textColorA", variables);
    if (!textColorSet) {
#ifdef _DEBUG
        std::cerr << "Error: Button: no se pudo inicializar el color del texto.\n";
#endif
        return false;
    }

    bool normalColorSet = setValueFromMap(normalColor.x, "normalColorR", variables) &&
        setValueFromMap(normalColor.y, "normalColorG", variables) &&
        setValueFromMap(normalColor.z, "normalColorB", variables) &&
        setValueFromMap(normalColor.w, "normalColorA", variables);
    if (!normalColorSet) {
#ifdef _DEBUG
        std::cerr << "Error: Button: no se pudo inicializar el color del estado normal.\n";
#endif
        return false;
    }

    bool hoverColorSet = setValueFromMap(hoverColor.x, "hoverColorR", variables) &&
        setValueFromMap(hoverColor.y, "hoverColorG", variables) &&
        setValueFromMap(hoverColor.z, "hoverColorB", variables) &&
        setValueFromMap(hoverColor.w, "hoverColorA", variables);
    if (!hoverColorSet) {
#ifdef _DEBUG
        std::cerr << "Error: Button: no se pudo inicializar el color del estado hover.\n";
#endif
        return false;
    }

    bool activeColorSet = setValueFromMap(activeColor.x, "activeColorR", variables) &&
        setValueFromMap(activeColor.y, "activeColorG", variables) &&
        setValueFromMap(activeColor.z, "activeColorB", variables) &&
        setValueFromMap(activeColor.w, "activeColorA", variables);
    if (!activeColorSet) {
#ifdef _DEBUG
        std::cerr << "Error: Button: no se pudo inicializar el color del estado active.\n";
#endif
        return false;
    }

    bool windowFlagsSet = setValueFromMap(windowFlags, "windowFlags", variables);
    if (!windowFlagsSet) {
#ifdef _DEBUG
        std::cerr << "Error: Button: no se pudo inicializar windowFlags.\n";
#endif
        return false;
    }

    return true;
}

void Button::awake() {
    setTransform(object->getComponent<Transform>());

    textFont = UIManager::instance()->getFont(textFontName, textSize);
}

void Button::render() const {
    std::string textStr = getText();
    const char* text = textStr.c_str();
    ImVec2 constSize = getSize();
    ImVec2 buttonSize = constSize;
    // Si el tamano es -1, -1, se calcula el tamano del boton en funcion del texto
    if (constSize.x <= -1 && constSize.y <= -1) {
        ImVec2 textSize = ImGui::CalcTextSize(text);
        buttonSize = ImVec2(textSize.x + getPadding().x, textSize.y + getPadding().y);
    }

    // Establece la posicion y el tamano de la ventana de fondo a la correspondiente del boton
    ImGui::SetNextWindowPos(getPosition());
    ImGui::SetNextWindowSize(buttonSize);

    // Establece los estilos de la ventana de fondo, sin borde, sin padding y transparente
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());

    ImGui::Begin(getName().c_str(), getCanCloseWindow(), getWindowFlags());

    ImGui::PopStyleVar(2);   // Pop para WindowBorderSize y WindowPadding

    // Establece la fuente del texto
    ImGui::PushFont(getFont());
    // Establece el color del texto
    ImGui::PushStyleColor(ImGuiCol_Text, getTextColor());
    // Establece los colores del boton en los diferentes estados
    ImGui::PushStyleColor(ImGuiCol_Button, getNormalColor());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, getHoverColor());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, getActiveColor());
    // Establece el ancho de envoltura para el texto del boton
    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + buttonSize.x);
    // -1, -1 para que el boton se ajuste al tamano de la ventana
    if (ImGui::Button(text, ImVec2(-1, -1))) getOnClick()();

    // Pop para el ancho de envoltura
    ImGui::PopTextWrapPos();
    // Pop para WindowBg, los colores de los estados del boton y el color del texto
    ImGui::PopStyleColor(5);
    // Pop para la fuente del texto
    ImGui::PopFont();

    ImGui::End();
}
}