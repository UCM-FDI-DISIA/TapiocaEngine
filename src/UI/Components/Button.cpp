#include "Button.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "UIManager.h"
#include "Structure/DynamicLibraryLoader.h"

namespace Tapioca {
Button::Button()
    : BaseWidget(), Component(), text("Button"), onClickId(ButtonFunction::BUTTON_NONE), onClick([]() {}), padding(ImVec2(10, 5)),
      textFont(nullptr), textFontName("arial.ttf"), textSize(16.0f), textColor(ImGui::GetStyle().Colors[ImGuiCol_Text]),
      normalColor(ImGui::GetStyle().Colors[ImGuiCol_Button]),
      hoverColor(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]),
      activeColor(ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]) { 
	createButtonFunctions();
}

bool Button::initComponent(const CompMap& variables) {
    bool nameSet = setValueFromMap(name, "name", variables);
    if (!nameSet) {
#ifdef _DEBUG
        std::cerr << "Error: Button: no se pudo inicializar el nombre.\n";
#endif
        return false;
    }

    bool textSet = setValueFromMap(text, "text", variables);
    if (!textSet) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de text. Se inicializo al valor predefinido\n";
#endif
    }

    bool onClickIdSet = setValueFromMap(onClickId, "onClickId", variables);
    if (!onClickIdSet) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de onClickId. Se inicializo al valor predefinido\n";
#endif
    }

    bool paddingSet =
        setValueFromMap(padding.x, "paddingX", variables) && setValueFromMap(padding.y, "paddingY", variables);
    if (!paddingSet) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de padding. Se inicializo a los valores predefinidos\n";
#endif
    }

    bool textFontNameSet = setValueFromMap(textFontName, "textFontName", variables);
    if (!textFontNameSet) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de textFontName. Se inicializo al valor predefinido\n";
#endif
    }

    bool textSizeSet = setValueFromMap(textSize, "textSize", variables);
    if (!textSizeSet) {
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

    bool windowFlagsSet = setValueFromMap(windowFlags, "windowFlags", variables);
    if (!windowFlagsSet) {
#ifdef _DEBUG
        std::cout << "Button: no se encontro el valor de windowFlags. Se inicializo a los valores predefinidos\n";
#endif
    }

    return true;
}

void Button::awake() {
    setTransform(object->getComponent<Transform>());
    onClick = buttonFunctions[onClickId];
    textFont = UIManager::instance()->getFont(textFontName, textSize);
}

void Button::render() const {
    std::string textStr = text;
    const char* text = textStr.c_str();
    ImVec2 constSize = getSize();
    ImVec2 buttonSize = constSize;
    // Si el tamano es -1, -1, se calcula el tamano del boton en funcion del texto
    if (constSize.x <= -1 && constSize.y <= -1) {
        ImVec2 textSize = ImGui::CalcTextSize(text);
        buttonSize = ImVec2(textSize.x + padding.x, textSize.y + padding.y);
    }

    // Establece la posicion y el tamano de la ventana de fondo a la correspondiente del boton
    ImGui::SetNextWindowPos(getPosition());
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
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    // Establece los colores del boton en los diferentes estados
    ImGui::PushStyleColor(ImGuiCol_Button, normalColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
    // Establece el ancho de envoltura para el texto del boton
    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + buttonSize.x);
    // -1, -1 para que el boton se ajuste al tamano de la ventana
    if (ImGui::Button(text, ImVec2(-1, -1))) onClick();

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
            setTextColor(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        }
    };
}
}