#include "Image.h"
#include <imgui.h>
#include "Structure/GameObject.h"
#include "UIManager.h"

namespace Tapioca {
Image::Image() : imagePath(""), textureId() { }

bool Image::initComponent(const CompMap& variables) {
    if (!setValueFromMap(name, "name", variables)) {
        logInfo(("Image: No se encontro el valor de name. Se inicializo a un valor random: \"" + name + "\".").c_str());
    }

    if (!setValueFromMap(imagePath, "imagePath", variables)) {
        logError("Image: No se encontro la ruta del archivo de origen para la imagen.");
        return false;
    }

    if (!setValueFromMap(windowFlags, "windowFlags", variables)) {
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        logInfo("Image: No se encontro el valor de windowFlags. Se inicializo a los valores predefinidos: "
                "\"ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | "
                "ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus\".");
    }
    return true;
}

void Image::start() {
    setTransform(object->getComponent<Transform>());
    updateTexture();
}

void Image::render() const {
    float scaleFactorX = uiManager->getScaleFactorX();
    float scaleFactorY = uiManager->getScaleFactorY();

    ImVec2 imageSize(getSize().x * scaleFactorX, getSize().y * scaleFactorY);
    ImVec2 imagePos(getPosition().x * scaleFactorX - imageSize.x / 2.0f,
                    getPosition().y * scaleFactorY - imageSize.y / 2.0f);

    // Establece la posicion y el tamano de la ventana de fondo a la correspondiente de la imagen
    ImGui::SetNextWindowPos(imagePos);
    ImGui::SetNextWindowSize(imageSize);

    // Establece los estilos de la ventana de fondo, sin borde, sin padding y transparente
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());

    ImGui::Begin(name.c_str(), nullptr, windowFlags);

    // Pop para WindowBorderSize y WindowPadding
    ImGui::PopStyleVar(2);

    ImGui::Image(textureId, imageSize);

    // Pop para WindowBg
    ImGui::PopStyleColor();

    ImGui::End();
}

void Image::updateTexture() { textureId = uiManager->getTextureId(imagePath); }
}
