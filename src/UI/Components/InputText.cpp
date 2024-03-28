#include "InputText.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "UIManager.h"

namespace Tapioca {
InputText::InputText()
    : BaseWidget(), Component(), placeHolderText("Enter text here"), bufferSize(256), buffer(nullptr),
      onTextEnteredId(InputTextFunction::INPUT_TEXT_NONE), onTextEntered([]() {}), textFontName("arial.ttf"),
      textSize(16.0f), textFont(nullptr),
      flags(ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue) {
    ImVec4 textColorImVec = ImGui::GetStyle().Colors[ImGuiCol_Text];
    textColor = Vector4(textColorImVec.x, textColorImVec.y, textColorImVec.z, textColorImVec.w);
    ImVec4 bgColorImVec = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
    bgColor = Vector4(bgColorImVec.x, bgColorImVec.y, bgColorImVec.z, bgColorImVec.w);
    createInputTextFunctions();
}

InputText::~InputText() { delete[] buffer; }

void InputText::startBuffer() {
    if (buffer != nullptr) {
        delete[] buffer;
    }
    buffer = new char[bufferSize];
    strcpy_s(buffer, bufferSize, placeHolderText.c_str());
}

bool InputText::initComponent(const CompMap& variables) {
    bool nameSet = setValueFromMap(name, "name", variables);
    if (!nameSet) {
#ifdef _DEBUG
        std::cerr << "Error: Input Text: no se pudo inicializar el nombre.\n";
#endif
        return false;
    }

    bool bufferSizeSet = setValueFromMap(bufferSize, "bufferSize", variables);
    if (!bufferSizeSet) {
#ifdef _DEBUG
        std::cout << "Input Text: no se encontro el valor de bufferSize. Se inicializo al valor predefinido\n";
#endif
    }

    bool placeHolderTextSet = setValueFromMap(placeHolderText, "placeHolderText", variables);
    if (!placeHolderTextSet) {
#ifdef _DEBUG
        std::cout << "Input Text: no se encontro el valor de placeHolderText. Se inicializo al valor predefinido\n";
#endif
    }

    bool onTextEnteredIdSet = setValueFromMap(onTextEnteredId, "onTextEnteredId", variables);
    if (!onTextEnteredIdSet) {
#ifdef _DEBUG
        std::cout << "Input Text: no se encontro el valor de onTextEnteredId. Se inicializo al valor predefinido\n";
#endif
    }

    bool textFontNameSet = setValueFromMap(textFontName, "textFontName", variables);
    if (!textFontNameSet) {
#ifdef _DEBUG
        std::cout << "Input Text: no se encontro el valor de textFontName. Se inicializo al valor predefinido\n";
#endif
    }

    bool textSizeSet = setValueFromMap(textSize, "textSize", variables);
    if (!textSizeSet) {
#ifdef _DEBUG
        std::cout << "Input Text: no se encontro el valor de textSize. Se inicializo al valor predefinido\n";
#endif
    }

    bool textColorSet = setValueFromMap(textColor.x, "textColorR", variables) &&
        setValueFromMap(textColor.y, "textColorG", variables) &&
        setValueFromMap(textColor.z, "textColorB", variables) && setValueFromMap(textColor.w, "textColorA", variables);
    if (!textColorSet) {
#ifdef _DEBUG
        std::cout << "Input Text : no se encontro el valor de textColor. Se inicializo a los valores predefinidos\n ";
#endif
    }

    bool bgColorSet = setValueFromMap(bgColor.x, "bgColorR", variables) &&
        setValueFromMap(bgColor.y, "bgColorG", variables) && setValueFromMap(bgColor.z, "bgColorB", variables) &&
        setValueFromMap(bgColor.w, "bgColorA", variables);
    if (!bgColorSet) {
#ifdef _DEBUG
        std::cout << "Input Text : no se encontro el valor de bgColor. Se inicializo a los valores predefinidos\n ";
#endif
    }

    bool flagsSet = setValueFromMap(flags, "flags", variables);
    if (!flagsSet) {
#ifdef _DEBUG
        std::cout << "Input Text: no se encontro el valor de flags. Se inicializo al valor predefinido\n ";
#endif
    }

    bool windowFlagsSet = setValueFromMap(windowFlags, "windowFlags", variables);
    if (!windowFlagsSet) {
#ifdef _DEBUG
        std::cout << "Input Text: no se encontro el valor de windowFlags. Se inicializo a los valores predefinidos\n";
#endif
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
    ImVec2 inputTextSize(getSize().x, 0);

    // Establece la posicion y el tamano de la ventana de fondo a la correspondiente del inputText
    ImGui::SetNextWindowPos(ImVec2(getPosition().x, getPosition().y));
    ImGui::SetNextWindowSize(inputTextSize);

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
    // Establece el color de la caja de texto
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(bgColor.x, bgColor.y, bgColor.z, bgColor.w));
    // Establece el ancho de envoltura para el texto del inputText
    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + inputTextSize.x);

    if (ImGui::InputText("##", getBuffer(), getBufferSize(), getFlags())) {
        if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) {
            getOnTextEntered()();
        }
    }

    // Pop para el ancho de envoltura
    ImGui::PopTextWrapPos();
    // Pop para WindowBg, el color del texto y el color de fondo
    ImGui::PopStyleColor(3);
    // Pop para la fuente del texto
    ImGui::PopFont();

    ImGui::End();
}

void InputText::createInputTextFunctions() {

    inputTextFunctions[InputTextFunction::INPUT_TEXT_NONE] = []() {
#ifdef _DEBUG
        std::cout << "No se ha asignado ninguna funcion especial a la caja de texto\n";
#endif
    };

    inputTextFunctions[InputTextFunction::INPUT_TEXT_TEXT_ENTERED] = [this]() {
        setPlaceHolderText("Se ha hecho ENTER");
        setTextColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        setBgColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
    };
}

void InputText::updateOnTextEnter() { onTextEntered = inputTextFunctions[onTextEnteredId]; }

void InputText::updateTextFont() { textFont = UIManager::instance()->getFont(textFontName, textSize); }

}
