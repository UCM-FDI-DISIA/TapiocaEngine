#pragma once
#include <vector>
#include <stack>

namespace Tapioca {
class Scene;
class Module;

class Game {
private:
    static Game* instance;

    std::stack<Scene*> scenes;
    std::vector<Scene*> toDelete;
    std::vector<Module*> modules;
    uint64_t deltaTime;
    static const uint64_t MAX_NUM_FIXED_UDPATES = 150;
    bool finish;

    friend class Module;
    void addModule(Module*);

    void initComponents();
    void update();
    void handleEvents();
    void fixedUpdate();
    void render();
    void refresh();

public:
    Game();
    ~Game();
    inline static Game* get() { return instance; }

    static const uint64_t FIXED_DELTA_TIME = 1000 / 60;   // mas de 60 fps (62.5)

    void exit() { finish = true; }

    void init();
    void run();

    void pushScene(Scene*);
    void popScene();
    void changeScene(Scene*);
};
}