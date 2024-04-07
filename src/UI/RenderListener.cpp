#include "RenderListener.h"
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "Structure/Game.h"
#include "Structure/Scene.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/DropBox.h"

namespace Tapioca {

RenderListener::RenderListener(Game* game) : game(game), testid(), item_current() {}

RenderListener::~RenderListener() {
    // TEMPORAL
    for (auto image : images) {
        delete image.second;
    }
    images.clear();

    for (auto progb : progressbars) {
        delete progb.second;
    }
    progressbars.clear();

    for (auto slider : sliders) {
        delete slider.second;
    }
    sliders.clear();

    for (auto dropbox : dropboxes) {
        delete dropbox.second;
    }
    dropboxes.clear();

    game = nullptr;
}

void RenderListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    for (std::pair<std::string, Scene*> s : game->getLoadedScenes())
        s.second->render();

    // TEMPORAL
    for (auto image : images) {
        ImGui::SetNextWindowPos(ImVec2(image.second->getPosition().x, image.second->getPosition().y));
        ImGui::SetNextWindowSize(ImVec2(image.second->getSize().x, image.second->getSize().y));


        ImGui::Begin(image.first.c_str(), nullptr, image.second->getWindowFlags());
        //ImGui::Begin(image.second->getName().c_str(), nullptr, image.second->getWindowFlags());

        // int* testid2 =new int( 1);


        //El tamanio de la ventana del mismo tama�o qeu laimagen para qeu se vea entera y no se corte

        ImVec2 offset = ImGui::
            GetContentRegionMax();   // las dimensiones de la ventana no son las mismas qeu las del espacion donde se muestra contenido cuidado
        ImGui::Image((void*)(intptr_t)image.second->getID(),
                     ImVec2(image.second->getSize().x - (image.second->getSize().x - offset.x),
                            image.second->getSize().y - (image.second->getSize().y - offset.y)));
        //de esta manera siempre se ajusta la imagen al las dimensiones de la ventana


        /* ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1, 0, 0, 0.4));
       
        ImGui::ProgressBar(0.6, ImVec2(100, 40), "Patata");

        ImGui::PopStyleColor();
      
        
        const char* itmes2[sizeof(items)] ;
        itmes2[0] = items[0].c_str();
        itmes2[1] = items[1].c_str();

        ImGui::Combo("dropbox", &item_current,itmes2, 2);
        float v = 12;
        ImGui::SliderFloat("slider", &v, 0, 30,0);
       // ImGui::VSliderFloat("slider", ImVec2(50, 100), &v, 0, 30, "%.3f");
      //al parecer se puede para un formato de strings que eso es el "%.3f"*/
        ImGui::End();
    }
    //temporal hasta qeu esten todo en el pair es qeu en el pueblo no tenia los archivos nuevos :(
    for (auto progb : progressbars) {
        ImGui::SetNextWindowPos(ImVec2(progb.second->getPosition().x, progb.second->getPosition().y));
        ImGui::SetNextWindowSize(ImVec2(progb.second->getSize().x, progb.second->getSize().y));


        ImGui::Begin(progb.first.c_str(), nullptr, progb.second->getWindowFlags());
        //ImGui::Begin(progb.second->getName().c_str(), nullptr, progb.second->getWindowFlags());
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
                              ImVec4(progb.second->getColor().x, progb.second->getColor().y, progb.second->getColor().z,
                                     progb.second->getColor().w));

        ImVec2 offset = ImGui::
            GetContentRegionMax();   // las dimensiones de la ventana no son las mismas qeu las del espacion donde se muestra contenido cuidado

        ImGui::ProgressBar(progb.second->getProgress(),
                           ImVec2(progb.second->getSize().x - (progb.second->getSize().x - offset.x),
                                  progb.second->getSize().y - (progb.second->getSize().y - offset.y)),
                           progb.second->getText().c_str());

        ImGui::PopStyleColor();
        ImGui::End();
    }
    for (auto slider : sliders) {
        ImGui::SetNextWindowPos(ImVec2(slider.second->getPosition().x, slider.second->getPosition().y));
        ImGui::SetNextWindowSize(ImVec2(slider.second->getSize().x, slider.second->getSize().y));


        ImGui::Begin(slider.first.c_str(), nullptr, slider.second->getWindowFlags());
        //ImGui::Begin(slider.second->getName().c_str(), nullptr, slider.second->getWindowFlags());
        ImVec2 offset = ImGui::GetContentRegionMax();
        if (slider.second->isVertical()) {
            ImGui::VSliderFloat(slider.second->getName().c_str(),
                                ImVec2(slider.second->getSize().x - (slider.second->getSize().x - offset.x),
                                       slider.second->getSize().y - (slider.second->getSize().y - offset.y)),
                                slider.second->getCurrentValue(), slider.second->getMin(), slider.second->getMax());
        }
        else {
            ImGui::SliderFloat(slider.second->getName().c_str(), slider.second->getCurrentValue(),
                               slider.second->getMin(), slider.second->getMax());
        }
        ImGui::End();
    }
    for (auto dbox : dropboxes) {
        ImGui::SetNextWindowPos(ImVec2(dbox.second->getPosition().x, dbox.second->getPosition().y));
        ImGui::SetNextWindowSize(ImVec2(dbox.second->getSize().x, dbox.second->getSize().y));
        // ImGui::SetCursorPos(ImVec2(dbox.second->getPosition().x, dbox.second->getPosition().y));
        //ImGui:
        ImGui::Begin(dbox.first.c_str(), nullptr, dbox.second->getWindowFlags());
        //ImGui::Begin(dbox.second->getName().c_str(), nullptr, dbox.second->getWindowFlags());
        int s = dbox.second->getContent().size();
        int current = *dbox.second->getCurrent();
        //int items[];
        /* const char* itmes2[];
        itmes2[0] = items[0].c_str();
        itmes2[1] = items[1].c_str();*/

        if (ImGui::BeginCombo(dbox.second->getName().c_str(), dbox.second->getContent()[current].c_str())) {
            for (int i = 0; i < s; i++) {

                bool selected = i == current;
                ImGui::Selectable(dbox.second->getContent()[i].c_str(), selected);
            }
            ImGui::EndCombo();
        }
        // ImGui::Combo(dbox.second->getName(), dbox.second->getCurrent(), dbox.second->getContent(),2);
        // float v = 12;
        ImGui::End();
    }

    //  ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// TEMPORAL
Image* RenderListener::createImage(std::string file, Tapioca::Vector2 widthandheigth, Tapioca::Vector2 xandy) {
    Image* image = new Image(file, widthandheigth, xandy);

    images.insert({file, image});
    return image;
}

ProgressBar* RenderListener::createProgressBar(float initprogress, Vector4 progesscolor, std::string backtext,
                                               Vector2 siz,
                                          Vector2 pos) {
    ProgressBar* progb = new ProgressBar(initprogress, progesscolor, pos, siz, backtext);
    progressbars.insert({backtext, progb});

    return progb;
}
Slider* RenderListener::createSlider(std::string tittle, bool verticalMode, float initvalue, float maxValue,
                                     float minValue,
                                Vector2 siz, Vector2 pos) {
    Slider* slide = new Slider(tittle, verticalMode, initvalue, maxValue, minValue, pos, siz);
    sliders.insert({tittle, slide});
    return slide;
}
DropBox* RenderListener::createDropBox(std::string tittle, std::vector<std::string> content, int initialselect,
                                       Vector2 siz,
                                  Vector2 pos) {
    DropBox* box = new DropBox(tittle, content, initialselect, pos, siz);
    dropboxes.insert({tittle, box});
    return box;
}
}