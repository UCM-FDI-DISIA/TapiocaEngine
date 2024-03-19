#pragma once
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
// warnings de ogre
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <OgreRenderTargetListener.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

struct SDL_Window;

namespace Ogre {
class RenderWindow;
}

namespace Tapioca {
/**
* @brief Clase que se encarga de la interfaz de usuario
*/
class TAPIOCA_API UIManager : public Singleton<UIManager>, public Module, public Ogre::RenderTargetListener {
private:
    friend Singleton<UIManager>;

    SDL_Window* mySDLWindow;            // Referencia a la ventana de SDL
    Ogre::RenderWindow* myOgreWindow;   // Referencia a la ventana de Ogre
    void* myGLContext;                  // Referencia al contexto de OpenGL

    // PRUEBA
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
    * @brief Se llama cuando Ogre termine de renderizar en el viewport al que hemos asignado el UIManager como listener
    */
    void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) override;

    /**
    * @brief Inicializa ImGui con SDL y OpenGL, y anade el UIManager como listener de la ventana de Ogre
    */
    bool init() override;

    /**
    * @brief Renderiza la interfaz de usuario
    */
    void render() override;
};
}