#include "Text.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
#include "UIManager.h"

namespace Tapioca {
Text::Text()
    : BaseWidget(), Component(), text("Texto..."), textSize(16.0f), textFontName("arial.ttf"), textFont(nullptr) {
    ImVec4 textColorImVec = ImGui::GetStyle().Colors[ImGuiCol_Text];
    textColor = Vector4(textColorImVec.x, textColorImVec.y, textColorImVec.z, textColorImVec.w);
}

Text::~Text() { textFont = nullptr; }

bool Text::initComponent(const CompMap& variables) {
    bool nameSet = setValueFromMap(name, "name", variables);
    if (!nameSet) {
#ifdef _DEBUG
        std::cerr << "Error: Text: no se pudo inicializar el nombre.\n";
#endif
        return false;
    }

    bool textSet = setValueFromMap(text, "text", variables);
    if (!textSet) {
#ifdef _DEBUG
        std::cout << "Text: no se encontro el valor de text. Se inicializo al valor predefinido\n";
#endif
    }

    bool textSizeSet = setValueFromMap(textSize, "textSize", variables);
    if (!textSizeSet) {
#ifdef _DEBUG
        std::cout << "Text: no se encontro el valor de textSize. Se inicializo al valor predefinido\n";
#endif
    }

    bool textFontNameSet = setValueFromMap(textFontName, "textFontName", variables);
    if (!textFontNameSet) {
#ifdef _DEBUG
        std::cout << "Text: no se encontro el valor de text. Se inicializo al valor predefinido\n";
#endif
    }

    bool textColorSet = setValueFromMap(textColor.x, "textColorR", variables) &&
        setValueFromMap(textColor.y, "textColorG", variables) &&
        setValueFromMap(textColor.z, "textColorB", variables) && setValueFromMap(textColor.w, "textColorA", variables);
    if (!textColorSet) {
#ifdef _DEBUG
        std::cout << "Text: no se encontro el valor de textColor. Se inicializo a los valores predefinidos\n";
#endif
    }

    bool windowFlagsSet = setValueFromMap(windowFlags, "windowFlags", variables);
    if (!windowFlagsSet) {
#ifdef _DEBUG
        std::cout << "Text: no se encontro el valor de windowFlags. Se inicializo a los valores predefinidos\n";
#endif
    }

    return true;
}

void Text::start() {
    setTransform(object->getComponent<Transform>());
    updateTextFont();
}

void Text::render() const {
    // Establece la posicion y el tamano de la ventana de fondo a la correspondiente del texto
    ImGui::SetNextWindowPos(ImVec2(getPosition().x, getPosition().y));
    ImGui::SetNextWindowSize(ImVec2(getSize().x, getSize().y));

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
    // Imprime el texto
    ImGui::Text(text.c_str());

    // Pop para WindowBg, el color del texto
    ImGui::PopStyleColor(2);
    // Pop para la fuente del texto
    ImGui::PopFont();

    ImGui::End();
}

void Text::updateTextFont() { textFont = UIManager::instance()->getFont(textFontName, textSize); }
}