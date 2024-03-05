#include "UIManager.h"

#include <SDL.h>
#include <Ogre.h>
#include <OgreImGuiOverlay.h>
#include <OgreOverlayManager.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#ifdef _DEBUG
#include <iostream>
#endif

namespace Tapioca {
UIManager::UIManager() : myOgreWindow(nullptr), imguiOverlay(nullptr) { }

UIManager::~UIManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

bool UIManager::init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    GraphicsEngine* graphics = GraphicsEngine::instance();
    ImGui_ImplSDL2_InitForOpenGL(graphics->getSDLWindow(), (SDL_GLContext)graphics->getGLContext());
    ImGui_ImplOpenGL3_Init("#version 130");

    if (auto overlay = Ogre::OverlayManager::getSingleton().getByName("ImGuiOverlay"))
        imguiOverlay = static_cast<Ogre::ImGuiOverlay*>(overlay);
    else {
        imguiOverlay = new Ogre::ImGuiOverlay();
        Ogre::OverlayManager::getSingleton().addOverlay(imguiOverlay);
    }

    float vpScale = Ogre::OverlayManager::getSingleton().getPixelRatio();
    ImGui::GetStyle().ScaleAllSizes(vpScale);
    io.FontGlobalScale = std::round(vpScale);
    imguiOverlay->setZOrder(300);
    imguiOverlay->show();

    myOgreWindow = GraphicsEngine::instance()->getOgreWindow();
    myOgreWindow->addListener(this);
    Ogre::ImGuiOverlay::NewFrame();

    return true;
}

void UIManager::render() {
    Ogre::ImGuiOverlay::NewFrame();
    ImGui::ShowDemoWindow();
}
}
