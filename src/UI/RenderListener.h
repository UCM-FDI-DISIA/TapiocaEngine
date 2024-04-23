#pragma once
#ifdef _MSC_VER
#pragma warning(disable : 4251)
#endif
#include <OgreRenderTargetListener.h>
#ifdef _MSC_VER
#pragma warning(default : 4251)
#endif

namespace Tapioca {
class MainLoop;
class WindowManager;

/*
* @brief Clase que se encarga de renderizar cuando Ogre termine de renderizar en el viewport
*/
class RenderListener : public Ogre::RenderTargetListener {
private:
    MainLoop* mainLoop;             // Referencia al MainLoop
    WindowManager* windowManager;   // Referencia al WindowManager

public:
    /*
    * @brief Inicializa el puntero al MainLoop
    * @param mainLoop Puntero al MainLoop
    */
    RenderListener(MainLoop* mainLoop);

    /*
    * @brief Pone a nullptr el puntero al MainLoop y al WindowManager
    */
    ~RenderListener();

    /*
    * @brief Se llama cuando Ogre termina de renderizar en el viewport
    * @param evt Evento de Ogre
    */
    void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt) override;
};
}
