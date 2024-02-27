#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <variant>
#include <Windows.h>
#include "Utilities/checkML.h"
#include "Utilities/Singleton.h"
#include "Structure/Module.h"
#include "Structure/Scene.h"
#include "Structure/Game.h"
#include "Structure/GameObject.h"
#include "Components/Transform.h"
using namespace std;
using CompMap = std::unordered_map<std::string, std::variant<char, int, float, bool, std::string>>;
typedef void(__cdecl* EntryPoint)(const char*);

namespace Tapioca {
class SceneManager : public Tapioca::Singleton<SceneManager>, public Module {
private:
    friend Singleton<SceneManager>;

    friend class Scene;
    void addScene(Scene*);

    HMODULE module;
    EntryPoint entryPoint;
    stack<Scene*> scenes;
    vector<Scene*> toDelete;

    SceneManager(HMODULE module);

    bool init() override;

public:
    ~SceneManager();

    SceneManager(SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

    void initComponents(const CompMap& variables);
    void update(const uint64_t deltaTime) override;
    void handleEvents() override;
    void fixedUpdate() override;
    void refresh() override;
    void pushScene(Scene*);
    void popScene();
    void changeScene(Scene*);
};
}