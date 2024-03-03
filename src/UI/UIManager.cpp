#include "UIManager.h"

#include <SDL.h>
#include <SDL_video.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#ifdef _DEBUG
#include <iostream>
#endif

Tapioca::UIManager::UIManager()
    : myWindow(nullptr) { }

void Tapioca::UIManager::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

Tapioca::UIManager::~UIManager() { shutdown(); }

bool Tapioca::UIManager::init() {

    myWindow = Tapioca::GraphicsEngine::instance()->getSDLWindow();

    if (myWindow == nullptr) {
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
    gl_context = Tapioca::GraphicsEngine::instance()->getGLContext();
    SDL_GL_SetSwapInterval(1);
    // Inicializar ImGui backends
    ImGui_ImplSDL2_InitForOpenGL(myWindow, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");   // Reemplazar con la versión que queramos de GLSL

    return true;
}

//void Tapioca::UIManager::update(const uint64_t deltaTime) { }

//void Tapioca::UIManager::handleEvents() { 
//    SDL_Event event;
//    while (SDL_PollEvent(&event))
//        ImGui_ImplSDL2_ProcessEvent(&event);
//}

//void Tapioca::UIManager::fixedUpdate() { }

//void Tapioca::UIManager::render() {}

//void Tapioca::UIManager::refresh() { }
