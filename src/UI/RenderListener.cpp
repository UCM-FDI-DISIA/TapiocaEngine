#include "RenderListener.h"
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include "Structure/MainLoop.h"
#include "Structure/Scene.h"
#include "Structure/GameObject.h"
#include "Structure/Component.h"
#include "WindowManager.h"
#include "checkML.h"

namespace Tapioca {
RenderListener::RenderListener(MainLoop* mainLoop) : mainLoop(mainLoop) { windowManager = WindowManager::instance(); }

RenderListener::~RenderListener() {
    mainLoop = nullptr;
    windowManager = nullptr;
}

void RenderListener::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) {
    // Actualiza la escala de los textos, imagenes, etc.
    for (std::pair<std::string, Scene*> s : mainLoop->getLoadedScenes())
        for (GameObject* go : s.second->getObjects())
            for (Component* c : go->getAllComponents())
                c->updateUI();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    for (std::pair<std::string, Scene*> s : mainLoop->getLoadedScenes())
        s.second->render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    windowManager->setResized(true);
}
}