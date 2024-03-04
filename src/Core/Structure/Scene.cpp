#include "Scene.h"
#include "GameObject.h"
#include "Game.h"

// TODO: solo para prueba
#include "Components/Transform.h"

namespace Tapioca {
Scene::Scene() { }

Scene::~Scene() {
    for (auto obj : objects) delete obj;
}

void Scene::addObject(GameObject* object, std::string handler) {
    objects.push_back(object);
    if (handler != "") {
        object->setName(handler);
        handlers[handler] = object;
    }
    object->setScene(this);
}

void Scene::refresh() {
    // sacar el objeto de los handlers
    /*for (auto it = handlers.begin(); it != handlers.end();) {
        if (!it->second->isAlive()) {
            it = handlers.erase(it);
        }
        else {
            ++it;
        }
    }*/

    objects.erase(std::remove_if(objects.begin(), objects.end(),
        [this](GameObject* obj) {
            if (obj->isAlive()) return false;
            else {
                // eliminar el objeto del handler
                if (handlers.find(obj->getName()) != handlers.end()) handlers.erase(obj->getName());

                delete obj;
                // hacer que el hueco de memoria
                // apunte a nullptr siempre va despues de eliminar el objeto
                obj = nullptr;
                return true;
            }
        }),
        objects.end());

    for (auto& obj : objects) obj->refresh();
}

std::vector<GameObject*> Scene::getObjects() const { return objects; }

GameObject* Scene::getHandler(const std::string& handler) const {
    auto it = handlers.find(handler);
    if (it != handlers.end()) return it->second;
    else return nullptr;
}

void Scene::update(const uint64_t deltaTime) {
    for (auto obj : objects) {
        if (obj->isAlive()) obj->update(deltaTime);
    }
}

void Scene::handleEvents() {
    for (auto obj : objects) {
        if (obj->isAlive()) obj->handleEvents();
    }
}

void Scene::fixedUpdate() {
    for (auto obj : objects) {
        if (obj->isAlive()) obj->fixedUpdate();
    }
}
void Scene::start() {
    // TODO: solo para pruebas
    /*auto holaObject = getHandler("Hola");
    auto holaTransform = holaObject->getComponent<Transform>();
    auto entity1Object = getHandler("Entity1");
    auto entity1Transform = entity1Object->getComponent<Transform>();
    auto entity2Object = getHandler("Entity2");
    auto entity2Transform = entity2Object->getComponent<Transform>();

    entity1Transform->setParentHierarchy(holaTransform);
    entity2Transform->setParentHierarchy(entity1Transform);*/

    for (auto obj : objects) obj->start();
}


}
