#pragma once
#include "GraphicsEngine.h"
#include "Structure/Module.h"
#include "Utilities/Singleton.h"
#include <OgreFrameListener.h>
#include <OgreRenderTargetListener.h>

class SDL_Window;
namespace Ogre {
class ImGuiOverlay;
}

namespace Tapioca {
/**
* @brief Clase que se encarga de la interfaz de usuario
*/
class TAPIOCA_API UIManager : public Singleton<UIManager>, public Module, public Ogre::RenderTargetListener {
private:
    friend Singleton<UIManager>;

    Ogre::RenderWindow* myOgreWindow;   // Referencia a la ventana de Ogre
    Ogre::ImGuiOverlay* imguiOverlay;   // Overlay de ImGui

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