#pragma once
#include <vector>
#include "Utilities/defs.h"

namespace Tapioca {
class GameObject;

class TAPIOCA_API Scene {
private:
    friend class GameObject;
    std::vector<GameObject*> objects;
    std::unordered_map<std::string, GameObject*> handlers;

public:
    Scene();
    ~Scene();

    std::vector<GameObject*> getObjects() const;
    GameObject* getHandler(const std::string& handler) const;

    bool addObject(GameObject* object, std::string const& handler = "");
    void start();
    void update(const uint64_t deltaTime);
    //void handleEvents();
    void fixedUpdate();
    void refresh();
    void pushEvent(std::string const& id, void* info);
};
}
