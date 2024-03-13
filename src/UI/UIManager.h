#pragma once
#include "GraphicsEngine.h"
#include "Structure/Module.h"
#include "Utilities/Singleton.h"
#include <OgreFrameListener.h>
#include <OgreRenderTargetListener.h>

class SDL_Window;

namespace Ogre {
class RenderWindow;
class ImGuiOverlay;
}

namespace Tapioca {
class FactoryManager;

/**
* @brief Clase que se encarga de la interfaz de usuario
*/
class TAPIOCA_API UIManager : public Singleton<UIManager>, public Module, public Ogre::RenderTargetListener {
private:
    friend Singleton<UIManager>;

    SDL_Window* mySDLWindow;            // Referencia a la ventana de SDL
    Ogre::RenderWindow* myOgreWindow;   // Referencia a la ventana de Ogre
    void* myGLContext;                  // Referencia al contexto de OpenGL
    Ogre::ImGuiOverlay* imguiOverlay;   // Overlay de ImGui
    const char* button;                 // Texto de Boton de Play

    /**
    * @brief Inicializa a nulo los punteros
    */
    UIManager();

public:
    /**
    * @brief Termina ImGui con SDL y OpenGL y destruye el contexto
    */
    ~UIManager();

    UIManager(UIManager&) = delete;
    UIManager(UIManager&&) = delete;
    UIManager& operator=(UIManager&) = delete;
    UIManager& operator=(UIManager&&) = delete;
    /*
    * @brief cuando Ogre termine de renderizar en el viewport al que hemos asignado el uiManager como listener , ejecutara este callback
    */
    void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) override;
    /**
    * @brief Inicializa ImGui con SDL y OpenGL, y crea el overlay con la interfaz
    */
    bool init() override;

    /**
    * @brief Renderiza la interfaz de usuario
    */
    void render() override;
};
}