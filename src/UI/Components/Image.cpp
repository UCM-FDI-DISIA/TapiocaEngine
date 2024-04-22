#include "Image.h"
#include <imgui.h>
#include "Structure/GameObject.h"
#include "UIManager.h"

namespace Tapioca {
Image::Image() : imagePath(""), textureId() { }

bool Image::initComponent(const CompMap& variables) {
    if (!setValueFromMap(name, "name", variables)) {
        logError("Image: No se pudo inicializar el nombre.");
        return false;
    }

    if (!setValueFromMap(imagePath, "imagePath", variables)) {
        logError("Image: No se encontro la ruta del archivo de origen para la imagen.");
        return false;
    }

    if (!setValueFromMap(windowFlags, "windowFlags", variables)) {
        logInfo("Image: No se encontro el valor de windowFlags. Se inicializo a los valores predefinidos: "
                "\"ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | "
                "ImGuiWindowFlags_NoSavedSettings\".");
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

    //ImGui::SetNextWindowPos(ImVec2(getPosition().x, getPosition().y));
    //ImGui::SetNextWindowSize(ImVec2(getSize().x, getSize().y));
    ImGui::SetNextWindowPos(imagePos);
    ImGui::SetNextWindowSize(imageSize);

    ImGui::Begin(name.c_str(), nullptr, windowFlags);

    ImVec2 imagesize = ImVec2(getSize().x, getSize().y);
    ImVec2 offset = ImGui::GetContentRegionMax();
    ImGui::Image(textureId, ImVec2(getSize().x - (getSize().x - offset.x), getSize().y - (getSize().y - offset.y)));

    ImGui::End();
}

void Image::updateTexture() { textureId = uiManager->getTextureId(imagePath); }
}
