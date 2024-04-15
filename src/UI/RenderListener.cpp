#include "RenderListener.h"
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "Structure/MainLoop.h"
#include "Structure/Scene.h"
#include "WindowManager.h"

// Temporal
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/DropBox.h"

namespace Tapioca {
RenderListener::RenderListener(MainLoop* mainLoop) : mainLoop(mainLoop), testid(), item_current() {
    windowManager = WindowManager::instance();
}

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

    mainLoop = nullptr;
}

void RenderListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    for (std::pair<std::string, Scene*> s : mainLoop->getLoadedScenes())
        s.second->render();

    windowManager->setResized(true);

 

    // ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// TEMPORAL
Image* RenderListener::createImage(std::string file, Tapioca::Vector2 widthandheigth, Tapioca::Vector2 xandy) {
    /* Image* image = new Image(file, widthandheigth, xandy);

    images.insert({file, image});
    return image;*/
    return nullptr;
}

ProgressBar* RenderListener::createProgressBar(float initprogress, Vector4 progesscolor, std::string backtext,
                                               Vector2 siz, Vector2 pos) {
    /* ProgressBar* progb = new ProgressBar(initprogress, progesscolor, pos, siz, backtext);
    progressbars.insert({backtext, progb});

    return progb;*/
    return nullptr;
}
Slider* RenderListener::createSlider(std::string tittle, bool verticalMode, float initvalue, float maxValue,
                                     float minValue, Vector2 siz, Vector2 pos) {
    /* Slider* slide = new Slider(tittle, verticalMode, initvalue, maxValue, minValue, pos, siz);
    sliders.insert({tittle, slide});
    return slide;*/
    return nullptr;
}
DropBox* RenderListener::createDropBox(std::string tittle, std::vector<std::string> content, int initialselect,
                                       Vector2 siz, Vector2 pos) {
    /* DropBox* box = new DropBox(tittle, content, initialselect, pos, siz);
    dropboxes.insert({tittle, box});
    return box;*/
    return nullptr;
}
}