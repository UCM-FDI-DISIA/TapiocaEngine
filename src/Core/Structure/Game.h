#pragma once
#include <vector>
#include <stack>
#include "Utilities/Singleton.h"
#include "Utilities/defs.h"

namespace Tapioca {
class Scene;
class Module;

class TAPIOCA_API Game : public Singleton<Game> {
private:
    friend Singleton<Game>;

    std::vector<Module*> modules;
    std::stack<Scene*> scenes;
    std::vector<Scene*> toDelete;

    uint64_t deltaTime;
    static const uint64_t MAX_NUM_FIXED_UDPATES = 150;
    bool finish;


    void start();
    void update();
    void handleEvents();
    void fixedUpdate();
    void render();
    void refresh();

    Game();

public:
    ~Game();

    static const uint64_t FIXED_DELTA_TIME = 1000 / 60;   // mas de 60 fps (62.5)

    inline void exit() { finish = true; }

    bool init();
    void run();

    void pushScene(Scene* const sc);
    void popScene();
    void changeScene(Scene* const sc);

    
    void addModule(Module* const m);

    void pushEvent(std::string const& id, void* info);
};
}