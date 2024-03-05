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

    Ogre::RenderWindow* myOgreWindow;
    Ogre::ImGuiOverlay* imguiOverlay;

    UIManager();

    Ogre::ImGuiOverlay* initImgui();
    void shutdown();

public:
    ~UIManager();

    UIManager(UIManager&) = delete;
    UIManager(UIManager&&) = delete;
    UIManager& operator=(UIManager&) = delete;
    UIManager& operator=(UIManager&&) = delete;

    bool init() override;
    //void update(const uint64_t deltaTime);
    //void handleEvents() override;
    //void fixedUpdate() override;
    void render() override;
    //void refresh() override;
};
}