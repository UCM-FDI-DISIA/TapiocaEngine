#include "InputText.h"
#include <imgui.h>
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "Structure/Scene.h"
#include "UIManager.h"
#include "LuaManager.h"
#include "WindowManager.h"

namespace Tapioca {
InputText::InputText()
    : BaseWidget(), Component(), placeHolderText("Enter text here"), bufferSize(256), buffer(nullptr),
      onTextEnteredId(""), textFontName(""), textSize(16.0f), textFont(nullptr),
      flags(ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue) {
    ImVec4 textColorImVec = ImGui::GetStyle().Colors[ImGuiCol_Text];
    textColor = Vector4(textColorImVec.x, textColorImVec.y, textColorImVec.z, textColorImVec.w);
    ImVec4 bgColorImVec = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
    bgColor = Vector4(bgColorImVec.x, bgColorImVec.y, bgColorImVec.z, bgColorImVec.w);
}

InputText::~InputText() {
    delete[] buffer;
    buffer = nullptr;
    textFont = nullptr;
}

void InputText::startBuffer() {
    if (buffer != nullptr) {
        delete[] buffer;
    }
    buffer = new char[bufferSize];
    strcpy_s(buffer, bufferSize, placeHolderText.c_str());
}

bool InputText::initComponent(const CompMap& variables) {
    if (!setValueFromMap(name, "name", variables)) {
        logInfo("InputText: No se encontro el valor de name. Se inicializo a un valor random");
    }
    uiManager->addWidgetName(name);

    if (!setValueFromMap(bufferSize, "bufferSize", variables)) {
        logInfo(("InputText: No se encontro el valor de bufferSize. Se inicializo al valor predefinido: \"" +
                 std::to_string(bufferSize) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(onTextEnteredId, "onTextEnteredId", variables)) {
        logInfo(("InputText: No se encontro el valor de onTextEnteredId. Se inicializo al valor predefinido: \"" +
                 onTextEnteredId + "\".")
                    .c_str());
    }

    if (!setValueFromMap(placeHolderText, "placeHolderText", variables)) {
        logInfo(("InputText: No se encontro el valor de placeHolderText. Se inicializo al valor predefinido: \"" +
                 placeHolderText + "\.")
                    .c_str());
    }

    if (!setValueFromMap(textFontName, "textFontName", variables)) {
        logInfo("InputText: No se encontro el valor de textFontName. Se inicializara con la fuente por predefinida.");
    }

    if (!setValueFromMap(textSize, "textSize", variables)) {
        logInfo(("InputText: No se encontro el valor de textSize. Se inicializo al valor predefinido: \"" +
                 std::to_string(textSize) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(textColor.x, "textColorR", variables)) {
        logInfo(("InputText: No se encontro el valor de textColorR. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColor.x) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(textColor.y, "textColorG", variables)) {
        logInfo(("InputText: No se encontro el valor de textColorG. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColor.y) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(textColor.z, "textColorB", variables)) {
        logInfo(("InputText: No se encontro el valor de textColorB. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColor.z) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(textColor.w, "textColorA", variables)) {
        logInfo(("InputText: No se encontro el valor de textColorA. Se inicializo al valor predefinido: \"" +
                 std::to_string(textColor.w) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(bgColor.x, "bgColorR", variables)) {
        logInfo(("InputText: No se encontro el valor de bgColorR. Se inicializo al valor predefinido: \"" +
                 std::to_string(bgColor.x) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(bgColor.y, "bgColorG", variables)) {
        logInfo(("InputText: No se encontro el valor de bgColorG. Se inicializo al valor predefinido: \"" +
                 std::to_string(bgColor.y) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(bgColor.z, "bgColorB", variables)) {
        logInfo(("InputText: No se encontro el valor de bgColorB. Se inicializo al valor predefinido: \"" +
                 std::to_string(bgColor.z) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(bgColor.w, "bgColorA", variables)) {
        logInfo(("InputText: No se encontro el valor de bgColorA. Se inicializo al valor predefinido: \"" +
                 std::to_string(bgColor.w) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(flags, "flags", variables)) {
        logInfo("InputText: No se encontro el valor de flags. Se inicializo al valor predefinido: "
                "\"ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue\".");
    }

    if (!setValueFromMap(windowFlags, "windowFlags", variables)) {
        logInfo("InputText: No se encontro el valor de windowFlags. Se inicializo a los valores predefinidos: "
                "\"ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | "
                "ImGuiWindowFlags_NoSavedSettings\".");
    }
    return true;
}

void InputText::start() {
    setTransform(object->getComponent<Transform>());
    if (object->getScene()->getFirstWindowW() != windowManager->getFirstWindowW() ||
        object->getScene()->getFirstWindowH() != windowManager->getFirstWindowH()) {
        float min = std::min((float)object->getScene()->getFirstWindowW() / (float)windowManager->getFirstWindowW(),
                             (float)object->getScene()->getFirstWindowH() / (float)windowManager->getFirstWindowH());
        if (min > 0.0f) transform->setScaleXY(Vector2(transform->getScale().x * min, transform->getScale().y * min));
    }
    startBuffer();
    updateTextFont();
}

void InputText::render() const {
    float scaleFactorX = object->getScene()->getScaleFactorX();
    float scaleFactorY = object->getScene()->getScaleFactorY();

    ImVec2 inputTextSize(getSize().x * scaleFactorX, getSize().y * scaleFactorY);
    ImVec2 inputTextPos(getPosition().x * scaleFactorX - inputTextSize.x / 2.0f,
                        getPosition().y * scaleFactorY - inputTextSize.y / 2.0f);

    ImGui::SetNextWindowPos(inputTextPos);
    ImGui::SetNextWindowSize(inputTextSize);

    // Establece los estilos de la ventana de fondo, sin borde, sin padding y transparente
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());

    ImGui::Begin(name.c_str(), nullptr, windowFlags);

    // Pop para WindowBorderSize y WindowPadding
    ImGui::PopStyleVar(2);

    // Establece la fuente del texto
    ImGui::PushFont(textFont);
    // Establece el color del texto
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(textColor.x, textColor.y, textColor.z, textColor.w));
    // Establece el color de la caja de texto
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(bgColor.x, bgColor.y, bgColor.z, bgColor.w));

    if (ImGui::InputText("##", buffer, bufferSize, flags)) {
        if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) {
            luaManager->callLuaFunction(onTextEnteredId);
        }
    }

    // Si el buffer esta vacio, se pone el texto del placeholder
    if (buffer[0] == '\0') strcpy_s(buffer, bufferSize, placeHolderText.c_str());

    // Pop para WindowBg, el color del texto y el color de fondo
    ImGui::PopStyleColor(3);
    // Pop para la fuente del texto
    ImGui::PopFont();

    ImGui::End();
}

void InputText::updateTextFont() { textFont = uiManager->getFont(textFontName, textSize); }
}
