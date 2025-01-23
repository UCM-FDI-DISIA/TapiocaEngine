#include "UIManager.h"
#include <filesystem>
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <OgreRenderWindow.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <SDL_opengl.h>
#include "OgreTextureManager.h"
#include "RenderListener.h"
#include "Structure/MainLoop.h"
#include "WindowManager.h"
#include "GraphicsManager.h"
#include "checkML.h"

namespace Tapioca {
UIManager::UIManager()
    : mainLoop(nullptr), windowManager(nullptr), graphicsManager(nullptr), sdlWindow(nullptr), glContext(nullptr),
      ogreWindow(nullptr), renderListener(nullptr), fontsPath("assets/fonts/"), texturesPath("assets/textures/"),
      widgetCounter(0) { }

UIManager::~UIManager() {
    mainLoop = nullptr;
    windowManager = nullptr;
    graphicsManager = nullptr;
    sdlWindow = nullptr;
    glContext = nullptr;
    ogreWindow = nullptr;
    if (renderListener != nullptr) {
        delete renderListener;
        renderListener = nullptr;
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
    fonts.clear();
}

bool UIManager::init() {
    mainLoop = MainLoop::instance();
    windowManager = WindowManager::instance();
    graphicsManager = GraphicsManager::instance();
    return mainLoop != nullptr && windowManager != nullptr && graphicsManager != nullptr;
}

bool UIManager::initConfig() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    io.WantCaptureKeyboard = true;
    io.WantCaptureMouse = true;
    sdlWindow = windowManager->getWindow();
    glContext = windowManager->getGLContext();
    ogreWindow = graphicsManager->getOgreRenderTarget();
    renderListener = new RenderListener(mainLoop);
    ogreWindow->addListener(renderListener);
    ImGui_ImplSDL2_InitForOpenGL(sdlWindow, glContext);
    ImGui_ImplOpenGL3_Init("#version 130");
    check();
    return true;
}
bool UIManager::handleEvents(const SDL_Event& event) {
    if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2((float)windowManager->getWindowW(), (float)windowManager->getWindowH());
            return true;
        }
    }
    if (ImGui_ImplSDL2_ProcessEvent(&event) && ImGui::GetIO().WantCaptureMouse) return true;
    return false;
}

void UIManager::check() const {
    if (!fontsFolderExists()) {
        try {
            if (std::filesystem::create_directory(fontsPath)) {
                logInfo("UIManager: Carpeta de fuentes creada correctamente.");
            }
        } catch (const std::filesystem::filesystem_error& e) {
            logError(("UIManager: No se pudo crear la carpeta de fuentes. " + std::string(e.what())).c_str());
        }
    }
    if (!texturesFolderExists()) {
        try {
            if (std::filesystem::create_directory(texturesPath)) {
                logInfo("UIManager: Carpeta de imagenes creada correctamente.");
            }
        } catch (const std::filesystem::filesystem_error& e) {
            logError(("UIManager: No se pudo crear la carpeta de imagenes. " + std::string(e.what())).c_str());
        }
    }
}

bool UIManager::fontsFolderExists() const {
    if (!std::filesystem::exists(fontsPath)) {
        logWarn(("UIManager: La carpeta \"" + fontsPath + " no existe.").c_str());
        return false;
    }
    return true;
}

bool UIManager::texturesFolderExists() const {
    if (!std::filesystem::exists(texturesPath)) {
        logWarn(("UIManager: La carpeta \"" + texturesPath + "\" no existe.").c_str());
        return false;
    }
    return true;
}

bool UIManager::fontsFolderEmpty() const {
    if (std::filesystem::directory_iterator(fontsPath) == std::filesystem::directory_iterator {}) {
        logWarn(("UIManager: La carpeta \"" + fontsPath + "\" esta vacia.").c_str());
        return true;
    }
    return false;
}

ImFont* UIManager::loadFont(const std::string& name, float pixelSize) {
    std::string path = fontsPath + name;
    if (!fileExists(path)) return nullptr;

    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF(path.c_str(), pixelSize);
    if (font == nullptr) {
        logError(("UIManager: No se pudo cargar la fuente \"" + path + "\".").c_str());
        return nullptr;
    }
    ImGui_ImplOpenGL3_CreateFontsTexture();

    fonts.insert({{name, pixelSize}, font});
    logInfo(("UIManager: Fuente \"" + name + "\" cargada correctamente.").c_str());
    return fonts[{name, pixelSize}];
}

ImFont* UIManager::getFont(const std::string& name, float pixelSize) {
    if (fonts.contains({name, pixelSize})) return fonts[{name, pixelSize}];
    // Se comprueba si existe la carpeta de fuentes o si esta vacia
    if (!fontsFolderExists() || fontsFolderEmpty() || name == "") {
        logInfo("UIManager: Se usara la fuente por defecto.");
        return ImGui::GetIO().FontDefault;
    }
    // Se intenta cargar la fuente
    if (loadFont(name, pixelSize) == nullptr) {
        logInfo("UIManager: Se usara la fuente por defecto.");
        return ImGui::GetIO().FontDefault;
    }
    return fonts[{name, pixelSize}];
}

ImTextureID UIManager::getTextureId(const std::string& name) {
    try {
        std::string path = "textures/" + name;
        Ogre::TexturePtr texturePtr = Ogre::TextureManager::getSingleton().load(path, "General");
        GLuint glID;
        texturePtr->getCustomAttribute("GLID", &glID);
        return reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(glID));
    } catch (...) {
        logError(("UIManager: No se pudo cargar la textura " + name + '.').c_str());
        return 0;
    }
}

std::string UIManager::generateName(const std::string& widget) { return widget + std::to_string(widgetCounter++); }

void UIManager::addWidgetName(std::string& name) {
    std::string nameToAdd = name;
    if (widgetNameExists(name) || name == "") {
        nameToAdd = generateName(name);
        if (name == "") {
            logWarn(("UIManager: El widget \"" + name + "\" no se permite.").c_str());
            logWarn(("UIManager: El widget recibira el nombre de \"" + nameToAdd + "\".").c_str());
        }
        else {
            logWarn(("UIManager: El nombre de widget \"" + name + "\" ya existe.").c_str());
            logWarn(("UIManager: El widget \"" + name + "\" recibira el nombre de \"" + nameToAdd + "\".").c_str());
        }
    }
    widgetNames.insert(nameToAdd);
    name = nameToAdd;
}

void UIManager::removeWidgetName(const std::string& name) {
    if (!widgetNames.contains(name)) logWarn(("UIManager: El nombre de widget \"" + name + "\" no existe.").c_str());
    else widgetNames.erase(name);
}

bool UIManager::widgetNameExists(const std::string& name) { return widgetNames.contains(name); }

bool UIManager::fileExists(const std::string& name) const { return std::filesystem::exists(name); }

UIManager::ScaledSize UIManager::getScaledSize(const int& x, const int& y, const int& sizeX, const int& sizeY) const {
    float scaleX = windowManager->getsScaleX();
    float scaleY = windowManager->getsScaleY();

    float scaledW = scaleX * sizeX;
    float scaledH = scaleY * sizeY;

    float scaledPosX = x * scaleX - scaledW / 2.0f;
    float scaledPosY = y * scaleY - scaledH / 2.0f;

    /*std::cout << scaleX << ' ' << scaleY << '\n';
    std::cout << x << ' ' << y << ' ' << sizeX << ' ' << sizeY << '\n';
    std::cout << scaledPosX << ' ' << scaledPosY << ' ' << scaledW << ' ' << scaledH << '\n';*/

    return {scaledPosX, scaledPosY, scaledW, scaledH};
}

float UIManager::getScaleX() const { 
    return windowManager->getsScaleX(); 
}
float UIManager::getScaleY() const { 
    return windowManager->getsScaleY(); 
}

}
