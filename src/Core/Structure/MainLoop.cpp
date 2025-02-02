#include "MainLoop.h"
#include <filesystem>
#include <chrono>
#include "DynamicLibraryLoader.h"
#include "Scene.h"
#include "Module.h"
#include "checkML.h"
#include "GameObject.h"
#include "checkML.h"

namespace Tapioca {
MainLoop::MainLoop() : deltaTime(0), finish(false), assetsPath("assets") {
    if (!std::filesystem::exists(assetsPath)) {
        logInfo(("MainLoop: La carpeta \"" + assetsPath + "\" no existe.").c_str());

        try {
            if (std::filesystem::create_directory(assetsPath)) {
                logInfo("MainLoop: Carpeta de assets creada correctamente.");
            }
        } catch (const std::filesystem::filesystem_error& e) {
            logError(("MainLoop: No se pudo crear la carpeta de assets. " + std::string(e.what())).c_str());
        }
    }

    loader = new DynamicLibraryLoader();
}

MainLoop::~MainLoop() {
    for (auto sc : sceneBuffer) delete sc;
    sceneBuffer.clear();

    for (auto& s : loadedScenes) delete s.second;
    loadedScenes.clear();

    for (Module* mod : modules) delete mod;

    if (loader != nullptr) delete loader;
    loader = nullptr;
}

bool MainLoop::init() {
    bool initialized = true;
    auto mod = modules.begin();

    while (initialized && mod != modules.end()) {
        initialized = (*mod)->init();
        ++mod;
    }
    return initialized;
}

bool MainLoop::initConfig() {
    bool initialized = true;
    auto mod = modules.begin();
    while (initialized && mod != modules.end()) {
        initialized = (*mod)->initConfig();
        ++mod;
    }
    return initialized;
}

bool MainLoop::loadGame(std::string const& gameName) { return loader->initGame(gameName); }

void MainLoop::start() {
    for (auto mod : modules) mod->start();

    if (loadedScenes.size() == 0 && sceneBuffer.size() == 0) {
        logWarn("MainLoop: No hay escena de inicio del motor. Se va a cerrar la aplicacion.");
        return;
    }
}

void MainLoop::run() {
    start();

    finish = false;
    deltaTime = 0;
    auto currentTime = std::chrono::high_resolution_clock::now();
    uint64_t lag = 0;
    uint64_t timeSinceStart = 0;

    while (!finish) {
        auto newTime = std::chrono::high_resolution_clock::now();
        deltaTime = (std::chrono::duration_cast<std::chrono::milliseconds>(newTime - currentTime)).count();
        currentTime = newTime;

        lag += deltaTime;

        uint64_t numFixedUpdates = 0;
        // Se realiza el update cada cierto tiempo determinado
        // Se va acumulando el tiempo sobrante (lag) --> varios updates en el mismo frame
        while (lag >= FIXED_DELTA_TIME) {
            fixedUpdate();
            lag -= FIXED_DELTA_TIME;

            // evitar el problema de Spiral of Death
            ++numFixedUpdates;
            if (numFixedUpdates > MAX_NUM_FIXED_UDPATES) {
                lag = 0;
                break;
            }
        }

        handleDelayedEvents();
        update();
        refresh();
        render();
    }
}

void MainLoop::update() {
    for (auto mod : modules) mod->update(deltaTime);

    for (auto& s : loadedScenes) {
        if (s.second->isActive()) s.second->update(deltaTime);
    }
}

void MainLoop::fixedUpdate() {
    for (auto mod : modules) mod->fixedUpdate();

    for (auto& s : loadedScenes) {
        if (s.second->isActive()) s.second->fixedUpdate();
    }
}

void MainLoop::render() { 
    for (auto mod : modules) mod->render(); 
}

void MainLoop::refresh() {
    for (auto mod : modules) mod->refresh();

    for (Scene* sc : toDelete) {
        loadedScenes.erase(sc->getName());
        delete sc;
    }
    toDelete.clear();

    if (!sceneBuffer.empty()) {
        std::unordered_set<Scene*> auxBuffer = sceneBuffer;
        sceneBuffer.clear();

        for (auto sc : auxBuffer) {
            auto loadedSc = loadedScenes.insert({sc->getName(), sc});
            if (!loadedSc.second) {
                delete sc;
                logError("[MainLoop]: No se ha cargado la escena porque ya existe");
            }
            else {
                sc->awake();
                sc->start();
            }
        }
    }

    if (loadedScenes.size() == 0 && sceneBuffer.size() == 0) {
        exit();
        logWarn("MainLoop: No hay escenas en el juego. Se va a cerrar la aplicacion.");
    }

    for (auto& s : loadedScenes) {
        if (s.second->isActive()) s.second->refresh();
    }
}

void MainLoop::handleDelayedEvents() {
    std::vector<Event> aux_events;
    std::swap(delayedEvents, aux_events);
    for (auto& e : aux_events) {
        if (!e.global && e.emisor != nullptr) {
            e.emisor->handleEvent(e.id, e.info);
        }
        else {
            for (auto& s : loadedScenes) {
                if (s.second->isActive()) s.second->handleEvent(e.id, e.info);
            }
        }
    }
    aux_events.clear();
}

void MainLoop::addModule(Module* const m) { modules.push_back(m); }

void MainLoop::pushEvent(Event const& e, bool const delay) {
    if (delay) delayedEvents.emplace_back(e);
    else {
        for (auto& s : loadedScenes) {
            if (s.second->isActive()) s.second->handleEvent(e.id, e.info);
        }
    }
}

Scene* MainLoop::getScene(std::string sc) {
    auto aux = loadedScenes.find(sc);
    if (aux != loadedScenes.end()) return aux->second;
    else return nullptr;
}

void MainLoop::deleteScene(Scene* const sc) { deleteScene(sc->getName()); }

void MainLoop::deleteScene(std::string const& sc) {
    auto it = loadedScenes.find(sc);
    if (it != loadedScenes.end()) toDelete.insert(it->second);
}

void MainLoop::loadScene(Scene* const sc) { sceneBuffer.insert(sc); }
}
