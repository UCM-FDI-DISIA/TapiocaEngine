#pragma once
#include <vector>
#include <stack>
#include <string>
#include <Windows.h>

typedef void(__cdecl* EntryPoint)(const char*);

namespace Tapioca {
class Scene;
class Module;

class Game {
private:
    static Game* instance;

    // TODO Carga de dll, debería moverse a otro lugar
    std::string gameName;
    std::string gamePath;
    HMODULE module;
    EntryPoint entryPoint;

    std::stack<Scene*> scenes;   // TODO manejo de escenas temporal
    std::vector<Module*> modules;
    uint32_t deltaTime;
    static const uint32_t FIXED_DELTA_TIME = 1000 / 60; // 60 fps
    uint32_t startTime;
    uint32_t fixedOffset;
    bool cont;

    friend class Scene;
    void addScene(Scene*);
    friend class Module;
    void addModule(Module*);

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
    void fixedUpdate();
    void render();
};
}