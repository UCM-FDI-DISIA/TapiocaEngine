#include "ProgressBar.h"
#include <imgui.h>
#include "Structure/GameObject.h"
#include "UIManager.h"

namespace Tapioca {
ProgressBar::ProgressBar() : progress(0), backgroundText("") {
    ImVec4 barColorImVec = ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg];
    barColor = Vector4(barColorImVec.x, barColorImVec.y, barColorImVec.z, barColorImVec.w);
}

bool ProgressBar::initComponent(const CompMap& variables) {
    if (!setValueFromMap(name, "name", variables)) {
        logInfo(("ProgressBar: No se encontro el valor de name. Se inicializo a un valor random: \"" + name + "\".").c_str());
    }

    if (!setValueFromMap(progress, "progress", variables)) {
        logInfo(("ProgressBar: No se encontro el valor de progress. Se inicializo al valor predefinido: \"" +
                 std::to_string(progress) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(barColor.x, "barColorR", variables)) {
        logInfo(("ProgressBar: No se encontro el valor de barColorR. Se inicializo al valor predefinido: \"" +
                 std::to_string(barColor.x) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(barColor.y, "barColorG", variables)) {
        logInfo(("ProgressBar: No se encontro el valor de barColorG. Se inicializo al valor predefinido: \"" +
                 std::to_string(barColor.y) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(barColor.z, "barColorB", variables)) {
        logInfo(("ProgressBar: No se encontro el valor de barColorB. Se inicializo al valor predefinido: \"" +
                 std::to_string(barColor.z) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(barColor.w, "barColorA", variables)) {
        logInfo(("ProgressBar: No se encontro el valor de barColorA. Se inicializo al valor predefinido: \"" +
                 std::to_string(barColor.w) + "\".")
                    .c_str());
    }

    if (!setValueFromMap(backgroundText, "backtext", variables)) {
        logInfo("ProgressBar: No se encontro el valor de backgroundText. Se inicializo al valor predefinido: \"\".");
    }

    if (!setValueFromMap(windowFlags, "windowFlags", variables)) {
        logInfo("ProgressBar: No se encontro el valor de windowFlags. Se inicializo a los valores predefinidos: "
                "\"ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | "
                "ImGuiWindowFlags_NoSavedSettings\".");
    }
    return true;
}

void ProgressBar::start() { setTransform(object->getComponent<Transform>()); }

void ProgressBar::render() const {
    float scaleFactorX = uiManager->getScaleFactorX();
    float scaleFactorY = uiManager->getScaleFactorY();

    ImVec2 progressBarSize(getSize().x * scaleFactorX, getSize().y * scaleFactorY);
    ImVec2 progressBarPos(getPosition().x * scaleFactorX - progressBarSize.x / 2.0f,
                          getPosition().y * scaleFactorY - progressBarSize.y / 2.0f);
    ImGui::SetNextWindowPos(progressBarPos);
    ImGui::SetNextWindowSize(progressBarSize);

    ImGui::Begin(name.c_str(), nullptr, windowFlags);

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(barColor.x, barColor.y, barColor.z, barColor.w));

    ImVec2 offset = ImGui::GetContentRegionMax();

    ImGui::ProgressBar(progress, ImVec2(getSize().x - (getSize().x - offset.x), getSize().y - (getSize().y - offset.y)),
                       backgroundText.c_str());

    ImGui::PopStyleColor();
    ImGui::End();
}

void ProgressBar::addProgress(float p) {
    progress += p;
    if (progress > 1.0) progress = 1.0;
}
}