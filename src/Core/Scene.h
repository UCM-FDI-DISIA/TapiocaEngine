#pragma once
#include <vector>
#include <string>
#include <unordered_map>

namespace Tapioca {
class GameObject;
class Vector;

class Scene {
private:
    friend class GameObject;
    void addObject(GameObject* object, std::string handler);
    std::vector<GameObject*> objects;
    std::unordered_map<std::string, GameObject*> handlers;

public:
    Scene();
    ~Scene();

    void refresh();
    GameObject* getHandler(const std::string& handler) const;

    void initComponent();
    void update();
    void handleEvents();
    // Pasar por mensaje --> void transitionEnded() { };
};
}