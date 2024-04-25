#include "Slider.h"
#include <imgui.h>
#include <string>
#include "Structure/GameObject.h"
#include "UIManager.h"

namespace Tapioca {
Slider::Slider() : isVertical(false), currentValue(0), maxLimit(100), minLimit(0) { }

bool Slider::initComponent(const CompMap& variables) {
    if (!setValueFromMap(name, "name", variables)) {
        name = uiManager->generateName("Slider");
        logInfo(("Slider: No se encontro el valor de name. Se inicializo a un valor random: \"" + name + "\".").c_str());
    }

    if (!setValueFromMap(isVertical, "isVertical", variables)) {
        logInfo(("Slider: No se encontro el valor de isVertical. Se inicializo al valor predefinido: \"" +
                 std::to_string(isVertical) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(currentValue, "currentValue", variables)) {
        logInfo(("Slider: No se encontro el valor de currentValue. Se inicializo al valor predefinido: \"" +
                 std::to_string(currentValue) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(maxLimit, "maxLimit", variables)) {
        logInfo(("Slider: No se encontro el valor de maxLimit. Se inicializo al valor predefinido: \"" +
                 std::to_string(maxLimit) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(minLimit, "minLimit", variables)) {
        logInfo(("Slider: No se encontro el valor de minLimit. Se inicializo al valor predefinido: \"" +
                 std::to_string(minLimit) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(windowFlags, "windowFlags", variables)) {
        logInfo("Slider: No se encontro el valor de windowFlags. Se inicializo a los valores predefinidos: "
                "\"ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | "
                "ImGuiWindowFlags_NoSavedSettings\".");
    }
    return true;
}

void Slider::start() { setTransform(object->getComponent<Transform>()); }

void Slider::render() const {
    float scaleFactorX = uiManager->getScaleFactorX();
    float scaleFactorY = uiManager->getScaleFactorY();

    ImVec2 sliderSize(getSize().x * scaleFactorX, getSize().y * scaleFactorY);
    ImVec2 sliderPos(getPosition().x * scaleFactorX - sliderSize.x / 2.0f,
                     getPosition().y * scaleFactorY - sliderSize.y / 2.0f);
    ImGui::SetNextWindowPos(sliderPos);
    ImGui::SetNextWindowSize(sliderSize);

    ImGui::Begin(name.c_str(), nullptr, windowFlags);

    ImVec2 offset = ImGui::GetContentRegionMax();
    if (isVertical)
        ImGui::VSliderFloat(name.c_str(),
                            ImVec2(getSize().x - (getSize().x - offset.x), getSize().y - (getSize().y - offset.y)),
                            (float*)&currentValue, minLimit, maxLimit);
    else
        ImGui::SliderFloat(name.c_str(), (float*)&currentValue, minLimit, maxLimit);

    ImGui::End();
}
}