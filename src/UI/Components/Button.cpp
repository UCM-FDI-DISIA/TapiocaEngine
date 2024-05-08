#include "Button.h"
#include <imgui.h>
#include "Structure/GameObject.h"
#include "Structure/Scene.h"
#include "Components/Transform.h"
#include "UIManager.h"
#include "LuaManager.h"
#include "WindowManager.h"
#include "checkML.h"

namespace Tapioca {
Button::Button() : BaseWidget(), Component(), onClickId(""), text("Button"), 
    textFont(nullptr), textFontName(""), textSize(16.0f), initialTextSize(16.0f) 
{
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
        logInfo("Button: No se encontro el valor de name. Se inicializo a un valor random");
    }
    uiManager->addWidgetName(name);

    if (!setValueFromMap(onClickId, "onClickId", variables)) {
        logInfo(("Button: No se encontro el valor de onClick. Se inicializo al valor predefinido: \"" + onClickId + "\".").c_str());
    }

    if (!setValueFromMap(text, "text", variables)) {
        logInfo(("Button: No se encontro el valor de text. Se inicializo al valor predefinido: \"" + text + "\".").c_str());
    }

    if (!setValueFromMap(textFontName, "textFontName", variables)) {
        logInfo("Button: No se encontro el valor de textFontName. Se inicializara con la fuente por predefinida.");
    }

    if (!setValueFromMap(textSize, "textSize", variables)) {
        logInfo(("Button: No se encontro el valor de textSize. Se inicializo al valor predefinido: \"" +
                 std::to_string(textSize) + "\".").c_str());
    }

    if (!setValueFromMap(textColorNormal.x, "textColorNormalR", variables)) {
        logInfo(("Button: No se encontro el valor de textColorNormalR. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColorNormal.x) + "\".").c_str());
    }

    if (!setValueFromMap(textColorNormal.y, "textColorNormalG", variables)) {
        logInfo(("Button: No se encontro el valor de textColorNormalG. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColorNormal.y) + "\".").c_str());
    }

    if (!setValueFromMap(textColorNormal.z, "textColorNormalB", variables)) {
        logInfo(("Button: No se encontro el valor de textColorNormalB. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColorNormal.z) + "\".").c_str());
    }

    if (!setValueFromMap(textColorNormal.w, "textColorNormalA", variables)) {
        logInfo(("Button: No se encontro el valor de textColorNormalA. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColorNormal.w) + "\".").c_str());
    }

    if (!setValueFromMap(textColorHover.x, "textColorHoverR", variables)) {
        logInfo(("Button: No se encontro el valor de textColorHoverR. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColorHover.x) + "\".").c_str());
    }

    if (!setValueFromMap(textColorHover.y, "textColorHoverG", variables)) {
        logInfo(("Button: No se encontro el valor de textColorHoverG. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColorHover.y) + "\".").c_str());
    }

    if (!setValueFromMap(textColorHover.z, "textColorHoverB", variables)) {
        logInfo(("Button: No se encontro el valor de textColorHoverB. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColorHover.z) + "\".").c_str());
    }

    if (!setValueFromMap(textColorHover.w, "textColorHoverA", variables)) {
        logInfo(("Button: No se encontro el valor de textColorHoverA. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColorHover.w) + "\".").c_str());
    }

    if (!setValueFromMap(textColorActive.x, "textColorActiveR", variables)) {
        logInfo(("Button: No se encontro el valor de textColorActiveR. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColorActive.x) + "\".").c_str());
    }

    if (!setValueFromMap(textColorActive.y, "textColorActiveG", variables)) {
        logInfo(("Button: No se encontro el valor de textColorActiveG. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColorActive.y) + "\".").c_str());
    }

    if (!setValueFromMap(textColorActive.z, "textColorActiveB", variables)) {
        logInfo(("Button: No se encontro el valor de textColorActiveB. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColorActive.z) + "\".").c_str());
    }

    if (!setValueFromMap(textColorActive.w, "textColorActiveA", variables)) {
        logInfo(("Button: No se encontro el valor de textColorActiveA. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColorActive.w) + "\".").c_str());
    }

    if (!setValueFromMap(normalColor.x, "normalColorR", variables)) {
        logInfo(("Button: No se encontro el valor de normalColorR. Se inicializo al valor predefinido: \"" +
                 std::to_string(normalColor.x) + "\".").c_str());
    }

    if (!setValueFromMap(normalColor.y, "normalColorG", variables)) {
        logInfo(("Button: No se encontro el valor de normalColorG. Se inicializo al valor predefinido: \"" +
                 std::to_string(normalColor.y) + "\".").c_str());
    }

    if (!setValueFromMap(normalColor.z, "normalColorB", variables)) {
        logInfo(("Button: No se encontro el valor de normalColorB. Se inicializo al valor predefinido: \"" +
                 std::to_string(normalColor.z) + "\".").c_str());
    }

    if (!setValueFromMap(normalColor.w, "normalColorA", variables)) {
        logInfo(("Button: No se encontro el valor de normalColorA. Se inicializo al valor predefinido: \"" +
                 std::to_string(normalColor.w) + "\".").c_str());
    }

    if (!setValueFromMap(hoverColor.x, "hoverColorR", variables)) {
        logInfo(("Button: No se encontro el valor de hoverColorR. Se inicializo al valor predefinido: \"" +
                 std::to_string(hoverColor.x) + "\".").c_str());
    }

    if (!setValueFromMap(hoverColor.y, "hoverColorG", variables)) {
        logInfo(("Button: No se encontro el valor de hoverColorG. Se inicializo al valor predefinido: \"" +
                 std::to_string(hoverColor.y) + "\".").c_str());
    }

    if (!setValueFromMap(hoverColor.z, "hoverColorB", variables)) {
        logInfo(("Button: No se encontro el valor de hoverColorB. Se inicializo al valor predefinido: \"" +
                 std::to_string(hoverColor.z) + "\".").c_str());
    }

    if (!setValueFromMap(hoverColor.w, "hoverColorA", variables)) {
        logInfo(("Button: No se encontro el valor de hoverColorA. Se inicializo al valor predefinido: \"" +
                 std::to_string(hoverColor.w) + "\".").c_str());
    }

    if (!setValueFromMap(activeColor.x, "activeColorR", variables)) {
        logInfo(("Button: No se encontro el valor de activeColorR. Se inicializo al valor predefinido: \"" +
                 std::to_string(activeColor.x) + "\".").c_str());
    }

    if (!setValueFromMap(activeColor.y, "activeColorG", variables)) {
        logInfo(("Button: No se encontro el valor de activeColorG. Se inicializo al valor predefinido: \"" +
                 std::to_string(activeColor.y) + "\".").c_str());
    }

    if (!setValueFromMap(activeColor.z, "activeColorB", variables)) {
        logInfo(("Button: No se encontro el valor de activeColorB. Se inicializo al valor predefinido: \"" +
                 std::to_string(activeColor.z) + "\".").c_str());
    }

    if (!setValueFromMap(activeColor.w, "activeColorA", variables)) {
        logInfo(("Button: No se encontro el valor de activeColorA. Se inicializo al valor predefinido: \"" +
                 std::to_string(activeColor.w) + "\".").c_str());
    }

    if (!setValueFromMap(windowFlags, "windowFlags", variables)) {
        logInfo("Button: No se encontro el valor de windowFlags. Se inicializo a los valores predefinidos: "
                "\"ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | "
                "ImGuiWindowFlags_NoSavedSettings\".");
    }
    return true;
}

void Button::start() {
    setTransform(object->getComponent<Transform>());
    if (object->getScene()->getFirstWindowW() != windowManager->getFirstWindowW() ||
        object->getScene()->getFirstWindowH() != windowManager->getFirstWindowH()) {
        float min = std::min((float)object->getScene()->getFirstWindowW() / (float)windowManager->getFirstWindowW(),
                             (float)object->getScene()->getFirstWindowH() / (float)windowManager->getFirstWindowH());
        if (min > 0.0f) {
            textSize *= min;
            transform->setScaleXY(Vector2(transform->getScale().x * min, transform->getScale().y * min));
        }
    }
    initialTextSize = textSize;
    updateTextFont();
}

void Button::updateUI() {
    if (!windowManager->getResized()) {
        float scaleFactorX = object->getScene()->getScaleFactorX();
        float scaleFactorY = object->getScene()->getScaleFactorY();
        float min = std::min(scaleFactorX, scaleFactorY);
        if (min > 0.0f) textSize = initialTextSize * min;
        updateTextFont();
    }
}

void Button::render() const {
    std::string textStr = text;
    const char* text = textStr.c_str();

    float scaleFactorX = object->getScene()->getScaleFactorX();
    float scaleFactorY = object->getScene()->getScaleFactorY();

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
    else if (hovered) textColor = ImVec4(textColorHover.x, textColorHover.y, textColorHover.z, textColorHover.w);

    // Establece la fuente del texto
    ImGui::PushFont(textFont);
    // Establece el color del texto
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    // Establece los colores del boton en los diferentes estados
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(normalColor.x, normalColor.y, normalColor.z, normalColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(hoverColor.x, hoverColor.y, hoverColor.z, hoverColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(activeColor.x, activeColor.y, activeColor.z, activeColor.w));

    if (ImGui::Button(text, buttonSize)) luaManager->callLuaFunction(onClickId);

    // Pop para WindowBg, los colores de los estados del boton y el color del texto
    ImGui::PopStyleColor(5);
    // Pop para la fuente del texto
    ImGui::PopFont();

    ImGui::End();
}

void Button::updateTextFont() { textFont = uiManager->getFont(textFontName, textSize); }
}
