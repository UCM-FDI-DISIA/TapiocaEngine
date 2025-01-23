#include "Scene.h"
#include "GameObject.h"
#include "MainLoop.h"
#include "checkML.h"
#include "Structure/PrefabManager.h"
#include "Components/Transform.h"

namespace Tapioca {
Scene::Scene(std::string const& name)
    : active(true), name(name) { }

Scene::~Scene() {
    for (auto obj : objects)
        delete obj;
}

bool Scene::addObject(GameObject* const object, std::string const& handler, int zIndex) {
    if (handler != "") {
        if (handlers.contains(handler)) {
            logError(("Scene: Ya existe ese nombre de handler (\"" + handler + "\"), por favor elige otro.").c_str());
            return false;
        }
        object->handler = handler;
        handlers[handler] = object;
    }

    objects.insert(object);
    layers[zIndex].insert(object);
    object->setScene(this);
    object->setZOrder(zIndex);
    return true;
}

void Scene::refresh() {
    std::unordered_set<GameObject*> objectsAux;
    for (GameObject* object : objects) {
        if (!object->isAlive()) objectsAux.insert(object);
    }
    for (GameObject* objectAux : objectsAux) {
        objects.erase(objectAux);
        if (handlers.find(objectAux->handler) != handlers.end()) handlers.erase(objectAux->handler);
        auto itLayer = layers.find(objectAux->getZOrder());
        if (itLayer != layers.end()) {
            if (itLayer->second.contains(objectAux)) itLayer->second.erase(objectAux);
        }
        delete objectAux;
        objectAux = nullptr;
    }

    for (auto& obj : objects)
        obj->refresh();
}

void Scene::handleEvent(std::string const& id, void* info) {
    for (auto obj : objects) {
        if (obj->isAlive()) obj->handleEvent(id, info);
    }
}

void Scene::pushEvent(Event const& e, bool const delay) { MainLoop::instance()->pushEvent(e, delay); }

GameObject* Scene::getHandler(std::string const& handler) const {
    auto it = handlers.find(handler);
    if (it != handlers.end()) return it->second;
    else
        return nullptr;
}

void Scene::update(const uint64_t deltaTime) {
    for (auto obj : objects) {
        if (obj->isAlive()) obj->update(deltaTime);
    }
}

void Scene::fixedUpdate() {
    for (auto obj : objects) {
        if (obj->isAlive()) obj->fixedUpdate();
    }
}

void Scene::render() const {
    if (!active) return;
    // Mayor zIndex implica que se dibuje antes para que quede por debajo
    for (auto it = layers.rbegin(); it != layers.rend(); ++it) {
        for (auto obj : it->second) {
            if (obj->isAlive()) obj->render();
        }
    }
}

void Scene::awake() {
    for (auto obj : objects)
        obj->awake();
}

void Scene::start() {
    for (auto obj : objects)
        obj->start();

    for (auto instance : lInstantiate) {
        if (instance.second->getAllComponents().size() > 1)
            PrefabManager::instance()->instantiate(instance.first, this, instance.second->getComponent<Transform>(), instance.second->idAndVars);
        else
            PrefabManager::instance()->instantiate(instance.first, this, instance.second->getComponent<Transform>());
        instance.second->die();
    }
    lInstantiate.clear();
}

void Scene::updateZIndex(GameObject* obj, int zIndex) {
    if (zIndex < 0) {
        logWarn("Scene: No se puede anadir un objeto con zIndex negativo.");
        return;
    }
    else if (zIndex == 0)
        return;

    // Elimina el objeto de la capa actual
    auto itLayer = layers.find(obj->getZOrder());
    if (itLayer != layers.end()) {
        if (itLayer->second.contains(obj)) {
            itLayer->second.erase(obj);
        }
    }

    // Lo anade a la nueva capa
    layers[zIndex].insert(obj);
}

void Scene::addInstance(std::string name, GameObject* gameObject) {
    if (PrefabManager::instance()->isPrefab(name)) lInstantiate.push_back({name, gameObject});
}
}
