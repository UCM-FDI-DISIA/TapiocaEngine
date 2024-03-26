#include "InputText.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "UIManager.h"

namespace Tapioca {
InputText::InputText()
    : BaseWidget(), Component(), placeHolderText("Enter text here"), bufferSize(256), buffer(nullptr),
      onTextEnteredId(InputTextFunction::INPUT_TEXT_NONE), onTextEntered([]() {}), textFontName("arial.ttf"),
      textSize(16.0f), textFont(nullptr), textColor(ImGui::GetStyle().Colors[ImGuiCol_Text]),
      bgColor(ImGui::GetStyle().Colors[ImGuiCol_FrameBg]),
      flags(ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue) { }

InputText::~InputText() { delete[] buffer; }

void InputText::startBuffer() {
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

void InputText::awake() {
    setTransform(object->getComponent<Transform>());
    startBuffer();

    // ESTA FEO
    switch (onTextEnteredId) {
    case InputTextFunction::INPUT_TEXT_TEXT_ENTERED:
        onTextEntered = [this]() {
            setPlaceHolderText("Se ha hecho ENTER");
            setTextColor(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            setBgColor(ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        };
        break;
    default:
    case InputTextFunction::INPUT_TEXT_NONE:
        onTextEntered = []() {
#ifdef _DEBUG
            std::cout << "No se ha asignado ninguna funcion especial a la caja de texto\n";
#endif
        };
        break;
    }

    textFont = UIManager::instance()->getFont(textFontName, textSize);
}

void InputText::render() const {
    ImVec2 inputTextSize(getSize().x, 0);

    // Establece la posicion y el tamano de la ventana de fondo a la correspondiente del inputText
    ImGui::SetNextWindowPos(getPosition());
    ImGui::SetNextWindowSize(inputTextSize);

    // Establece los estilos de la ventana de fondo, sin borde, sin padding y transparente
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());

    ImGui::Begin(getName().c_str(), nullptr, getWindowFlags());

    ImGui::PopStyleVar(2);   // Pop para WindowBorderSize y WindowPadding

    // Establece la fuente del texto
    ImGui::PushFont(getTextFont());
    // Establece el color del texto
    ImGui::PushStyleColor(ImGuiCol_Text, getTextColor());
    // Establece el color de la caja de texto
    ImGui::PushStyleColor(ImGuiCol_FrameBg, getBgColor());
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
}
