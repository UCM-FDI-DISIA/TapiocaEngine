#include "imgui.h"
#include "ProgressBar.h"

Tapioca::ProgressBar::ProgressBar() { }

Tapioca::ProgressBar::~ProgressBar() { }

bool Tapioca::ProgressBar::initComponent(const CompMap& variables) {
    
     if (!setValueFromMap(name, "name", variables)) {
        logError("ProgressBar: No se pudo inicializar el nombre.");
        return false;
     }
     if (!setValueFromMap(progress, "progress", variables)) {
         progress = 0;
     }

     if (!setValueFromMap(color.x, "colorX", variables)) {
        
         color.x = 0;
     }
     if (!setValueFromMap(color.y, "colorY", variables)) {

         color.y = 0;
     }
     if (!setValueFromMap(color.z, "colorZ", variables)) {

         color.z = 0;
     }
     if (!setValueFromMap(color.w, "colorW", variables)) {

         color.w = 1.0;
     }
     if (!setValueFromMap(backgroundtext, "backtext", variables)) {

        backgroundtext = "";
     }

    return true; }

void Tapioca::ProgressBar::start() { 
setTransform(object->getComponent<Transform>()); 
}

void Tapioca::ProgressBar::render() const { 
    ImGui::SetNextWindowPos(ImVec2(getPosition().x, getPosition().y));
    ImGui::SetNextWindowSize(ImVec2(getSize().x,getSize().y));


    ImGui::Begin(name.c_str(), nullptr, getWindowFlags());
    //ImGui::Begin(progb.second->getName().c_str(), nullptr, progb.second->getWindowFlags());

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
                          ImVec4(color.x,color.y, color.z,
                                color.w));

    ImVec2 offset = ImGui::GetContentRegionMax();

    ImGui::ProgressBar(progress,
                       ImVec2(getSize().x - (getSize().x - offset.x),
                             getSize().y - (getSize().y - offset.y)),
                      backgroundtext.c_str());

    ImGui::PopStyleColor();
    ImGui::End();

}

void Tapioca::ProgressBar::addProgress(float p) { 
    
    progress += p;
    if (progress > 1.0) {
        progress = 1.0;

    }

     }

