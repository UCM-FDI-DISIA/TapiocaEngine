#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
#include "Utilities/Concepts.h"

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
    void start();

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
    template<IsComponent TComp>
    inline TComp* addComponent() {
        TComp* comp = new TComp();
        addComponent(comp, TComp::id);
        return comp;
    }
    Component* getComponent(std::string id);
    template<IsComponent TComp>
    inline TComp* getComponent() {
        auto it = components.find(TComp::id);
        if (it == components.end()) return nullptr; 
        return static_cast<TComp*>(it->second);
    }
    std::vector<Component*> getComponents(std::string id);
    template<IsComponent TComp>
    inline std::vector<TComp*> getComponents() {
        std::vector<TComp*> out;

        for (auto& comp : components) {
            if (comp.first == TComp::id) out.push_back(static_cast<TComp*>(comp.second));
        }

        return out;
    }
    void deleteComponent(std::string id);
    template<IsComponent TComp>
    void deleteComponent() { }


    void onCollisionEnter(GameObject* other);
    void onCollisionExit(GameObject* other);
    void onCollisionStay(GameObject* other);
};
}
