#include "Button.h"
#include <imgui.h>
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "UIManager.h"
#include "Structure/DynamicLibraryLoader.h"

namespace Tapioca {
Button::Button()
    : BaseWidget(), Component(), text("Button"), onClickId("Debug"), onClick([]() {}),
      textFont(nullptr), textFontName("arial.ttf"), textSize(16.0f) {
    ImVec4 textColorImVec = ImGui::GetStyle().Colors[ImGuiCol_Text];
    textColor = Vector4(textColorImVec.x, textColorImVec.y, textColorImVec.z, textColorImVec.w);
    ImVec4 normalColorImVec = ImGui::GetStyle().Colors[ImGuiCol_Button];
    normalColor = Vector4(normalColorImVec.x, normalColorImVec.y, normalColorImVec.z, normalColorImVec.w);
    ImVec4 hoverColorImVec = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
    hoverColor = Vector4(hoverColorImVec.x, hoverColorImVec.y, hoverColorImVec.z, hoverColorImVec.w);
    ImVec4 activeColorImVec = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
    activeColor = Vector4(activeColorImVec.x, activeColorImVec.y, activeColorImVec.z, activeColorImVec.w);
    createButtonFunctions();
}

Button::~Button() { textFont = nullptr; }

bool Button::initComponent(const CompMap& variables) {
    if (!setValueFromMap(name, "name", variables)) {
        logError("Button: No se pudo inicializar el nombre.");
        return false;
    }

    if (!setValueFromMap(text, "text", variables)) {
        logInfo("Button: No se encontro el valor de text. Se inicializo al valor predefinido.");
    }

    if (!setValueFromMap(onClickId, "onClickId", variables)) {
        logInfo("Button: No se encontro el valor de onClickId. Se inicializo al valor predefinido.");
    }

    if (!setValueFromMap(textFontName, "textFontName", variables)) {
        logInfo("Button: No se encontro el valor de textFontName. Se inicializo al valor predefinido.");
    }

    if (!setValueFromMap(textSize, "textSize", variables)) {
        logInfo("Button: No se encontro el valor de textSize. Se inicializo al valor predefinido.");
    }

    bool textColorSet = setValueFromMap(textColor.x, "textColorR", variables) &&
        setValueFromMap(textColor.y, "textColorG", variables) &&
        setValueFromMap(textColor.z, "textColorB", variables) && setValueFromMap(textColor.w, "textColorA", variables);
    if (!textColorSet) {
        logInfo("Button: No se encontro el valor de textColor. Se inicializo a los valores predefinidos.");
    }

    bool normalColorSet = setValueFromMap(normalColor.x, "normalColorR", variables) &&
        setValueFromMap(normalColor.y, "normalColorG", variables) &&
        setValueFromMap(normalColor.z, "normalColorB", variables) &&
        setValueFromMap(normalColor.w, "normalColorA", variables);
    if (!normalColorSet) {
        logInfo("Button: No se encontro el valor de normalColor. Se inicializo a los valores predefinidos.");
    }

    bool hoverColorSet = setValueFromMap(hoverColor.x, "hoverColorR", variables) &&
        setValueFromMap(hoverColor.y, "hoverColorG", variables) &&
        setValueFromMap(hoverColor.z, "hoverColorB", variables) &&
        setValueFromMap(hoverColor.w, "hoverColorA", variables);
    if (!hoverColorSet) {
        logInfo("Button: No se encontro el valor de hoverColor. Se inicializo a los valores predefinidos.");
    }

    bool activeColorSet = setValueFromMap(activeColor.x, "activeColorR", variables) &&
        setValueFromMap(activeColor.y, "activeColorG", variables) &&
        setValueFromMap(activeColor.z, "activeColorB", variables) &&
        setValueFromMap(activeColor.w, "activeColorA", variables);
    if (!activeColorSet) {
        logInfo("Button: No se encontro el valor de activeColor. Se inicializo a los valores predefinidos.");
    }

    if (!setValueFromMap(windowFlags, "windowFlags", variables)) {
        logInfo("Button: No se encontro el valor de windowFlags. Se inicializo a los valores predefinidos.");
    }

    return true;
}

void Button::start() {
    setTransform(object->getComponent<Transform>());
    updateOnClick();
    updateTextFont();
}

void Button::render() const {
    std::string textStr = text;
    const char* text = textStr.c_str();

    float scaleFactorX = uiManager->getScaleFactorX();
    float scaleFactorY = uiManager->getScaleFactorY();

    ImVec2 buttonSize(getSize().x * scaleFactorX, getSize().y * scaleFactorY);
    ImVec2 buttonPos(getPosition().x * scaleFactorX - buttonSize.x / 2.0f,
                     getPosition().y * scaleFactorY - buttonSize.y / 2.0f);

    // Establece la posicion y el tamano de la ventana de fondo a la correspondiente del boton
    ImGui::SetNextWindowPos(buttonPos);
    ImGui::SetNextWindowSize(buttonSize);

    // Establece los estilos de la ventana de fondo, sin borde, sin padding y transparente
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());

    ImGui::Begin(name.c_str(), nullptr, windowFlags);

    ImGui::PopStyleVar(2);   // Pop para WindowBorderSize y WindowPadding

    // Establece la fuente del texto
    ImGui::PushFont(textFont);
    // Establece el color del texto
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(textColor.x, textColor.y, textColor.z, textColor.w));
    // Establece los colores del boton en los diferentes estados
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(normalColor.x, normalColor.y, normalColor.z, normalColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(hoverColor.x, hoverColor.y, hoverColor.z, hoverColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(activeColor.x, activeColor.y, activeColor.z, activeColor.w));
    // Establece el ancho de envoltura para el texto del boton
    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + buttonSize.x);
    // -1, -1 para que el boton se ajuste al tamano de la ventana
    if (ImGui::Button(text, buttonSize)) onClick();

    // Pop para el ancho de envoltura
    ImGui::PopTextWrapPos();
    // Pop para WindowBg, los colores de los estados del boton y el color del texto
    ImGui::PopStyleColor(5);
    // Pop para la fuente del texto
    ImGui::PopFont();

    ImGui::End();
}

void Button::createButtonFunctions() {
    buttonFunctions["Debug"] = []() {
#ifdef _DEBUG
        std::cout << "Se ha hecho click en el boton!\n";
#endif
    };
}

void Button::updateOnClick() { onClick = buttonFunctions[onClickId]; }

void Button::updateTextFont() { textFont = uiManager->getFont(textFontName, textSize); }
}