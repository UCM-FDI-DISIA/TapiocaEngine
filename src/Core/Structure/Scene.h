#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>
using CompMap = std::unordered_map<std::string, std::variant<char, int, float, bool, std::string>>;

namespace Tapioca {
class GameObject;

class Scene {
private:
    friend class GameObject;
    std::vector<GameObject*> objects;
    std::unordered_map<std::string, GameObject*> handlers;

public:
    Scene();
    ~Scene();

    std::vector<GameObject*> getObjects() const;
    GameObject* getHandler(const std::string& handler) const;

    void addObject(GameObject* object, std::string const& handler = "");
    void start();
    void update(const uint64_t deltaTime);
    //void handleEvents();
    void fixedUpdate();
    void refresh();
    void pushEvent(std::string const& id, void* info);
};
}
