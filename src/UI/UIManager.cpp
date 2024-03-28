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
#include "SDL_opengl.h"

#include "Structure/Game.h"
#include "Structure/Scene.h"
#include "WindowManager.h"

// TEMPORAL
#include "Components/Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "OgreGLTextureCommon.h"
#include "OgreGLTexture.h"
#include "OgreTextureManager.h"


namespace Tapioca {
template class TAPIOCA_API Singleton<UIManager>;
template<>
UIManager* Singleton<UIManager>::instance_ = nullptr;

UIManager::UIManager()
    : game(nullptr), windowManager(nullptr), sdlWindow(nullptr), glContext(nullptr), sceneManager(nullptr),
      fontsPath("assets/fonts/"), testid() { }

UIManager::~UIManager() {
    sdlWindow = nullptr;
    glContext = nullptr;
    sceneManager = nullptr;

    fonts.clear();

    // TEMPORAL
    for (auto image : images) {
        delete image.second;
    }
    images.clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) { render(); }

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
    ImGui_ImplSDL2_InitForOpenGL(sdlWindow, glContext);
    ImGui_ImplOpenGL3_Init("#version 130");

    loadFonts();
    
    // TEMPORAL
    createImage("imagetest.PNG", Tapioca::Vector2(300, 300), Tapioca::Vector2(200, 0));
    
    /*//pruebas de la imagen
    //sceneManager = graphics->getSceneManager();
    int w = 200;
    int h = 200;
    //el filename dpende del working directory al parecer
    //unsigned char* image_data = stbi_load("./imagetest.PNG", &w, &h, NULL, 4);//carga los datos de la imagen en bruto
    //pero luego para convertirloa una textura de opengl con esto daba errores de linkado muy turbios
    /* if (image_data == NULL) 
    {
        std::cout << "NO SE CARGO LA IMAGEN";
    }
    Ogre::TexturePtr p;
    try {

         p = Ogre::TextureManager::getSingleton().load("imagetest.PNG", "General");
    } catch (Ogre::Exception oe) {
         std::cout <<"ERROR AL CARGAR IMAGEN PARA INTERFAZ " << oe.getDescription();
    }
   // Ogre::Texture* t =  p.get();
    Ogre::GLTexture*  t = (Ogre::GLTexture*) p.get();
    
    GLuint* image_texture = new GLuint();// = p.getPointer().get;

    t->getCustomAttribute("GLID", image_texture);
    int imageHeigh= t->getHeight();
    int imageWidth = t->getWidth();
   
    testid = *image_texture;
    //pgetCustomAttribute("GLID", image_texture);
    //glGenTextures(1, &image_texture);
    //glBindTexture(GL_TEXTURE_2D, image_texture);
     //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    delete image_texture;*/

    //lo que he usado para probar que si no deja leaks
    // delete image_data;
   
    return true;
}

void UIManager::render() {
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (game->getTopScene() != nullptr) game->getTopScene()->render();

    // TEMPORAL
    for (auto image : images) {
        ImGui::SetNextWindowPos(ImVec2(image.second->getPosition().x, image.second->getPosition().y));
        ImGui::SetNextWindowSize(ImVec2(image.second->getSize().x, image.second->getSize().y ));
       ImGui::Begin(image.first.c_str(), image.second->getCanCloseWindow(), image.second->getFlags());
       // int* testid2 =new int( 1);
      
      
       //El tama�o de la ventana del mismo tama�o qeu laimagen para qeu se vea entera y no se corte 
      
       ImVec2 offset = ImGui::GetContentRegionMax(); // las dimensiones de la ventana no son las mismas qeu las del espacion donde se muestra contenido cuidado
       ImGui::Image((void*)(intptr_t)image.second->getID(),
                    ImVec2(image.second->getSize().x - (image.second->getSize().x - offset.x),
                           image.second->getSize().y - (image.second->getSize().y - offset.y)));
            
      //de esta manera siempre se ajusta la imagen al las dimensiones de la ventana
       ImGui::End();
    }
   

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UIManager::handleEvents(const SDL_Event& event) {
    /*    if (event.type == SDL_WINDOWEVENT) {
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            
        }
    }  */
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

// TEMPORAL
Image* UIManager::createImage(std::string file, Tapioca::Vector2 widthandheigth, Tapioca::Vector2 xandy,
                                ImGuiWindowFlags flags) {
    Image* image = new Image(file, widthandheigth, xandy, flags);

    images.insert({file, image});
    return image;
}
}
