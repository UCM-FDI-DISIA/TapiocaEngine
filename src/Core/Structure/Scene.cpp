#include "Scene.h"
#include "GameObject.h"
#include "Game.h"

namespace Tapioca {
Scene::Scene() { }

Scene::~Scene() {
    for (auto obj : objects)
        delete obj;
}

void Scene::addObject(GameObject* object, std::string handler) {
    objects.push_back(object);
    if (handler != "") handlers[handler] = object;
    object->setScene(this);
}

void Scene::refresh() {
    // Sacar el objeto de los handlers
    for (auto it = handlers.begin(); it != handlers.end();) {
        if (!it->second->isAlive()) it = handlers.erase(it);
        else
            ++it;
    }

    objects.erase(std::remove_if(objects.begin(), objects.end(),
                      [](GameObject* obj) {
                          if (obj->isAlive()) return false;
                          else {
                              delete obj;
                              // hacer que el hueco de memoria
                              // apunte a nullptr siempre va despues de eliminar el objeto
                              obj = nullptr;
                              return true;
                          }
                      }),
        objects.end());

    for (auto& obj : objects)
        obj->refresh();
}

GameObject* Scene::getHandler(const std::string& handler) const {
    auto it = handlers.find(handler);
    if (it != handlers.end()) return it->second;
    else
        return nullptr;
}

void Scene::update(const uint64_t deltaTime) {
    for (auto obj : objects)
        obj->update(deltaTime);
}

void Scene::handleEvents() {
    for (auto obj : objects)
        obj->handleEvents();
}

void Scene::fixedUpdate() {
    for (auto obj : objects)
        obj->fixedUpdate();
}
void Scene::start() {
    for (auto obj : objects)
        obj->start();
}
}
