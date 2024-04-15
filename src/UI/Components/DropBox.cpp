#include "imgui.h"
#include "DropBox.h"

#include "Structure/GameObject.h"

namespace Tapioca {
DropBox::DropBox() { }

DropBox::~DropBox() { }


bool DropBox::initComponent(const CompMap& variables) {
    if(!setValueFromMap(name, "name", variables)) {
        logError("DropBox: No se pudo inicializar el nombre.");
        return false;
    }
    int itemCuantity = 0;
    if (!setValueFromMap(itemCuantity, "itemCuantity", variables)) {
        logWarn("DropBox: no se encunetra la cantidad de items, la dropbox estara vacia");
        
       

    }

    for (int i = 0; i < itemCuantity; i++) {
        std::string varName = "item" + std::to_string(i);
        std::string placeholder = "???";
        if (!setValueFromMap(placeholder, varName, variables)) {
            logWarn("DropBox: no se encunetra el item");
        } 
        items.push_back(placeholder);
    }

    if (!setValueFromMap(currentItem, "currentItem", variables)){
        logWarn("DropBox: no se encuentra item inicial sera el primero");
        currentItem = 0;
    }


    return true;

}

void DropBox::start() { 
     setTransform(object->getComponent<Transform>());
}

void DropBox::render() const {
    
     ImGui::SetNextWindowPos(ImVec2(getPosition().x, getPosition().y));
    ImGui::SetNextWindowSize(ImVec2(getSize().x, getSize().y));

    ImGui::Begin(name.c_str(), nullptr, getWindowFlags());
    //ImGui::Begin(dbox.second->getName().c_str(), nullptr, dbox.second->getWindowFlags());

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