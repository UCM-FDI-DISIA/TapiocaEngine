#include "Text.h"
#include <imgui.h>
#include <limits>
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "UIManager.h"
#include "WindowManager.h"
#include "checkML.h"

namespace Tapioca {
Text::Text() : BaseWidget(), Component(), text("Texto..."), textSize(16.0f), initialTextSize(textSize), textFontName(""), textFont(nullptr) 
{
    ImVec4 textColorImVec = ImGui::GetStyle().Colors[ImGuiCol_Text];
    textColor = Vector4(textColorImVec.x, textColorImVec.y, textColorImVec.z, textColorImVec.w);
}

Text::~Text() { textFont = nullptr; }

bool Text::initComponent(const CompMap& variables) {
    if (!setValueFromMap(name, "name", variables)) {
        logInfo("Text: No se encontro el valor de name. Se inicializo a un valor random");
    }
    uiManager->addWidgetName(name);

    if (!setValueFromMap(text, "text", variables)) {
        logInfo(("Text: No se encontro el valor de text. Se inicializo al valor predefinido: \"" + 
                text + "\".").c_str());
    }

    if (!setValueFromMap(textSize, "textSize", variables)) {
        logInfo(("Text: No se encontro el valor de textSize. Se inicializo al valor predefinido: \"" + 
                 std::to_string(textSize) + "\".").c_str());
    }
    if (textSize <= 0.0f) {
        logError("Text: El valor de textSize no puede ser negativo o cero.");
        return false;
    }

    if (!setValueFromMap(textFontName, "textFontName", variables)) {
        logInfo("Text: No se encontro el valor de textFontName. Se inicializara con la fuente por predefinida.");
    }

    if (!setValueFromMap(textColor.x, "textColorR", variables)) {
        logInfo(("Text: No se encontro el valor de textColorR. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColor.x) + "\".").c_str());
    }

    if (!setValueFromMap(textColor.y, "textColorG", variables)) {
        logInfo(("Text: No se encontro el valor de textColorG. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColor.y) + "\".").c_str());
    }

    if (!setValueFromMap(textColor.z, "textColorB", variables)) {
        logInfo(("Text: No se encontro el valor de textColorB. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColor.z) + "\".").c_str());
    }

    if (!setValueFromMap(textColor.w, "textColorA", variables)) {
        logInfo(("Text: No se encontro el valor de textColorA. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColor.w) + "\".").c_str());
    }

    if (!setValueFromMap(windowFlags, "windowFlags", variables)) {
        logInfo("Text: No se encontro el valor de windowFlags. Se inicializo a los valores predefinidos: "
                "\"ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | "
                "ImGuiWindowFlags_NoSavedSettings\".");
    }
    return true;
}

void Text::start() {
    setTransform(object->getComponent<Transform>());
    float min = std::min(windowManager->getsScaleX(), windowManager->getsScaleY());
    if (min > 0.0f) textSize *= min;
    initialTextSize = textSize;
    updateTextFont();
}

void Text::updateUI() {
    if (!windowManager->getResized()) {
        float min = std::min(windowManager->getsScaleX(), windowManager->getsScaleY());
        if (min > 0.0f) textSize = initialTextSize * min;
        updateTextFont();
    }
}

void Text::render() const {
    std::string textStr = text;
    const char* text = textStr.c_str();

    ImGui::PushFont(textFont);

    ImVec2 calculatedTextSize = ImGui::CalcTextSize(text);

    float scaleX = windowManager->getsScaleX();
    float scaleY = windowManager->getsScaleY();

    float scaledW = scaleX * calculatedTextSize.x;
    float scaledH = scaleY * calculatedTextSize.y;

    float scaledPosX = getPosition().x * scaleX - scaledW / (2.0f * scaleX);
    float scaledPosY = getPosition().y * scaleY - scaledH / (2.0f * scaleY);

    ImVec2 textPos(scaledPosX, scaledPosY);

    ImGui::SetNextWindowPos(textPos);
    ImGui::SetNextWindowSize(calculatedTextSize);

    // Establece los estilos de la ventana de fondo, sin borde, sin padding y transparente
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());

    ImGui::Begin(name.c_str(), nullptr, windowFlags);

    // Pop para WindowBorderSize y WindowPadding
    ImGui::PopStyleVar(2);

    // Establece el color del texto
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(textColor.x, textColor.y, textColor.z, textColor.w));

    // Imprime el texto
    ImGui::Text(text);

    // Pop para WindowBg, el color del texto
    ImGui::PopStyleColor(2);

    ImGui::End();

    // Pop para la fuente del texto
    ImGui::PopFont();
}

void Text::updateTextFont() { textFont = uiManager->getFont(textFontName, textSize); }
}
