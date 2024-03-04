#include "UIManager.h"

#include <SDL.h>
#include <SDL_video.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <Ogre.h>
#include "OgreImGuiOverlay.h"
#include "OgreOverlayManager.h"

#ifdef _DEBUG
#include <iostream>
#endif

Tapioca::UIManager::UIManager() : myWindow(nullptr) { }

//bool Tapioca::UIManager::frameRenderingQueued(const Ogre::FrameEvent& evt) { 
// // render();
//    return true;
//}

void Tapioca::UIManager::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

Ogre::ImGuiOverlay* Tapioca::UIManager::initImgui() { 
     if (auto overlay = Ogre::OverlayManager::getSingleton().getByName("ImGuiOverlay"))
        return static_cast<Ogre::ImGuiOverlay*>(overlay);

    auto imguiOverlay = new Ogre::ImGuiOverlay();
    Ogre::OverlayManager::getSingleton().addOverlay(imguiOverlay);   // now owned by overlaymgr

    // handle DPI scaling
    float vpScale = Ogre::OverlayManager::getSingleton().getPixelRatio();
    ImGui::GetStyle().ScaleAllSizes(vpScale);

    //mImGuiListener.reset(new ImGuiInputListener());

    return imguiOverlay;

}

Tapioca::UIManager::~UIManager() { shutdown(); }

bool Tapioca::UIManager::init() {

    myWindow = Tapioca::GraphicsEngine::instance()->getSDLWindow();
    myogreWindow = Tapioca::GraphicsEngine::instance()->getOgreWindow();
    imguiOverlay = initImgui();
  
    float vpScale = Ogre::OverlayManager::getSingleton().getPixelRatio();
    ImGui::GetIO().FontGlobalScale = std::round(vpScale);   // default font does not work with fractional scaling
    imguiOverlay->setZOrder(300);
    imguiOverlay->show();
    myogreWindow->addListener(this);
    Ogre::ImGuiOverlay::NewFrame();

    ImGui::ShowDemoWindow();
    /* if (myWindow == nullptr) {
#ifdef _DEBUG
        std::cerr << "Error al inicializar ImGui: " << SDL_GetError() << '\n';
#endif
        return false;
    }

    // Inicializar ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    float width = 400.f;
    float height = 400.f;
    io.DisplaySize = ImVec2(width, height);
    gl_context = Tapioca::GraphicsEngine::instance()->getGLContext();
    SDL_GL_SetSwapInterval(1);
    // Inicializar ImGui backends
    ImGui_ImplSDL2_InitForOpenGL(myWindow, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");   // Reemplazar con la versión que queramos de GLSL
   // ImGui::NewFrame();*/
    return true;
}
//void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)  {
//    if (!evt.source->getOverlaysEnabled()) return;
//   // if (!mTrayMgr->getTraysLayer()->isVisible()) return;
//
//    Ogre::ImGuiOverlay::NewFrame();
//
//    ImGui::ShowDemoWindow();
//}

//void Tapioca::UIManager::update(const uint64_t deltaTime) { }

//void Tapioca::UIManager::handleEvents() { 
//    SDL_Event event;
//    while (SDL_PollEvent(&event))
//        ImGui_ImplSDL2_ProcessEvent(&event);
//}

//void Tapioca::UIManager::fixedUpdate() { }

//void Tapioca::UIManager::render() {
//    // Ejemplo de ventana de demostracion de ImGui
//    ImGui_ImplSDL2_InitForOpenGL(myWindow, gl_context);
//    ImGui_ImplOpenGL3_Init("#version 130");
//    ImGui::NewFrame();
//    bool demo = true;
//    ImGui::ShowDemoWindow(&demo);
//
//    ImGui::Render();
//    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//    SDL_GL_SwapWindow(myWindow);
//
//}

//void Tapioca::UIManager::refresh() { }
