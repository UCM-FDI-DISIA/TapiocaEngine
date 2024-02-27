#pragma once
#include <vector>
#include <string>
#include <unordered_map>

namespace Tapioca {
class GameObject;

class Scene {
private:
    friend class GameObject;
    void addObject(GameObject* object, std::string handler);
    std::vector<GameObject*> objects;
    std::unordered_map<std::string, GameObject*> handlers;

public:
    Scene();
    ~Scene();

    GameObject* getHandler(const std::string& handler) const;

    void refresh();
    void initComponent();
    void update(const uint64_t deltaTime);
    void handleEvents();
    void fixedUpdate();
};
}
