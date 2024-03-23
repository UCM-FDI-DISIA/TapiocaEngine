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


namespace Tapioca {
template class TAPIOCA_API Singleton<UIManager>;
template<>
UIManager* Singleton<UIManager>::instance_ = nullptr;

UIManager::UIManager() : sdlWindow(nullptr), glContext(nullptr), sceneManager(nullptr), fontsPath("assets/fonts/") { }

UIManager::~UIManager() {
    sdlWindow = nullptr;
    glContext = nullptr;
    sceneManager = nullptr;

    for (auto button : buttons) {
        delete button.second;
    }
    buttons.clear();
    fonts.clear();

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

    loadFonts();

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
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());

        ImGui::Begin(text, button.second->getCanCloseWindow(), button.second->getFlags());

        ImGui::PopStyleVar(2);   // Pop para WindowBorderSize y WindowPadding

        // Establece la fuente del texto
        ImGui::PushFont(button.second->getFont());
        // Establece el color del texto
        ImGui::PushStyleColor(ImGuiCol_Text, button.second->getTextColor());
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
        ImGui::PopStyleColor(5);
        // Pop para la fuente del texto
        ImGui::PopFont();

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UIManager::handleEvents(const SDL_Event& event) { return ImGui_ImplSDL2_ProcessEvent(&event); }

bool UIManager::fontsFolderExists() {
    if (!std::filesystem::exists(fontsPath)) {
#ifdef _DEBUG
        std::cout << "La carpeta " << fontsPath << " no existe\n";
#endif
        return false;
    }
    return true;
}

bool UIManager::loadFonts(float pixelSize) {

    if (!fontsFolderExists()) return false;

    for (const auto& entry : std::filesystem::directory_iterator(fontsPath)) {
        if (entry.is_regular_file()) {
            auto path = entry.path();
            std::string extension = path.extension().string();
            std::transform(extension.begin(), extension.end(), extension.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            if (extension == ".ttf" || extension == ".otf") {
                loadFont(path.stem().string() + extension, pixelSize);
            }
        }
    }
    return true;
}

void UIManager::loadFont(const std::string& name, float pixelSize) {
    std::string path = fontsPath + name;
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF(path.c_str(), pixelSize);
    fonts.insert({{name, pixelSize}, font});
}

ImFont* UIManager::getFont(const std::string& name, float pixelSize) {
    // Si el tamano de la fuente es la que hay por defecto y ya se ha cargado, se devuelve
    if (pixelSize == fontDefaultSize && fonts.contains({name, pixelSize})) return fonts[{name, pixelSize}];
    // Si el tamano de la fuente es la que hay por defecto y no se ha cargado, se devuelve nullptr
    else if (pixelSize == fontDefaultSize && !fonts.contains({name, pixelSize})) {
#ifdef _DEBUG
        std::cout << "No existe la fuente con el nombre " << name << " con el tamano predeterminado de "
                  << fontDefaultSize << '\n';
#endif
        return nullptr;
    }
    // Si no es el tamano de fuente por defecto, se intenta cargar
    else {
        // Se comprueba si existe la carpeta de fuentes
        if (!fontsFolderExists()) return nullptr;

        // Se intenta cargar la fuente
        loadFont(name, pixelSize);
        return fonts[{name, pixelSize}];
    }
}

Button* UIManager::createButton(const std::string& name, const Button::ButtonOptions& options) {
    return createButton(name, options.node, options.position, options.text, options.onClick, options.constSize,
                        options.textFont, options.textColor, options.normalColor, options.hoverColor,
                        options.activeColor, options.canCloseWindow, options.flags);
}

Button* UIManager::createButton(const std::string& name, RenderNode* const node, const ImVec2& position,
                                const std::string& text, std::function<void()> onClick, const ImVec2& constSize,
                                const ImVec2& padding, ImFont* const textFont, const ImVec4& textColor,
                                const ImVec4& normalColor, const ImVec4& hoverColor, const ImVec4& activeColor,
                                bool* canCloseWindow, ImGuiWindowFlags flags) {

    if (!buttons.contains(name)) {
        Button* button = new Button(sceneManager, node, position, text, onClick, constSize, padding, textFont,
                                    textColor, normalColor, hoverColor, activeColor, canCloseWindow, flags);
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
                                ImFont* const textFont, const ImVec4& textColor, const ImVec4& normalColor,
                                const ImVec4& hoverColor, const ImVec4& activeColor, bool* canCloseWindow,
                                ImGuiWindowFlags flags) {
    return createButton(name, node, position, text, onClick, constSize, ImVec2(10, 5), textFont, textColor, normalColor,
                        hoverColor, activeColor, canCloseWindow, flags);
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
