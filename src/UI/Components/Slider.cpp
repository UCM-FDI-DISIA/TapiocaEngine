#include "imgui.h"
#include "Slider.h"
#include "UIManager.h"
namespace Tapioca {
Tapioca::Slider::Slider() { }

Tapioca::Slider::~Slider() { }
float* Tapioca::Slider::getCurrentValue() { return &currentValue; }
bool Tapioca::Slider::initComponent(const CompMap& variables) {
    if (!setValueFromMap(name, "name", variables)) {
        logError("Slider: No se pudo inicializar el nombre.");
        return false;
    }
    if (!setValueFromMap(vertical, "vertical", variables)) {
        vertical = false;
    }
    if (!setValueFromMap(currentValue, "initValue", variables)) {
        currentValue = 0;
    }
    if (!setValueFromMap(max, "maxValue", variables)) {
        max = 100;
    }
    if (!setValueFromMap(min, "minValue", variables)) {
        min = 0;
    }
    return true;
}

void Tapioca::Slider::start() { setTransform(object->getComponent<Transform>()); }

void Tapioca::Slider::render() const {
    float scaleFactorX = uiManager->getScaleFactorX();
    float scaleFactorY = uiManager->getScaleFactorY();

    ImVec2 sliderSize(getSize().x * scaleFactorX, getSize().y * scaleFactorY);
    ImVec2 sliderPos(getPosition().x * scaleFactorX - sliderSize.x / 2.0f,
                      getPosition().y * scaleFactorY - sliderSize.y / 2.0f);
    ImGui::SetNextWindowPos(sliderPos);
    ImGui::SetNextWindowSize(sliderSize);


    ImGui::Begin(name.c_str(), nullptr, getWindowFlags());
    //ImGui::Begin(slider.second->getName().c_str(), nullptr, slider.second->getWindowFlags());

    ImVec2 offset = ImGui::GetContentRegionMax();
    if (vertical) {
        ImGui::VSliderFloat(name.c_str(),
                            ImVec2(getSize().x - (getSize().x - offset.x), getSize().y - (getSize().y - offset.y)),
                            (float*) & currentValue, min, max);
    }
    else {
        ImGui::SliderFloat(name.c_str(), (float*) &currentValue, min, max);
    }
    ImGui::End();
}



}