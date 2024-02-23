#pragma once
#include <unordered_map>
#include <vector>
#include <string>

namespace Tapioca {
class Component;
class Scene;

class GameObject {
private:
    friend class Scene;
    void refresh();
    void update();
    void handleEvents();
    void initComponent();
    void fixedUpdate();

    Scene* scene;
    bool alive;

    void deleteCompVector(Component* comp);
    std::unordered_map<std::string, Component*> components;
    std::vector<Component*> cmpOrder; // TODO Hace falta??????
public:
    GameObject(Scene* scene, std::string handler = "");
    ~GameObject();

    inline bool isAlive() const { return alive; }
    inline void setAlive(bool alive) { this->alive = alive; }
    inline virtual Scene* getScene() const { return scene; }
    
    template<typename Comp> inline Comp* getComponent() {
        auto it = components.find(Comp::id);
        if (it == components.end()) {
            return nullptr;
        }
        return static_cast<Comp*>(it->second);
    }

    // No tocar :3
    void addComponent(Component* comp, std::string id);
};
}
