#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>

typedef void(__cdecl* EntryPoint)(const char*);

namespace Tapioca {
class Scene;

class Game {
private:
    static Game* instance;

    std::string gameName;
    std::string gamePath;
    HMODULE module;
    EntryPoint entryPoint;

    std::vector<Scene*> scenes;   // TODO manejo de escenas temporal
    uint32_t deltaTime;
    const uint32_t FIXED_DELTA_TIME = 16;

public:
    Game(std::string const& gameName);
    ~Game();
    inline static Game* get() { return instance; }

    inline const uint32_t& getDeltaTime() const { return deltaTime; }
    inline const uint32_t& getFixedDeltaTime() const { return FIXED_DELTA_TIME; }

    bool setup();
    void init();
    void run();
    void initComponents();
    void update();
    void handleEvents();
};
}