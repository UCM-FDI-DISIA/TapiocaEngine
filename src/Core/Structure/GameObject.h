#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
using CompMap = std::unordered_map<std::string, std::variant<char, int, float, bool, std::string>>;

namespace Tapioca {
class Component;
class Scene;

class GameObject {
private:
    friend class Scene;
    void setScene(Scene*);

    void refresh();
    void update(const uint64_t deltaTime);
    void handleEvents();
    void initComponents(const CompMap& variables);
    void fixedUpdate();

    Scene* scene;
    bool alive;

    void deleteCompVector(Component* comp);
    std::unordered_multimap<std::string, Component*> components;
    std::vector<Component*> cmpOrder; // TODO Hace falta??????

public:
    GameObject();
    ~GameObject();

    inline bool isAlive() const { return alive; }
    inline void setAlive(bool alive) { this->alive = alive; }
    inline virtual Scene* getScene() const { return scene; }

    void addComponent(Component* comp, std::string id);

    template<typename Comp>
    Comp* addComponent() {
        Comp* comp = new Comp();
        addComponent(comp, Comp::id);
        return comp;
    }
    template<typename Comp>
    Comp* getComponent() {
        auto it = components.find(Comp::id);
        if (it == components.end()) {
            return nullptr;
        }
        return static_cast<Comp*>(it->second);
    }
    template<typename Comp>
    std::vector<Comp*> getComponents() {
        std::vector<Comp*> out;

        auto end = components.cend();
        for(auto it = components.cbegin(); it != end; ++it) {
            if(it->first == Comp::id) out.push_back(static_cast<Comp*>(it->second));
        }

        return out;
    }

    void onCollisionEnter(GameObject* other);
    void onCollisionExit(GameObject* other);
    void onCollisionStay(GameObject* other);
};
}
