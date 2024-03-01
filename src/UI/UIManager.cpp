#include "UIManager.h"
#include <iostream>
#include <SDL.h>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"



Tapioca::UIManager::UIManager(SDL_Window* window) { myWindow = window; }

bool Tapioca::UIManager::init() {
    // Inicializar ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    SDL_GLContext gl_context = SDL_GL_CreateContext(myWindow);
    SDL_GL_SetSwapInterval(1);
    // Inicializar ImGui backends
    ImGui_ImplSDL2_InitForOpenGL(myWindow, gl_context);
   // ImGui_ImplOpenGL3_Init("#version 130");   // Reemplazar con la versión que queramos de GLSL


      // Nuevos frames de ImGui
    /* ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Ejemplo de ventana de demostración de ImGui
    ImGui::ShowDemoWindow();

    // Renderizar ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Intercambiar buffers
    SDL_GL_SwapWindow(myWindow);*/
    return true;
}
