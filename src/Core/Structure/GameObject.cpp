#include "GameObject.h"
#include "Scene.h"
#include "Component.h"
#include "FactoryManager.h"
#include "checkML.h"

namespace Tapioca {
GameObject::GameObject() : scene(nullptr), alive(true), handler(""), zOrder(0) { }

GameObject::~GameObject() {
    for (auto& i : components) delete i.second;
}

void GameObject::addComponent(Component* const comp, std::string const& id) {
    components.insert(std::pair<std::string, Component*>(id, comp));
    cmpOrder.push_back(comp);
    comp->object = this;
}

Component* GameObject::addComponent(const std::string& id, const CompMap& variables) {
    Component* comp = FactoryManager::instance()->createComponent(id);
    if (!comp->initComponent(variables)) {
        delete comp;
        return nullptr;
    }
    addComponent(comp, id);
    comp->awake();
    comp->start();
    return nullptr;
}

std::vector<Component*> GameObject::addComponents(const std::vector<std::pair<std::string, CompMap>>& idAndVars) {
    std::vector<Component*> vec;
    for (auto& [id, params] : idAndVars) {
        Component* comp = FactoryManager::instance()->createComponent(id);
        vec.push_back(comp);
        if (!comp->initComponent(params)) {
            // Si un componente no se puede inicializar, se cancela la operacion completa.
            for (Component* c : vec) delete c;
            return vec;
        }
        addComponent(comp, id);
    }
    for (auto& comp : vec) comp->awake();
    for (auto& comp : vec) comp->start();
    return vec;
}

Component* GameObject::getComponent(std::string const& id) {
    auto it = components.find(id);
    if (it == components.end()) return nullptr;
    return it->second;
}

std::vector<Component*> GameObject::getAllComponents() {
    std::vector<Component*> out;
    for (auto& comp : components) out.push_back(comp.second);
    return out;
}

std::vector<Component*> GameObject::getComponents(std::string const& id) {
    std::vector<Component*> out;

    for (auto& comp : components) {
        if (comp.first == id) out.push_back(comp.second);
    }

    return out;
}

void GameObject::pushEvent(std::string const& id, void* info, const bool global, const bool delay) {
    if (global || delay) scene->pushEvent({this, id, info, global}, delay);
    else handleEvent(id, info);
}

void GameObject::deleteCompVector(Component* const comp) {
    cmpOrder.erase(std::remove(cmpOrder.begin(), cmpOrder.end(), comp), cmpOrder.end());
}

void GameObject::refresh() {
    auto it = components.begin();
    while (it != components.end()) {
        if (!it->second->isAlive()) {
            deleteCompVector(it->second);
            delete it->second;
            it = components.erase(it);
        }
        else ++it;
    }
}

void GameObject::update(const uint64_t deltaTime) {
    for (auto comp : cmpOrder) {
        if (comp->isActive() && comp->isAlive()) comp->update(deltaTime);
    }
}

void GameObject::handleEvent(std::string const& id, void* info) {
    for (auto comp : cmpOrder) comp->handleEvent(id, info);
}

void GameObject::fixedUpdate() {
    for (auto comp : cmpOrder) {
        if (comp->isActive() && comp->isAlive()) comp->fixedUpdate();
    }
}

void GameObject::render() const {
    for (auto comp : cmpOrder) {
        if (comp->isActive() && comp->isAlive()) comp->render();
    }
}

void GameObject::awake() { for (auto comp : cmpOrder) comp->awake(); }

void GameObject::start() { for (auto comp : cmpOrder) comp->start(); }
}