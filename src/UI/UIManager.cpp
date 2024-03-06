#include "UIManager.h"
#include <SDL.h>
#include <Ogre.h>
#include <OgreImGuiOverlay.h>
#include <OgreOverlayManager.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "Structure/FactoryManager.h"

namespace Tapioca {
UIManager::UIManager() : myOgreWindow(nullptr), imguiOverlay(nullptr), button("Play") { }

UIManager::~UIManager() {
    imguiOverlay = nullptr;
    myOgreWindow = nullptr;
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

    Ogre::OverlayManager* overlayManager = Ogre::OverlayManager::getSingletonPtr();
    if (auto overlay = overlayManager->getByName("ImGuiOverlay"))
        imguiOverlay = static_cast<Ogre::ImGuiOverlay*>(overlay);
    else {
        imguiOverlay = new Ogre::ImGuiOverlay(); // <- Aqui se producen los memory leaks
        overlayManager->addOverlay(imguiOverlay);
    }

    float vpScale = overlayManager->getPixelRatio();
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

    // PRUEBA
    ImGui::Begin("Tapioca Engine");

    if (ImGui::Button(button, ImVec2(120, 40))) {
#ifdef _DEBUG
    std::cout << "Pulsado el boton de jugar\n";
#endif
        // Cargar la .dll
        if (!FactoryManager::instance()->initGame()) {
			button = "Couldn't run game :(";
		}
    }
    ImGui::ShowDemoWindow();

    ImGui::End();
    // PRUEBA
}
}
