#include "Scene.h"
#include "GameObject.h"
#include "Game.h"

// TODO: solo para prueba
#include "Components/Transform.h"

namespace Tapioca {
Scene::Scene() { }

Scene::~Scene() {
    for (auto obj : objects)
        delete obj;
}

bool Scene::addObject(GameObject* object, std::string const& handler) {
    if (handler != "") {
        if (handlers.contains(handler)) {
#ifdef _DEBUG
            std::cerr << "Ya existe ese nombre de handler, por favor elige otro.\n";
#endif
            return false;
        }
        object->handler = handler;
        handlers[handler] = object;
    }
    objects.push_back(object);
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

void Scene::pushEvent(std::string const& id, void* info) {
    for (auto obj : objects) {
        if (obj->isAlive()) obj->handleEvent(id, info);
    }
}

std::vector<GameObject*> Scene::getObjects() const { return objects; }

GameObject* Scene::getHandler(const std::string& handler) const {
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
void Scene::start() {
    for (auto obj : objects)
        obj->start();
}


}
