#include "ImageButton.h"
#include <imgui.h>
#include "UIManager.h"

namespace Tapioca {
ImageButton::ImageButton()
    : Button(), imagePath(""), textureId(), uv0(Vector2(0.0f, 0.0f)), uv1(Vector2(1.0f, 1.0f)),
      imageBgColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f)), imageTint(Vector4(1.0f, 1.0f, 1.0f, 1.0f)) { }

bool ImageButton::initComponent(const CompMap& variables) {

    if (!Button::initComponent(variables)) return false;

    if (!setValueFromMap(imagePath, "imagePath", variables)) {
#ifdef _DEBUG
        std::cerr << "Error: ImageButton: no se pudo inicializar la ruta de la imagen.\n";
#endif
        return false;
    }

    bool uv0Set = setValueFromMap(uv0.x, "uv0X", variables) && setValueFromMap(uv0.y, "uv0Y", variables);
    if (!uv0Set) {
#ifdef _DEBUG
        std::cout << "ImageButton: no se encontro el valor de uv0. Se inicializo a los valores predefinidos.\n";
#endif
    }

    bool uv1Set = setValueFromMap(uv1.x, "uv1X", variables) && setValueFromMap(uv1.y, "uv1Y", variables);
    if (!uv0Set) {
#ifdef _DEBUG
        std::cout << "ImageButton: no se encontro el valor de uv1. Se inicializo a los valores predefinidos.\n";
#endif
    }

    bool imageBgColorSet = setValueFromMap(imageBgColor.x, "imageBgColorR", variables) &&
        setValueFromMap(imageBgColor.y, "imageBgColorG", variables) &&
        setValueFromMap(imageBgColor.z, "imageBgColorB", variables) &&
        setValueFromMap(imageBgColor.w, "imageBgColorA", variables);
    if (!imageBgColorSet) {
#ifdef _DEBUG
        std::cout
            << "ImageButton: no se encontro el valor de imageBgColor. Se inicializo a los valores predefinidos.\n";
#endif
    }

    bool imageTintSet = setValueFromMap(imageTint.x, "imageTintR", variables) &&
        setValueFromMap(imageTint.y, "imageTintG", variables) &&
        setValueFromMap(imageTint.z, "imageTintB", variables) && setValueFromMap(imageTint.w, "imageTintA", variables);
    if (!imageTintSet) {
#ifdef _DEBUG
        std::cout << "ImageButton: no se encontro el valor de imageTint. Se inicializo a los valores predefinidos.\n";
#endif
    }

    return true;
}

void ImageButton::start() {
    Button::start();
    updateTexture();
}

void ImageButton::render() const {
    float scaleFactorX = uiManager->getScaleFactorX();
    float scaleFactorY = uiManager->getScaleFactorY();

    // Establece la posicion y el tamano de la ventana de fondo a la correspondiente del boton
    ImGui::SetNextWindowPos(ImVec2(getPosition().x * scaleFactorX, getPosition().y * scaleFactorY));
    ImGui::SetNextWindowSize(ImVec2(getSize().x * scaleFactorX, getSize().y * scaleFactorY));

    // Establece los estilos de la ventana de fondo, sin borde, sin padding y transparente
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());

    ImGui::Begin(name.c_str(), nullptr, windowFlags);

    ImGui::PopStyleVar(2);   // Pop para WindowBorderSize y WindowPadding

    // Establece los colores del boton en los diferentes estados
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(normalColor.x, normalColor.y, normalColor.z, normalColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(hoverColor.x, hoverColor.y, hoverColor.z, hoverColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(activeColor.x, activeColor.y, activeColor.z, activeColor.w));

    if (ImGui::ImageButton(imagePath.c_str(), textureId, ImVec2(getSize().x, getSize().y), ImVec2(uv0.x, uv0.y),
                           ImVec2(uv1.x, uv1.y), ImVec4(imageBgColor.x, imageBgColor.y, imageBgColor.z, imageBgColor.w),
                           ImVec4(imageTint.x, imageTint.y, imageTint.z, imageTint.w)))
        onClick();

    // Pop para WindowBg y los colores de los estados del boton
    ImGui::PopStyleColor(4);

    ImGui::End();
}

void ImageButton::updateTexture() { textureId = uiManager->getTextureId(imagePath); }
}
