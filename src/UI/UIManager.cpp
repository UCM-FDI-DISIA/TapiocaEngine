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

#include "Structure/MainLoop.h"
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
    : mainLoop(nullptr), windowManager(nullptr), sdlWindow(nullptr), glContext(nullptr),
      ogreWindow(nullptr), renderListener(nullptr), scaleFactorX(1.0f), scaleFactorY(1.0f), fontsPath("assets/fonts/") {
}

UIManager::~UIManager() {
    mainLoop = nullptr;
    windowManager = nullptr;
    sdlWindow = nullptr;
    glContext = nullptr;
    ogreWindow = nullptr;
    delete renderListener;
    renderListener = nullptr;

    fonts.clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

bool UIManager::init() {
    mainLoop = MainLoop::instance();

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
    renderListener = new RenderListener(mainLoop);
    ogreWindow->addListener(renderListener);
    ImGui_ImplSDL2_InitForOpenGL(sdlWindow, glContext);
    ImGui_ImplOpenGL3_Init("#version 130");

    loadFonts();

    // TEMPORAL
    //renderListener->createImage("imagetest.PNG", Tapioca::Vector2(200, 200), Tapioca::Vector2(0, 200));
    //renderListener->createProgressBar(0.42, Vector4(00.2, 0.1, 0.6, 1), "42%", Vector2(200, 100), Vector2(10, 0));
    //renderListener->createSlider("slide", true, 13, 22, 0, Vector2(40, 120), Vector2(10, 80));
    //std::vector<std::string> s({"opcion", "otraopcion", "otramas"});
    //renderListener->createDropBox("dropbox", s, 0, Vector2(100, 60), Vector2(80, 120));

    return true;
}

bool UIManager::handleEvents(const SDL_Event& event) {
    if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(windowManager->getWindowW(), windowManager->getWindowH());

            float newWidth = (float)windowManager->getWindowW();
            float newHeight = (float)windowManager->getWindowH();
            float firstWidth = (float)windowManager->getFirstWindowW();
            float firstHeight = (float)windowManager->getFirstWindowH();
            if (!windowManager->getResized()) {
                scaleFactorX = (float)(newWidth / firstWidth);
                scaleFactorY = (float)(newHeight / firstHeight);
            }
            else
                scaleFactorX = scaleFactorY = 1.0f;

            //io.FontGlobalScale = std::min(scaleFactorX, scaleFactorY);

            return true;
        }
    }
    if (ImGui_ImplSDL2_ProcessEvent(&event) && ImGui::GetIO().WantCaptureMouse) {
        //#ifdef _DEBUG
        //        std::cout << "Interactuando con UI\n";
        //#endif
        return true;
    }
    return false;
}

bool UIManager::fontsFolderExists() {
    if (!std::filesystem::exists(fontsPath)) {
        logError(("UIManager: La carpeta \"" + fontsPath + " no existe.").c_str());
        return false;
    }
    return true;
}

void UIManager::loadFonts(float pixelSize) {

    if (!fontsFolderExists()) {
        logInfo("UIManager: No se han cargado las fuentes.");
    }

    for (const auto& entry : std::filesystem::directory_iterator(fontsPath)) {
        if (entry.is_regular_file()) {
            auto path = entry.path();
            std::string extension = path.extension().string();
            if (extension == ".ttf" || extension == ".TTF" || extension == ".otf" || extension == ".OTF")
                if (loadFont(path.stem().string() + extension, pixelSize) == nullptr) {
                    logError(("UIManager: No se ha podido cargar la fuente " + path.stem().string() + extension + '.').c_str());
                }
        }
    }
}

ImFont* UIManager::loadFont(const std::string& name, float pixelSize) {
    std::string path = fontsPath + name;
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF(path.c_str(), pixelSize);
    if (font == nullptr) {
        logError(("UIManager: No se pudo cargar la fuente \"" + name + "\".").c_str());
        return nullptr;
    }
    ImGui_ImplOpenGL3_CreateFontsTexture();

    fonts.insert({{name, pixelSize}, font});
    logInfo(("UIManager: Fuente \"" + name + "\" cargada correctamente.").c_str());
    return fonts[{name, pixelSize}];
}

ImFont* UIManager::getFont(const std::string& name, float pixelSize) {
    // Si el tamano de la fuente es la que hay por defecto y ya se ha cargado, se devuelve
    if (pixelSize == fontDefaultSize && fonts.contains({name, pixelSize})) return fonts[{name, pixelSize}];
    // Si el tamano de la fuente es la que hay por defecto y no se ha cargado, se devuelve nullptr
    else if (pixelSize == fontDefaultSize && !fonts.contains({name, pixelSize})) {
        logInfo(("UIManager: No existe la fuente con el nombre \"" + name + "\" con el tamano predeterminado de " +
                 std::to_string(fontDefaultSize) + ". Se usara la fuente por defecto.").c_str());
        return ImGui::GetIO().FontDefault;
    }
    // Si no es el tamano de fuente por defecto, se intenta cargar
    else {
        // Se comprueba si existe la carpeta de fuentes
        if (!fontsFolderExists()) {
            logInfo("UIManager: Se usara la fuente por defecto.");
            return ImGui::GetIO().FontDefault;
        }

        // Se intenta cargar la fuente
        if (!loadFont(name, pixelSize)) {
            logInfo("UIManager: Se usara la fuente por defecto.");
            return ImGui::GetIO().FontDefault;
        }
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
