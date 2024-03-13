#include "UIManager.h"
#include <SDL.h>
#include <Ogre.h>
#include <OgreImGuiOverlay.h>
#include <OgreOverlayManager.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "Structure/FactoryManager.h"
#include "Structure/DynamicLibraryLoader.h"
#include "checkML.h"

namespace Tapioca {
UIManager::UIManager()
    : mySDLWindow(nullptr), myOgreWindow(nullptr), myGLContext(nullptr), imguiOverlay(nullptr), button("Slay") { }

UIManager::~UIManager() {
    mySDLWindow = nullptr;
    myOgreWindow = nullptr;
    myGLContext = nullptr;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) { render(); }

bool UIManager::init() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    GraphicsEngine* graphics = GraphicsEngine::instance();
    mySDLWindow = graphics->getSDLWindow();
    myGLContext = graphics->getGLContext();
    ImGui_ImplSDL2_InitForOpenGL(mySDLWindow, myGLContext);
    ImGui_ImplOpenGL3_Init("#version 130");
    myOgreWindow = graphics->getOgreWindow();
    myOgreWindow->addListener(this);
 

    return true;
}

void UIManager::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    // PRUEBA
    ImGui::Begin("Tapioca Engine");

    if (ImGui::Button(button, ImVec2(130, 40))) {
#ifdef _DEBUG
        std::cout << "Pulsado el boton de jugar\n";
#endif
        // Cargar la .dll
        if (!DynamicLibraryLoader::initGame()) {
            button = "Couldn't run game";
        }
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
}
