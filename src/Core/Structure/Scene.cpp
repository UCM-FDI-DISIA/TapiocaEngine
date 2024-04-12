#include "Scene.h"
#include "GameObject.h"
#include "MainLoop.h"

namespace Tapioca {
Scene::Scene(std::string const& name) : name(name), active(true) { }

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

    objects.push_back(object);
    layers[zIndex].push_back(object);
    object->setScene(this);
    return true;
}

void Scene::refresh() {
    objects.erase(std::remove_if(objects.begin(), objects.end(),
                                 [this](GameObject* obj) {
                                     if (obj->isAlive()) return false;
                                     else {
                                         // eliminar el objeto del handler
                                         if (handlers.find(obj->handler) != handlers.end())
                                             handlers.erase(obj->handler);

                                         delete obj;
                                         obj = nullptr;
                                         return true;
                                     }
                                 }),
                  objects.end());

    for (auto& obj : objects)
        obj->refresh();
}

void Scene::handleEvent(std::string const& id, void* info) {
    for (auto obj : objects) {
        if (obj->isAlive()) obj->handleEvent(id, info);
    }
}

void Scene::pushEvent(std::string const& id, void* info) { MainLoop::instance()->pushEvent(id, info); }


std::vector<GameObject*> Scene::getObjects() const { return objects; }

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
    // Mayor zIndex implica que se dibuje antes para que quede por debajo
    for (auto it = layers.rbegin(); it != layers.rend(); ++it) {
        for (auto obj : it->second)
            obj->render();
    }

    /*for (auto obj : objects)
        obj->render();*/
}

void Scene::awake() {
    for (auto obj : objects)
        obj->awake();
}

void Scene::start() {
    for (auto obj : objects)
        obj->start();
}

std::string const& Scene::getName() const { return name; }

void Scene::setActive(const bool a) { active = a; }

bool Scene::isActive() const { return active; }

void Scene::updateZIndex(GameObject* obj, int zIndex) {
    if (zIndex < 0) {
        logWarn("Scene: No se puede anadir un objeto con zIndex negativo.");
        return;
    }
    else if (zIndex == 0)
        return;

    // Elimina el objeto de la capa actual
    for (auto it = layers.begin(); it != layers.end(); ++it) {
        for (GameObject* o : it->second) {
            if (o == obj) {
				it->second.erase(std::remove(it->second.begin(), it->second.end(), obj), it->second.end());
				break;
			}
		}
    }
    // Lo aniade a la nueva capa
    layers[zIndex].push_back(obj);
}
}