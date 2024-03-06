#include "GameObject.h"
#include "Scene.h"
#include "Component.h"

namespace Tapioca {
GameObject::GameObject() : scene(nullptr), alive(true), handler("") { }

GameObject::~GameObject() {
    for (auto& i : components)
        delete i.second;
}

void GameObject::addComponent(Component* const comp, std::string const& id) {
    components.insert(std::pair<std::string, Component*>(id, comp));
    cmpOrder.push_back(comp);
    comp->object = this;
}

Component* GameObject::getComponent(std::string const& id) {
    auto it = components.find(id);
    if (it == components.end()) return nullptr;
    return it->second;
}

std::vector<Component*> GameObject::getAllComponents() {
    std::vector<Component*> out;

    for (auto& comp : components)
        out.push_back(comp.second);

    return out;
}

std::vector<Component*> GameObject::getComponents(std::string const& id) {
    std::vector<Component*> out;

    for (auto& comp : components) {
        if (comp.first == id) out.push_back(comp.second);
    }

    return out;
}

void GameObject::pushEvent(std::string const& id, void* info, const bool global) {
    if (global) scene->pushEvent(id, info);
    else handleEvent(id, info);
}

void GameObject::deleteCompVector(Component* const comp) {
    cmpOrder.erase(std::remove(cmpOrder.begin(), cmpOrder.end(), comp), cmpOrder.end());
}

void GameObject::setScene(Scene* const sc) { scene = sc; }

void GameObject::refresh() {
    auto it = components.begin();
    while (it != components.end()) {
        if (!it->second->isAlive()) {
            deleteCompVector(it->second);
            delete it->second;
            it = components.erase(it);
        }
        else
            ++it;
    }
}

void GameObject::update(const uint64_t deltaTime) {
    for (auto comp : cmpOrder) {
        if (comp->isActive()) comp->update(deltaTime);
    }
}

void GameObject::handleEvent(std::string const& id, void* info) {
    for (auto comp : cmpOrder) {
        if (comp->isActive()) comp->handleEvent(id, info);
    }
}

void GameObject::initComponents(const CompMap& variables) {
    for (auto comp : cmpOrder)
        comp->initComponent(variables);
}
void GameObject::fixedUpdate() {
    for (auto comp : cmpOrder) {
        if (comp->isActive()) comp->fixedUpdate();
    }
}

void GameObject::start() {
    for (auto comp : cmpOrder)
        comp->start();
}
}