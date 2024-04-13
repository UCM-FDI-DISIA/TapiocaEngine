#include "InputText.h"
#include <imgui.h>
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "UIManager.h"

namespace Tapioca {
InputText::InputText()
    : BaseWidget(), Component(), placeHolderText("Enter text here"), bufferSize(256), buffer(nullptr),
      onTextEnteredId("Debug"), onTextEntered([]() {}), textFontName("arial.ttf"), textSize(16.0f), textFont(nullptr),
      flags(ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue) {
    ImVec4 textColorImVec = ImGui::GetStyle().Colors[ImGuiCol_Text];
    textColor = Vector4(textColorImVec.x, textColorImVec.y, textColorImVec.z, textColorImVec.w);
    ImVec4 bgColorImVec = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
    bgColor = Vector4(bgColorImVec.x, bgColorImVec.y, bgColorImVec.z, bgColorImVec.w);
    createInputTextFunctions();
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
        logError("InputText: No se pudo inicializar el nombre.");
        return false;
    }

    if (!setValueFromMap(bufferSize, "bufferSize", variables)) {
        logInfo("InputText: No se encontro el valor de bufferSize. Se inicializo al valor predefinido.");
    }

    if (!setValueFromMap(placeHolderText, "placeHolderText", variables)) {
        logInfo("InputText: No se encontro el valor de placeHolderText. Se inicializo al valor predefinido.");
    }

    if (!setValueFromMap(onTextEnteredId, "onTextEnteredId", variables)) {
        logInfo("InputText: No se encontro el valor de onTextEnteredId. Se inicializo al valor predefinido.");
    }

    if (!setValueFromMap(textFontName, "textFontName", variables)) {
        logInfo("InputText: No se encontro el valor de textFontName. Se inicializo al valor predefinido.");
    }

    if (!setValueFromMap(textSize, "textSize", variables)) {
        logInfo("InputText: No se encontro el valor de textSize. Se inicializo al valor predefinido.");
    }

    bool textColorSet = setValueFromMap(textColor.x, "textColorR", variables) &&
        setValueFromMap(textColor.y, "textColorG", variables) &&
        setValueFromMap(textColor.z, "textColorB", variables) && setValueFromMap(textColor.w, "textColorA", variables);
    if (!textColorSet) {
        logInfo("InputText: No se encontro el valor de textColor. Se inicializo a los valores predefinidos");
    }

    bool bgColorSet = setValueFromMap(bgColor.x, "bgColorR", variables) &&
        setValueFromMap(bgColor.y, "bgColorG", variables) && setValueFromMap(bgColor.z, "bgColorB", variables) &&
        setValueFromMap(bgColor.w, "bgColorA", variables);
    if (!bgColorSet) {
        logInfo("InputText: No se encontro el valor de bgColor. Se inicializo a los valores predefinidos.");
    }

    if (!setValueFromMap(flags, "flags", variables)) {
        logInfo("InputText: No se encontro el valor de flags. Se inicializo al valor predefinido.");
    }

    if (!setValueFromMap(windowFlags, "windowFlags", variables)) {
        logInfo("InputText: No se encontro el valor de windowFlags. Se inicializo a los valores predefinidos.");
    }

    return true;
}

void InputText::start() {
    setTransform(object->getComponent<Transform>());
    startBuffer();
    updateOnTextEnter();
    updateTextFont();
}

void InputText::render() const {
    float scaleFactorX = uiManager->getScaleFactorX();
    float scaleFactorY = uiManager->getScaleFactorY();

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

    if (ImGui::InputText("##", getBuffer(), getBufferSize(), getFlags())) {
        if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) {
            getOnTextEntered()();
        }
    }

    // Pop para WindowBg, el color del texto y el color de fondo
    ImGui::PopStyleColor(3);
    // Pop para la fuente del texto
    ImGui::PopFont();

    ImGui::End();
}

void InputText::createInputTextFunctions() {
    inputTextFunctions["Debug"] = []() {
#ifdef _DEBUG
        std::cout << "Se ha hecho Enter en la caja de texto\n";
#endif
    };
}

void InputText::updateOnTextEnter() { onTextEntered = inputTextFunctions[onTextEnteredId]; }

void InputText::updateTextFont() { textFont = uiManager->getFont(textFontName, textSize); }

}
