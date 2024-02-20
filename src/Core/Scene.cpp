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
}

void Scene::refresh() {
    // sacar el objeto de los handlers
    handlers.erase(std::remove_if(handlers.begin(), handlers.end(),
                       [](GameObject* obj) {
                           if (obj->isAlive()) return false;
                           else
                               return true;
                       }),
        handlers.end());


    objects.erase(std::remove_if(objects.begin(), objects.end(),
                      [](GameObject* obj) {
                          if (obj->isAlive()) return false;

                          else {
                              delete obj;
                              // hacer que el hueco de memoria
                              // apunte a nullptr siempre va después de eliminar el objeto
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
    if (it != handlers.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

void Scene::initComponent() {
    for (auto obj : objects)
        obj->initComponent();
}

void Scene::update() {
    for (auto obj : objects)
        obj->update();
}

void Scene::handleEvents() {
    for (auto obj : objects)
        obj->handleEvents();
}
}