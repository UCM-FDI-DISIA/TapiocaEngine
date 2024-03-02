#include "UIManager.h"
#include <iostream>
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.cpp>

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
        std::cerr << "Error al inicializar ImGui: " << SDL_GetError() << '\n';
        return false;
    }

    // Inicializar ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    SDL_GLContext gl_context = SDL_GL_CreateContext(myWindow);
    SDL_GL_SetSwapInterval(1);
    // Inicializar ImGui backends
    ImGui_ImplSDL2_InitForOpenGL(myWindow, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");   // Reemplazar con la versión que queramos de GLSL

    return true;
}

//void Tapioca::UIManager::update(const uint64_t deltaTime) { }

void Tapioca::UIManager::handleEvents() { 
    SDL_Event event;
    while (SDL_PollEvent(&event))
        ImGui_ImplSDL2_ProcessEvent(&event);
}

//void Tapioca::UIManager::fixedUpdate() { }

void Tapioca::UIManager::render() {
    // Nuevos frames de ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    
    // Ejemplo de ventana de demostración de ImGui
    ImGui::ShowDemoWindow();
    
    // Renderizar ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Intercambiar buffers
    SDL_GL_SwapWindow(myWindow);
}

//void Tapioca::UIManager::refresh() { }
