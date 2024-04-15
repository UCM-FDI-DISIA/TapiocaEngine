#include "imgui.h"
#include "Slider.h"
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
    ImGui::SetNextWindowPos(ImVec2(getPosition().x, getPosition().y));
    ImGui::SetNextWindowSize(ImVec2(getSize().x, getSize().y));


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