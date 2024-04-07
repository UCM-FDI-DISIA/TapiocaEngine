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
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <SDL_opengl.h>

#include "Structure/Game.h"
#include "Structure/Scene.h"
#include "WindowManager.h"
#include "GraphicsManager.h"
#include "RenderListener.h"

// TEMPORAL
#include "Components/Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "OgreGLTextureCommon.h"
#include "OgreGLTexture.h"
#include "OgreTextureManager.h"

#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/DropBox.h"


namespace Tapioca {
template class TAPIOCA_API Singleton<UIManager>;
template<>
UIManager* Singleton<UIManager>::instance_ = nullptr;

UIManager::UIManager()
    : game(nullptr), windowManager(nullptr), sdlWindow(nullptr), glContext(nullptr), sceneManager(nullptr),
      ogreWindow(nullptr), renderListener(nullptr), scaleFactorX(1.0f), scaleFactorY(1.0f), fontsPath("assets/fonts/") { }

UIManager::~UIManager() {
    game = nullptr;
    windowManager = nullptr;
    sdlWindow = nullptr;
    glContext = nullptr;
    sceneManager = nullptr;
    ogreWindow = nullptr;
    delete renderListener;
    renderListener = nullptr;

    fonts.clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

bool UIManager::init() {
    game = Game::instance();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    io.WantCaptureKeyboard = true;
    io.WantCaptureMouse = true;
    windowManager = WindowManager::instance();
    sdlWindow = windowManager->getWindow();
    glContext = windowManager->getGLContext();
    ogreWindow = GraphicsManager::instance()->getOgreRenderTarget();
    renderListener = new RenderListener(game);
    ogreWindow->addListener(renderListener);
    ImGui_ImplSDL2_InitForOpenGL(sdlWindow, glContext);
    ImGui_ImplOpenGL3_Init("#version 130");

    loadFonts();

    // TEMPORAL
    renderListener->createImage("imagetest.PNG", Tapioca::Vector2(200, 200), Tapioca::Vector2(0, 200));
    renderListener->createProgressBar(0.42, Vector4(00.2, 0.1, 0.6, 1), "42%", Vector2(200, 100), Vector2(10, 0));
    renderListener->createSlider("slide", true, 13, 22, 0, Vector2(40, 120), Vector2(10, 80));
    std::vector<std::string> s({"opcion", "otraopcion", "otramas"});
    renderListener->createDropBox("dropbox", s, 0, Vector2(100, 60), Vector2(80, 120));

    return true;
}

bool UIManager::handleEvents(const SDL_Event& event) {
    if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            ImGuiIO& io= ImGui::GetIO();
            io.DisplaySize = ImVec2(windowManager->getWindowW(), windowManager->getWindowH());

            scaleFactorX = (float)windowManager->getWindowW() / (float)windowManager->getLastWindowW();
            scaleFactorY = (float)windowManager->getWindowH() / (float)windowManager->getLastWindowH();
            io.FontGlobalScale = std::min(scaleFactorX, scaleFactorY);

            return true;
        }
    }
    if (ImGui_ImplSDL2_ProcessEvent(&event) &&
        (ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive() || ImGui::IsAnyItemFocused())) {
        //#ifdef _DEBUG
        //        std::cout << "Interactuando con UI\n";
        //#endif
        return true;
    }
    return false;
}

bool UIManager::fontsFolderExists() {
    if (!std::filesystem::exists(fontsPath)) {
#ifdef _DEBUG
        std::cout << "La carpeta " << fontsPath << " no existe\n";
#endif
        return false;
    }
    return true;
}

void UIManager::loadFonts(float pixelSize) {

    if (!fontsFolderExists()) {
#ifdef _DEBUG
        std::cout << "No se han cargado las fuentes\n";
#endif
        return;
    }

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
}

void UIManager::loadFont(const std::string& name, float pixelSize) {
    std::string path = fontsPath + name;
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF(path.c_str(), pixelSize);
    fonts.insert({{name, pixelSize}, font});
#ifdef _DEBUG
    std::cout << "Fuente " << name << " cargada correctamente\n";
#endif
}

ImFont* UIManager::getFont(const std::string& name, float pixelSize) {
    // Si el tamano de la fuente es la que hay por defecto y ya se ha cargado, se devuelve
    if (pixelSize == fontDefaultSize && fonts.contains({name, pixelSize})) return fonts[{name, pixelSize}];
    // Si el tamano de la fuente es la que hay por defecto y no se ha cargado, se devuelve nullptr
    else if (pixelSize == fontDefaultSize && !fonts.contains({name, pixelSize})) {
#ifdef _DEBUG
        std::cout << "No existe la fuente con el nombre " << name << " con el tamano predeterminado de "
                  << fontDefaultSize << '\n';
        std::cout << "Se usara la fuente por defecto\n";
#endif
        return ImGui::GetIO().FontDefault;
    }
    // Si no es el tamano de fuente por defecto, se intenta cargar
    else {
        // Se comprueba si existe la carpeta de fuentes
        if (!fontsFolderExists()) {
#ifdef _DEBUG
            std::cout << "Se usara la fuente por defecto\n";
#endif
            return ImGui::GetIO().FontDefault;
        }

        // Se intenta cargar la fuente
        loadFont(name, pixelSize);
        return fonts[{name, pixelSize}];
    }
}

ImTextureID UIManager::getTextureId(const std::string& name) {
    Ogre::TexturePtr texturePtr = Ogre::TextureManager::getSingleton().load(name, "General");
    if (texturePtr == nullptr) {
        throw Ogre::Exception(Ogre::Exception::ERR_FILE_NOT_FOUND, "No se encontro la textura " + name,
                              "UIManager::getTextureId");
        return 0;
    }

    GLuint glID;
    texturePtr->getCustomAttribute("GLID", &glID);
    return (ImTextureID)glID;
}
}
