#include "ImageTextButton.h"
#include <imgui.h>
#include "Structure/GameObject.h"
#include "Structure/Scene.h"
#include "UIManager.h"
#include "LuaManager.h"

namespace Tapioca {
ImageTextButton::ImageTextButton()
    : Button(), wantText(false), imagePathNormal(""), imagePathHover(""), imagePathActive(""), textureIdNormal(),
      textureIdHover(), textureIdActive(), uv0(Vector2(0.0f, 0.0f)), uv1(Vector2(1.0f, 1.0f)),
      imageBgColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f)), imageTint(Vector4(1.0f, 1.0f, 1.0f, 1.0f)) { }

bool ImageTextButton::initComponent(const CompMap& variables) {

    if (!Button::initComponent(variables)) return false;

    if (!setValueFromMap(wantText, "wantText", variables)) {
        logInfo(("ImageTextButton: No se encontro el valor de wantText. Se inicializo al valor predefinido: \"" +
                 std::to_string(wantText) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(imagePathNormal, "imagePathNormal", variables)) {
        logError("ImageTextButton: No se pudo inicializar la ruta de la imagen para el estado normal.");
        return false;
    }

    if (!setValueFromMap(imagePathHover, "imagePathHover", variables)) {
        logError("ImageTextButton: No se pudo inicializar la ruta de la imagen para el estado hover.");
        return false;
    }

    if (!setValueFromMap(imagePathActive, "imagePathActive", variables)) {
        logError("ImageTextButton: No se pudo inicializar la ruta de la imagen para el estado active.");
        return false;
    }

    if (!setValueFromMap(uv0.x, "uv0X", variables)) {
        logInfo(("ImageTextButton: No se encontro el valor de uv0X. Se inicializo al predefinido: \"" +
                 std::to_string(uv0.x) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(uv0.y, "uv0Y", variables)) {
        logInfo(("ImageTextButton: No se encontro el valor de uv0Y. Se inicializo al predefinido: \"" +
                 std::to_string(uv0.y) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(uv1.x, "uv1X", variables)) {
        logInfo(("ImageTextButton: No se encontro el valor de uv1X. Se inicializo al predefinido: \"" +
                 std::to_string(uv1.x) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(uv1.y, "uv1Y", variables)) {
        logInfo(("ImageTextButton: No se encontro el valor de uv1Y. Se inicializo al predefinido: \"" +
                 std::to_string(uv1.y) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(imageBgColor.x, "imageBgColorR", variables)) {
        logInfo(("ImageTextButton: No se encontro el valor de imageBgColorR. Se inicializo al predefinido: \"" +
                 std::to_string(imageBgColor.x) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(imageBgColor.y, "imageBgColorG", variables)) {
        logInfo(("ImageTextButton: No se encontro el valor de imageBgColorG. Se inicializo al predefinido: \"" +
                 std::to_string(imageBgColor.y) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(imageBgColor.z, "imageBgColorB", variables)) {
        logInfo(("ImageTextButton: No se encontro el valor de imageBgColorB. Se inicializo al predefinido: \"" +
                 std::to_string(imageBgColor.z) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(imageBgColor.w, "imageBgColorA", variables)) {
        logInfo(("ImageTextButton: No se encontro el valor de imageBgColorA. Se inicializo al predefinido: \"" +
                 std::to_string(imageBgColor.w) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(imageTint.x, "imageTintR", variables)) {
        logInfo(("ImageTextButton: No se encontro el valor de imageTintR. Se inicializo al predefinido: \"" +
                 std::to_string(imageTint.x) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(imageTint.y, "imageTintG", variables)) {
        logInfo(("ImageTextButton: No se encontro el valor de imageTintG. Se inicializo al predefinido: \"" +
                 std::to_string(imageTint.y) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(imageTint.z, "imageTintB", variables)) {
        logInfo(("ImageTextButton: No se encontro el valor de imageTintB. Se inicializo al predefinido: \"" +
                 std::to_string(imageTint.z) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(imageTint.w, "imageTintA", variables)) {
        logInfo(("ImageTextButton: No se encontro el valor de imageTintA. Se inicializo al predefinido: \"" +
                 std::to_string(imageTint.w) + "\".")
                    .c_str());
    }
    return true;
}

void ImageTextButton::start() {
    Button::start();
    updateTextures();
}

void ImageTextButton::render() const {
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

    // Establece los colores del boton en los diferentes estados
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(normalColor.x, normalColor.y, normalColor.z, normalColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(hoverColor.x, hoverColor.y, hoverColor.z, hoverColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(activeColor.x, activeColor.y, activeColor.z, activeColor.w));

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2());

    // Calcula los limites del boton a partir de la posicion del cursor
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    ImVec2 buttonMin = cursorPos;
    ImVec2 buttonMax = ImVec2(cursorPos.x + buttonSize.x, cursorPos.y + buttonSize.y);
    // Comprueba si el cursor esta encima del boton
    bool hovered = ImGui::IsMouseHoveringRect(buttonMin, buttonMax);
    // Comprueba si el cursor esta encima del boton y se ha pulsado el boton izquierdo del raton
    bool active = ImGui::IsMouseDown(0) && hovered;

    // Establece la textura del boton y color del texto en funcion del estado
    ImTextureID textureId = textureIdNormal;
    ImVec4 textColor = ImVec4(textColorNormal.x, textColorNormal.y, textColorNormal.z, textColorNormal.w);
    // Primero hay que comprobar si el boton esta activo, luego si esta encima (hover) y por ultimo si esta normal
    if (active) {
        textureId = textureIdActive;
        textColor = ImVec4(textColorActive.x, textColorActive.y, textColorActive.z, textColorActive.w);
    }
    else if (hovered) {
        textureId = textureIdHover;
        textColor = ImVec4(textColorHover.x, textColorHover.y, textColorHover.z, textColorHover.w);
    }

    if (ImGui::ImageButton(name.c_str(), textureId, buttonSize, ImVec2(uv0.x, uv0.y), ImVec2(uv1.x, uv1.y),
                           ImVec4(imageBgColor.x, imageBgColor.y, imageBgColor.z, imageBgColor.w),
                           ImVec4(imageTint.x, imageTint.y, imageTint.z, imageTint.w)))
        luaManager->callLuaFunction(onClickId);

    if (wantText) {
        std::string textStr = text;
        const char* text = textStr.c_str();

        ImGui::PushFont(textFont);
        ImVec2 textSize = ImGui::CalcTextSize(text);
        ImGui::PopFont();

        ImVec2 textPos =
            ImVec2(buttonPos.x + (buttonSize.x - textSize.x) / 2.0f, buttonPos.y + (buttonSize.y - textSize.y) / 2.0f);

        ImGui::SetCursorScreenPos(textPos);

        ImGui::PushFont(textFont);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(textColor.x, textColor.y, textColor.z, textColor.w));
        ImGui::Text(text);
        ImGui::PopStyleColor();
        ImGui::PopFont();
    }

    // Pop para FramePadding
    ImGui::PopStyleVar();
    // Pop para WindowBg y los colores de los estados del boton
    ImGui::PopStyleColor(4);

    ImGui::End();
}

void ImageTextButton::updateTextures() {
    updateTextureNormal();
    updateTextureHover();
    updateTextureActive();
}

void ImageTextButton::updateTextureNormal() { textureIdNormal = uiManager->getTextureId(imagePathNormal); }
void ImageTextButton::updateTextureHover() { textureIdHover = uiManager->getTextureId(imagePathHover); }
void ImageTextButton::updateTextureActive() { textureIdActive = uiManager->getTextureId(imagePathActive); }
}
