#include "GameObject.h"
#include "Scene.h"
#include "Component.h"

namespace Tapioca {
GameObject::GameObject() : scene(nullptr), alive(true) {
    
}

GameObject::~GameObject() {
    for (auto& i : components)
        delete i.second;
}

void GameObject::addComponent(Component* comp, std::string id) {
    components.insert(std::pair<std::string, Component*>(id, comp));
    cmpOrder.push_back(comp);
    comp->setParent(this);
}

Component* GameObject::getComponent(std::string id) {
    auto it = components.find(id);
    if(it == components.end()) {
        return nullptr;
    }
    return it->second;
}

std::vector<Component*> GameObject::getComponents(std::string id) {
    std::vector<Component*> out;

    for(auto& comp : components) {
        if(comp.first == id) out.push_back(comp.second);
    }

    return out;
}

void GameObject::deleteComponent(std::string id) {
    auto it = components.find(id);
    if(it != components.end()) {
        it->second->alive = false;
    }
}

void GameObject::deleteCompVector(Component* comp) {
    for (auto it = cmpOrder.cbegin(); it != cmpOrder.cend(); ++it) {
        if (*it == comp) {
            cmpOrder.erase(it);
            break;
        }
    }
}

void GameObject::setScene(Scene* sc) { scene = sc; }

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

void GameObject::update(const uint64_t deltaTime) {
    for (auto comp : cmpOrder)
        if (comp->isActive()) comp->update(deltaTime);
}

void GameObject::handleEvents() {
    for (auto comp : cmpOrder)
        if (comp->isActive()) comp->handleEvents();
}

void GameObject::initComponents(const CompMap& variables) {
    for (auto comp : cmpOrder)
        comp->initComponent(variables);
}
void GameObject::fixedUpdate() {
    for (auto comp : cmpOrder)
        if (comp->isActive()) comp->fixedUpdate();
}

void GameObject::start() {
    for(auto comp : cmpOrder)
        comp->start();
}

void GameObject::onCollisionEnter(GameObject* other) {
    for (auto comp : cmpOrder)
        if (comp->isActive()) comp->onCollisionEnter(other);
}

void GameObject::onCollisionExit(GameObject* other) {
    for (auto comp : cmpOrder)
        if (comp->isActive()) comp->onCollisionExit(other);
}

void GameObject::onCollisionStay(GameObject* other) {
    for (auto comp : cmpOrder)
        if (comp->isActive()) comp->onCollisionStay(other);
}
}