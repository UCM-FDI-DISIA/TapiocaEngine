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
    components.insert(std::pair<std::string, Component*>(id, comp));
    cmpOrder.push_back(comp);
    comp->setParent(this);
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
void GameObject::fixedUpdate() {
    for (auto comp : cmpOrder)
        if (comp->isActive()) comp->fixedUpdate();
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