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
class UIManager : public Singleton<UIManager>, public Module, public Ogre::RenderTargetListener {

private:
    friend Singleton<UIManager>;

    SDL_Window* myWindow;
    Ogre::RenderWindow* myogreWindow;
    Ogre::ImGuiOverlay* imguiOverlay;
    void* gl_context;

    UIManager();
    //bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    void shutdown();
    Ogre::ImGuiOverlay* initImgui();

public:
    ~UIManager();

    UIManager(UIManager&) = delete;
    UIManager(UIManager&&) = delete;
    UIManager& operator=(UIManager&) = delete;
    UIManager& operator=(UIManager&&) = delete;

    bool init() override;
    //void preViewportUpdate(const Ogre:: RenderTargetViewportEvent& evt) override;
    //void update(const uint64_t deltaTime) { }
    //void handleEvents() override;
    //void fixedUpdate() override { }
    // void render() override;
    //void refresh() override { }
};
}