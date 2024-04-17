#include "RenderListener.h"
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "Structure/MainLoop.h"
#include "Structure/Scene.h"
#include "WindowManager.h"

namespace Tapioca {
RenderListener::RenderListener(MainLoop* mainLoop) : mainLoop(mainLoop) { windowManager = WindowManager::instance(); }

RenderListener::~RenderListener() {
    mainLoop = nullptr;
    windowManager = nullptr;
}

void RenderListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    for (std::pair<std::string, Scene*> s : mainLoop->getLoadedScenes())
        s.second->render();

    windowManager->setResized(true);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
}