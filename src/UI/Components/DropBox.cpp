#include "DropBox.h"
#include <imgui.h>
#include "UIManager.h"
#include "Structure/GameObject.h"

namespace Tapioca {
DropBox::DropBox() : currentItem(0) { }

bool DropBox::initComponent(const CompMap& variables) {
    if (!setValueFromMap(name, "name", variables)) {
        logError("DropBox: No se pudo inicializar el nombre.");
        return false;
    }

    int itemCuantity = 0;
    if (!setValueFromMap(itemCuantity, "itemCuantity", variables)) {
        logWarn("DropBox: No se encontro la cantidad de items. La dropbox quedara vacia.");
    }

    for (int i = 0; i < itemCuantity; i++) {
        std::string varName = "item" + std::to_string(i);
        std::string placeholder = "???";
        if (!setValueFromMap(placeholder, varName, variables)) {
            logWarn("DropBox: No se encontro el item");
        }
        items.push_back(placeholder);
    }

    if (!setValueFromMap(currentItem, "currentItem", variables)) {
        logWarn("DropBox: No se encontro el item inicial. Sera el primero");
    }

    if (!setValueFromMap(windowFlags, "windowFlags", variables)) {
        logInfo("DropBox: No se encontro el valor de windowFlags. Se inicializo a los valores predefinidos: "
                "\"ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | "
                "ImGuiWindowFlags_NoSavedSettings\".");
    }
    return true;
}

void DropBox::start() { setTransform(object->getComponent<Transform>()); }

void DropBox::render() const {
    float scaleFactorX = uiManager->getScaleFactorX();
    float scaleFactorY = uiManager->getScaleFactorY();

    ImVec2 dropBoxSize(getSize().x * scaleFactorX, getSize().y * scaleFactorY);
    ImVec2 dropBoxPos(getPosition().x * scaleFactorX - dropBoxSize.x / 2.0f,
                      getPosition().y * scaleFactorY - dropBoxSize.y / 2.0f);
    ImGui::SetNextWindowPos(dropBoxSize);
    ImGui::SetNextWindowSize(dropBoxPos);

    ImGui::Begin(name.c_str(), nullptr, windowFlags);

    int s = items.size();
    int current = currentItem;
    if (s > 0) {
        if (ImGui::BeginCombo("##", items[current].c_str())) {
            for (int i = 0; i < s; i++) {

                bool selected = i == current;
                ImGui::Selectable(items[i].c_str(), selected);
            }
            ImGui::EndCombo();
        }
    }
    ImGui::End();
}
}