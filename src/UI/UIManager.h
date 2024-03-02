#pragma once
#include "Structure/Module.h"
#include "Utilities/Singleton.h"
#include "GraphicsEngine.h"
class SDL_Window;

namespace Tapioca {
class UIManager : public Singleton<UIManager>, public Module {
private:
    friend Singleton<UIManager>;

    SDL_Window* myWindow;

    UIManager();

    void shutdown();

public:
    ~UIManager();

    UIManager(UIManager&) = delete;
    UIManager(UIManager&&) = delete;
    UIManager& operator=(UIManager&) = delete;
    UIManager& operator=(UIManager&&) = delete;

    bool init() override;
    //void update(const uint64_t deltaTime) { }
    void handleEvents() override { }
    //void fixedUpdate() override { }
    void render() override { }
    //void refresh() override { }
};
}