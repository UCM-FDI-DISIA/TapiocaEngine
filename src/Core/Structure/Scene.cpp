#include "Scene.h"
#include "GameObject.h"
#include "Game.h"

namespace Tapioca {
std::vector<GameObject*> Scene::dontDestroyOnLoad = std::vector<GameObject*>();

Scene::Scene(std::string const& name):name(name) { }

Scene::~Scene() {
    for (auto obj : objects)
        delete obj;
    
    for (auto dobj : dontDestroyOnLoad)
        delete dobj;
    dontDestroyOnLoad.clear();

}

bool Scene::addObject(GameObject* const object, std::string const& handler) {
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
    std::string ddol = "g_";
    //comprueba si es un objeto que no se borra al cambiar de escena
    if (handler.size() > ddol.size() && handler.substr(0, ddol.size()) == ddol) {
#ifdef _DEBUG
        std::cout << "ddol " << handler << "aniadido\n";
#endif   // _DEBUG

        dontDestroyOnLoad.push_back(object);
        object->setScene(this);
    }
    else {
        objects.push_back(object);
        object->setScene(this);    
    }
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

    dontDestroyOnLoad.erase(std::remove_if(dontDestroyOnLoad.begin(), dontDestroyOnLoad.end(),
                                 [this](GameObject* dobj) {
                                     if (dobj->isAlive()) return false;
                                     else {
                                         // eliminar el objeto del handler
                                         if (handlers.find(dobj->handler) != handlers.end())
                                             handlers.erase(dobj->handler);

                                         delete dobj;
                                         dobj = nullptr;
                                         return true;
                                     }
                                 }),
                            dontDestroyOnLoad.end());

    for (auto& obj : objects)
        obj->refresh();

    for (auto& dobj : dontDestroyOnLoad)
        dobj->refresh();
}

void Scene::handleEvent(std::string const& id, void* info) {
    for (auto obj : objects) {
        if (obj->isAlive()) obj->handleEvent(id, info);
    }

    for (auto dobj : dontDestroyOnLoad) {
        if (dobj->isAlive()) dobj->handleEvent(id, info);
    }
}

void Scene::pushEvent(std::string const& id, void* info) { Game::instance()->pushEvent(id, info); }


std::vector<GameObject*> Scene::getObjects() const { return objects; }

std::vector<GameObject*> Scene::getDontDestroyOnLoad() const { return dontDestroyOnLoad; }

GameObject* Scene::getHandler(std::string const& handler) const {
    auto it = handlers.find(handler);
    if (it != handlers.end()) return it->second;
    else return nullptr;
}

void Scene::update(const uint64_t deltaTime) {
    for (auto obj : objects) {
        if (obj->isAlive()) obj->update(deltaTime);
    }

    for (auto dobj : dontDestroyOnLoad) {
        if (dobj->isAlive()) dobj->update(deltaTime);
    }
}

void Scene::fixedUpdate() {
    for (auto obj : objects) {
        if (obj->isAlive()) obj->fixedUpdate();
    }

    for (auto dobj : dontDestroyOnLoad) {
        if (dobj->isAlive()) dobj->fixedUpdate();
    }
}

void Scene::render() const {
    for (auto obj : objects)
        obj->render();

    for (auto dobj : dontDestroyOnLoad)
        dobj->render();
}

void Scene::awake() {
    for (auto obj : objects)
        obj->awake();

    for (auto dobj : dontDestroyOnLoad)
        dobj->awake();
}

void Scene::start() {
    for (auto obj : objects)
        obj->start();

    for (auto dobj : dontDestroyOnLoad)
        dobj->start();
}

std::string const& Scene::getName() const { return name; }

}