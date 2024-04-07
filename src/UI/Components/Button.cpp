#include "Button.h"
#include <imgui.h>
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "UIManager.h"
#include "Structure/DynamicLibraryLoader.h"

namespace Tapioca {
Button::Button()
    : BaseWidget(), Component(), text("Button"), onClickId(ButtonFunction::BUTTON_NONE), onClick([]() {}),
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
#ifdef _DEBUG
        std::cerr << "Error: Button: no se pudo inicializar el nombre.\n";
#endif
        return false;
    }

    if (!setValueFromMap(text, "text", variables)) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de text. Se inicializo al valor predefinido\n";
#endif
    }

    if (!setValueFromMap(onClickId, "onClickId", variables)) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de onClickId. Se inicializo al valor predefinido\n";
#endif
    }

    if (!setValueFromMap(textFontName, "textFontName", variables)) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de textFontName. Se inicializo al valor predefinido\n";
#endif
    }

    if (!setValueFromMap(textSize, "textSize", variables)) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de textSize. Se inicializo al valor predefinido\n";
#endif
    }

    bool textColorSet = setValueFromMap(textColor.x, "textColorR", variables) &&
        setValueFromMap(textColor.y, "textColorG", variables) &&
        setValueFromMap(textColor.z, "textColorB", variables) && setValueFromMap(textColor.w, "textColorA", variables);
    if (!textColorSet) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de textColor. Se inicializo a los valores predefinidos\n";
#endif
    }

    bool normalColorSet = setValueFromMap(normalColor.x, "normalColorR", variables) &&
        setValueFromMap(normalColor.y, "normalColorG", variables) &&
        setValueFromMap(normalColor.z, "normalColorB", variables) &&
        setValueFromMap(normalColor.w, "normalColorA", variables);
    if (!normalColorSet) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de normalColor. Se inicializo a los valores predefinidos\n";
#endif
    }

    bool hoverColorSet = setValueFromMap(hoverColor.x, "hoverColorR", variables) &&
        setValueFromMap(hoverColor.y, "hoverColorG", variables) &&
        setValueFromMap(hoverColor.z, "hoverColorB", variables) &&
        setValueFromMap(hoverColor.w, "hoverColorA", variables);
    if (!hoverColorSet) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de hoverColor. Se inicializo a los valores predefinidos\n";
#endif
    }

    bool activeColorSet = setValueFromMap(activeColor.x, "activeColorR", variables) &&
        setValueFromMap(activeColor.y, "activeColorG", variables) &&
        setValueFromMap(activeColor.z, "activeColorB", variables) &&
        setValueFromMap(activeColor.w, "activeColorA", variables);
    if (!activeColorSet) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de activeColor. Se inicializo a los valores predefinidos\n";
#endif
    }

    if (!setValueFromMap(windowFlags, "windowFlags", variables)) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de windowFlags. Se inicializo a los valores predefinidos\n";
#endif
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
    buttonFunctions[ButtonFunction::BUTTON_NONE] = []() {
#ifdef _DEBUG
        std::cout << "No se ha asignado ninguna funcion especial al boton\n";
#endif
    };

    buttonFunctions[ButtonFunction::BUTTON_INIT_GAME] = [this]() {
        if (!DynamicLibraryLoader::initGame()) {
            setText("Couldn't init game");
            setTextColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
        }
    };
}

void Button::updateOnClick() { onClick = buttonFunctions[onClickId]; }

void Button::updateTextFont() { textFont = uiManager->getFont(textFontName, textSize); }
}