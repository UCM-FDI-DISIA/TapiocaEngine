#include "GameObject.h"
#include "Scene.h"
#include "Component.h"

namespace Tapioca {
    GameObject::GameObject(Scene* scene, std::string handler)
        : scene(scene)
        , alive(true) {
        scene->addObject(this, handler);
    }

    GameObject::~GameObject() {
        for (auto& i : components)
            delete i.second;
    }

    void GameObject::addComponent(Component* comp, std::string id) {
        auto it = components.find(id);




        if (it != components.end()) {
            deleteCompVector(it->second);
            delete it->second;
        }
        components[id] = comp;
        cmpOrder.push_back(comp);
    }

    void GameObject::deleteCompVector(Component* comp) {
        for (auto it = cmpOrder.cbegin(); it != cmpOrder.cend(); ++it) {
            if (*it == comp) {
                cmpOrder.erase(it);
                break;
            }
        }
    }

    void GameObject::refresh() {
        auto it = components.begin();
        while (it != components.end()) {
            if (!it->second->isAlive()) {
                deleteCompVector(it->second);
                delete it->second;
                it = components.erase(it);
            } else
                ++it;
        }
    }

    void GameObject::update() {
        for (auto comp : cmpOrder)
            if (comp->isActive()) comp->update();
    }

    void GameObject::handleEvents() {
        for (auto comp : cmpOrder)
            if (comp->isActive()) comp->handleEvents();
    }

    void GameObject::initComponent() {
        for (auto comp : cmpOrder)
            comp->initComponent();
    }
}