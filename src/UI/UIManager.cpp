#include "UIManager.h"

#include <filesystem>
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

#include "WindowManager.h"

#include "Button.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<UIManager>;
template<>
UIManager* Singleton<UIManager>::instance_ = nullptr;

UIManager::UIManager() : sdlWindow(nullptr), glContext(nullptr), sceneManager(nullptr) { }

UIManager::~UIManager() {
    sdlWindow = nullptr;
    glContext = nullptr;
    sceneManager = nullptr;

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
    sdlWindow = WindowManager::instance()->getWindow();
    glContext = WindowManager::instance()->getGLContext();
    ImGui_ImplSDL2_InitForOpenGL(sdlWindow, glContext);
    ImGui_ImplOpenGL3_Init("#version 130");

    //sceneManager = graphics->getSceneManager();

    loadFonts(io, 16.0f);

    return true;
}

void UIManager::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    for (auto button : buttons) {
        std::string textStr = button.second->getText();
        const char* text = textStr.c_str();
        ImVec2 constSize = button.second->getConstSize();
        ImVec2 buttonSize = constSize;
        // Si el tamano es -1, -1, se calcula el tamano del boton en funcion del texto
        if (constSize.x <= -1 && constSize.y <= -1) {
            ImVec2 textSize = ImGui::CalcTextSize(text);
            buttonSize = ImVec2(textSize.x + button.second->getPadding().x, textSize.y + button.second->getPadding().y);
        }

        // Establece la posicion y el tamano de la ventana de fondo a la correspondiente del boton
        ImGui::SetNextWindowPos(button.second->getPosition());
        ImGui::SetNextWindowSize(buttonSize);

        // Establece los estilos de la ventana de fondo, sin borde, sin padding y transparente
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

        ImGui::Begin(text, button.second->getCanCloseWindow(), button.second->getFlags());

        ImGui::PopStyleVar(2);   // Pop para WindowBorderSize y WindowPadding
        // Establece los colores del boton en los diferentes estados
        ImGui::PushStyleColor(ImGuiCol_Button, button.second->getNormalColor());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button.second->getHoverColor());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, button.second->getActiveColor());

        // Establece el ancho de envoltura para el texto del boton
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + buttonSize.x);

        // -1, -1 para que el boton se ajuste al tamano de la ventana
        if (ImGui::Button(text, ImVec2(-1, -1))) button.second->getOnClick()();

        // Pop para el ancho de envoltura
        ImGui::PopTextWrapPos();

        // Pop para WindowBg y los colores del boton
        ImGui::PopStyleColor(4);

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UIManager::handleEvents(const SDL_Event& event) { return ImGui_ImplSDL2_ProcessEvent(&event); }

bool UIManager::loadFonts(ImGuiIO& io, float pixelSize) {
    std::string fontsPath = "assets/fonts/";

    for (const auto& entry : std::filesystem::directory_iterator(fontsPath)) {
        if (entry.is_regular_file()) {
            auto path = entry.path();
            if (path.extension() == ".ttf" || path.extension() == ".TTF" || path.extension() == ".otf" ||
                path.extension() == ".OFT") {
                ImFont* font = io.Fonts->AddFontFromFileTTF(path.string().c_str(), pixelSize);
                if (font == nullptr) {
#ifdef _DEBUG
                    std::cerr << "Error al cargar la fuente: " << path << "\n";
#endif
                    return false;
                }
#ifdef _DEBUG
                std::cout << "Se ha cargado la fuente: " << path << "\n";
#endif
            }
        }
    }
    return true;
}


Button* UIManager::createButton(const std::string& name, RenderNode* const node, const ImVec2& position,
                                const std::string& text, std::function<void()> onClick, const ImVec2& constSize,
                                const ImVec2& padding, const ImVec4& normalColor, const ImVec4& hoverColor,
                                const ImVec4& activeColor, bool* canCloseWindow, ImGuiWindowFlags flags) {

    if (!buttons.contains(name)) {
        Button* button = new Button(sceneManager, node, position, text, onClick, constSize, padding, normalColor,
                                    hoverColor, activeColor, canCloseWindow, flags);
        buttons.insert({name, button});
        return button;
    }
    else {
#ifdef _DEBUG
        std::cout << "Ya existe el boton con el nombre " << name << '\n';
#endif
        return nullptr;
    }
}

Button* UIManager::createButton(const std::string& name, RenderNode* const node, const ImVec2& position,
                                const std::string& text, std::function<void()> onClick, const ImVec2& constSize,
                                const ImVec4& normalColor, const ImVec4& hoverColor, const ImVec4& activeColor,
                                bool* canCloseWindow, ImGuiWindowFlags flags) {
    return createButton(name, node, position, text, onClick, constSize, ImVec2(10, 5), normalColor, hoverColor,
                        activeColor, canCloseWindow, flags);
}

Button* UIManager::createButton(const std::string& name, RenderNode* const node, const ImVec2& position,
                                const std::string& text, std::function<void()> onClick,
                                const ImVec4& normalColor, const ImVec4& hoverColor, const ImVec4& activeColor,
                                bool* canCloseWindow, ImGuiWindowFlags flags) {
    return createButton(name, node, position, text, onClick, ImVec2(-1, -1), ImVec2(10, 5), normalColor, hoverColor,
                        activeColor, canCloseWindow, flags);
}

Button* UIManager::getButton(const std::string& name) {
    if (buttons.contains(name)) return buttons[name];
    else
        return nullptr;
}

bool UIManager::deleteButton(const std::string& name) {
    if (buttons.contains(name)) {
        delete buttons[name];
        buttons.erase(name);
        return true;
    }
    else {
#ifdef _DEBUG
        std::cout << "No existe el boton con el nombre " << name << '\n';
#endif
        return false;
    }
}
}
