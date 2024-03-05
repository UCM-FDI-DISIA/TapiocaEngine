#include "UIManager.h"

#include <Ogre.h>
#include <OgreImGuiOverlay.h>
#include <OgreOverlayManager.h>

#ifdef _DEBUG
#include <iostream>
#endif

Tapioca::UIManager::UIManager() : myOgreWindow(nullptr), imguiOverlay(nullptr) { }

void Tapioca::UIManager::shutdown() {
    ImGui::DestroyContext();
}

Ogre::ImGuiOverlay* Tapioca::UIManager::initImgui() { 
     if (auto overlay = Ogre::OverlayManager::getSingleton().getByName("ImGuiOverlay"))
        return static_cast<Ogre::ImGuiOverlay*>(overlay);

    auto imguiOverlay = new Ogre::ImGuiOverlay();
    Ogre::OverlayManager::getSingleton().addOverlay(imguiOverlay);   // now owned by overlaymgr

    float vpScale = Ogre::OverlayManager::getSingleton().getPixelRatio();
    ImGui::GetStyle().ScaleAllSizes(vpScale);

    //mImGuiListener.reset(new ImGuiInputListener());

    return imguiOverlay;
}

Tapioca::UIManager::~UIManager() { shutdown(); }

bool Tapioca::UIManager::init() {

    myOgreWindow = Tapioca::GraphicsEngine::instance()->getOgreWindow();
    imguiOverlay = initImgui();
  
    float vpScale = Ogre::OverlayManager::getSingleton().getPixelRatio();
    ImGui::GetIO().FontGlobalScale = std::round(vpScale);   // default font does not work with fractional scaling
    imguiOverlay->setZOrder(300);
    imguiOverlay->show();
    myOgreWindow->addListener(this);
    Ogre::ImGuiOverlay::NewFrame();

    return true;
}

//void Tapioca::UIManager::update(const uint64_t deltaTime) { }

//void Tapioca::UIManager::handleEvents() { 
//    SDL_Event event;
//    while (SDL_PollEvent(&event))
//        ImGui_ImplSDL2_ProcessEvent(&event);
//}

//void Tapioca::UIManager::fixedUpdate() { }

void Tapioca::UIManager::render() {
    Ogre::ImGuiOverlay::NewFrame();
    ImGui::ShowDemoWindow();
}

//void Tapioca::UIManager::refresh() { }
