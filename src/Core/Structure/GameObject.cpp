#include "GameObject.h"
#include "Scene.h"
#include "Component.h"
#include "FactoryManager.h"
#include "checkML.h"
#include "Components/Transform.h"
#include "PrefabManager.h"


namespace Tapioca {
GameObject::GameObject()
    : scene(nullptr), alive(true), handler(""), zOrder(0),
      idAndVars(*(new std::vector<std::pair<std::string, CompMap>>())) { }

GameObject::GameObject(std::vector<std::pair<std::string, CompMap>>& lComponents)
    : scene(nullptr), alive(true), handler(""), zOrder(0), idAndVars(lComponents) { }

GameObject::~GameObject() {
    if (PrefabManager::instance()->isPrefab(handler)) PrefabManager::instance()->erasePrefab(handler);
    delete &idAndVars;
    for (auto& i : components)
        delete i.second;
}

void GameObject::addComponent(Component* const comp, std::string const& id, CompMap map) {
    components.insert(std::pair<std::string, Component*>(id, comp));
    comp->object = this;
    idAndVars.push_back({id, map});
}

Component* GameObject::addComponent(const std::string& id, const CompMap& variables) {
    Component* comp = FactoryManager::instance()->createComponent(id);
    if (!comp->initComponent(variables)) {
        delete comp;
        return nullptr;
    }
    addComponent(comp, id, variables);
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
            for (Component* c : vec)
                delete c;
            return vec;
        }
        addComponent(comp, id, params);
    }
    for (auto& comp : vec)
        comp->awake();
    for (auto& comp : vec)
        comp->start();
    return vec;
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

void GameObject::pushEvent(std::string const& id, void* info, const bool global, const bool delay) {
    if (global || delay) scene->pushEvent({this, id, info, global}, delay);
    else
        handleEvent(id, info);
}

void GameObject::refresh() {
    auto it = components.begin();
    while (it != components.end()) {
        if (!it->second->isAlive()) {
            delete it->second;
            it = components.erase(it);
        }
        else
            ++it;
    }
}

void GameObject::update(const uint64_t deltaTime) {
    for (auto& [name, comp] : components) {
        if (comp->isActive() && comp->isAlive()) comp->update(deltaTime);
    }
}

void GameObject::handleEvent(std::string const& id, void* info) {
    for (auto& [name, comp] : components)
        comp->handleEvent(id, info);
}

void GameObject::fixedUpdate() {
    for (auto& [name, comp] : components) {
        if (comp->isActive() && comp->isAlive()) comp->fixedUpdate();
    }
}

void GameObject::render() const {
    for (auto& [name, comp] : components) {
        if (comp->isActive() && comp->isAlive()) comp->render();
    }
}

void GameObject::awake() {
    for (auto& [name, comp] : components)
        comp->awake();
}

void GameObject::start() {
    for (auto& [name, comp] : components)
        comp->start();
}

GameObject* GameObject::InstantiatePrefab(Scene* scene, Transform* t) {
    GameObject* gameObject = new GameObject();
    gameObject->addComponents(idAndVars);
    for (auto& i : getComponent<Tapioca::Transform>()->getChildren()) {
        i->getObject()->InstantiateCopy(scene, gameObject);
    }
    scene->addObject(gameObject);

    // Aplicar el nuevo transform al objeto (las transformaciones también se aplican a sus hijos)
    gameObject->getComponent<Transform>()->setGlobalPosition(
        Tapioca::Vector3(t->getGlobalPosition().x, t->getGlobalPosition().y, t->getGlobalPosition().z));
    gameObject->getComponent<Transform>()->setScale(
        Tapioca::Vector3(t->getScale().x, t->getScale().y, t->getScale().z));
    gameObject->getComponent<Transform>()->setRotation(t->getGlobalRotation());

    return gameObject;
}

void GameObject::InstantiateCopy(Scene* scene, GameObject* parentObject) {
    GameObject* gameObject = new GameObject();
    gameObject->addComponents(idAndVars);
    for (auto& i : getComponent<Tapioca::Transform>()->getChildren()) {
        i->getObject()->InstantiateCopy(scene, gameObject);
    }
    // Relaciona al hijo con el padre
    gameObject->getComponent<Transform>()->setParent(parentObject->getComponent<Transform>());
}
}
