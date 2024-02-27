#include <stack>
#include <Windows.h>
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
#include "Structure/Scene.h"
using namespace std;

namespace Tapioca {
class SceneManager : public Tapioca::Singleton<SceneManager>, public Module {
private:
    friend Singleton<SceneManager>;

    friend class Scene;
    void addScene(Scene*);

    HMODULE module;
    std::stack<Scene*> scenes;
    std::vector<Scene*> toDelete;

    SceneManager(HMODULE module);

    void init() override;

public:
    ~SceneManager();

    SceneManager(SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

    void initComponents();
    /*void update();
    void handleEvents();
    void fixedUpdate();
    void refresh();*/
    void pushScene(Scene*);
    void popScene();
    void changeScene(Scene*);
};
}