#include "Button.h"
#include <imgui.h>
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "UIManager.h"
#include "Structure/DynamicLibraryLoader.h"

namespace Tapioca {
Button::Button()
    : BaseWidget(), Component(), text("Button"), onClickId("Debug"), onClick([]() {}), textFont(nullptr),
      textFontName("arial.ttf"), textSize(16.0f) {
    ImVec4 textColorImVec = ImGui::GetStyle().Colors[ImGuiCol_Text];
    textColorNormal = Vector4(textColorImVec.x, textColorImVec.y, textColorImVec.z, textColorImVec.w);
    textColorHover = Vector4(textColorImVec.x, textColorImVec.y, textColorImVec.z, textColorImVec.w);
    textColorActive = Vector4(textColorImVec.x, textColorImVec.y, textColorImVec.z, textColorImVec.w);
    ImVec4 normalColorImVec = ImGui::GetStyle().Colors[ImGuiCol_Button];
    normalColor = Vector4(normalColorImVec.x, normalColorImVec.y, normalColorImVec.z, normalColorImVec.w);
    ImVec4 hoverColorImVec = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
    hoverColor = Vector4(hoverColorImVec.x, hoverColorImVec.y, hoverColorImVec.z, hoverColorImVec.w);
    ImVec4 activeColorImVec = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
    activeColor = Vector4(activeColorImVec.x, activeColorImVec.y, activeColorImVec.z, activeColorImVec.w);
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

    bool textColorNormalSet = setValueFromMap(textColorNormal.x, "textColorNormalR", variables) &&
        setValueFromMap(textColorNormal.y, "textColorNormalG", variables) &&
        setValueFromMap(textColorNormal.z, "textColorNormalB", variables) &&
        setValueFromMap(textColorNormal.w, "textColorNormalA", variables);
    if (!textColorNormalSet) {
        logInfo("Button: No se encontro el valor de textColorNormal. Se inicializo a los valores predefinidos.");
    }

    bool textColorHoverSet = setValueFromMap(textColorHover.x, "textColorHoverR", variables) &&
        setValueFromMap(textColorHover.y, "textColorHoverG", variables) &&
        setValueFromMap(textColorHover.z, "textColorHoverB", variables) &&
        setValueFromMap(textColorHover.w, "textColorHoverA", variables);
    if (!textColorHoverSet) {
        logInfo("Button: No se encontro el valor de textColorHover. Se inicializo a los valores predefinidos.");
    }

    bool textColorActiveSet = setValueFromMap(textColorActive.x, "textColorActiveR", variables) &&
        setValueFromMap(textColorActive.y, "textColorActiveG", variables) &&
        setValueFromMap(textColorActive.z, "textColorActiveB", variables) &&
        setValueFromMap(textColorActive.w, "textColorActiveA", variables);
    if (!textColorActiveSet) {
        logInfo("Button: No se encontro el valor de textColorActive. Se inicializo a los valores predefinidos.");
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

    // Pop para WindowBorderSize y WindowPadding
    ImGui::PopStyleVar(2);

    // Calcula los limites del boton a partir de la posicion del cursor
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    ImVec2 buttonMin = cursorPos;
    ImVec2 buttonMax = ImVec2(cursorPos.x + buttonSize.x, cursorPos.y + buttonSize.y);
    // Comprueba si el cursor esta encima del boton
    bool hovered = ImGui::IsMouseHoveringRect(buttonMin, buttonMax);
    // Comprueba si el cursor esta encima del boton y se ha pulsado el boton izquierdo del raton
    bool active = ImGui::IsMouseDown(0) && hovered;

    // Establece la textura del boton en funcion del estado
    ImVec4 textColor = ImVec4(textColorNormal.x, textColorNormal.y, textColorNormal.z, textColorNormal.w);

    // Primero hay que comprobar si el boton esta activo, luego si esta encima (hover) y por ultimo si esta normal
    if (active) textColor = ImVec4(textColorActive.x, textColorActive.y, textColorActive.z, textColorActive.w);
    else if (hovered)
        textColor = ImVec4(textColorHover.x, textColorHover.y, textColorHover.z, textColorHover.w);

    // Establece la fuente del texto
    ImGui::PushFont(textFont);
    // Establece el color del texto
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    // Establece los colores del boton en los diferentes estados
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(normalColor.x, normalColor.y, normalColor.z, normalColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(hoverColor.x, hoverColor.y, hoverColor.z, hoverColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(activeColor.x, activeColor.y, activeColor.z, activeColor.w));

    if (ImGui::Button(text, buttonSize)) onClick();

    // Pop para WindowBg, los colores de los estados del boton y el color del texto
    ImGui::PopStyleColor(5);
    // Pop para la fuente del texto
    ImGui::PopFont();

    ImGui::End();
}

void Button::updateOnClick() { onClick = uiManager->getFunction(onClickId); }

void Button::updateTextFont() { textFont = uiManager->getFont(textFontName, textSize); }
}