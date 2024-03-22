#include "UIManager.h"

// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <OgreRenderWindow.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "Structure/DynamicLibraryLoader.h"
#include "GraphicsEngine.h"

#include "Button.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<UIManager>;
template<>
UIManager* Singleton<UIManager>::instance_ = nullptr;

UIManager::UIManager()
    : mySDLWindow(nullptr), myOgreWindow(nullptr), myGLContext(nullptr), mySceneManager(nullptr) { }

UIManager::~UIManager() {
    mySDLWindow = nullptr;
    myOgreWindow = nullptr;
    myGLContext = nullptr;
    mySceneManager = nullptr;

    for (auto button : buttons) {
        delete button.second;
    }
    buttons.clear();

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
    mySceneManager = graphics->getSceneManager();

    return true;
}

void UIManager::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    for (auto button : buttons) {
        std::string textStr = button.second->getText();
        const char* text = textStr.c_str();
        ImGui::Begin(text, button.second->getCanCloseWindow(), button.second->getFlags());
        if (ImGui::Button(text, button.second->getSize())) {
            button.second->getOnClick()();
        }
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Button* UIManager::createButton(const std::string& name, RenderNode* const node, const ImVec2 position,
                                const std::string& text, std::function<void()> onClick, const ImVec2 size,
                                bool* canCloseWindow, ImGuiWindowFlags flags) {
    if (!buttons.contains(name)) {
        Button* button = new Button(mySceneManager, node, position, text, onClick, size, canCloseWindow, flags);
        buttons.insert({name, button});
        return button;
    }
    else {
#ifdef _DEBUG
        std::cout << "Ya existe el botón con el nombre " << name << '\n';
#endif
        return nullptr;
    }
}

Button* UIManager::getButton(const std::string& name) {
    if (buttons.contains(name)) return buttons[name];
    else
        return nullptr;
}
}
