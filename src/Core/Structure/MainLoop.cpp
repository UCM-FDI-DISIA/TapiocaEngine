#include "MainLoop.h"
#include <chrono>
#include "Scene.h"
#include "Module.h"
#include "DynamicLibraryLoader.h"
#include "checkML.h"
#include "GameObject.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<MainLoop>;
template<>
MainLoop* Singleton<MainLoop>::instance_ = nullptr;

MainLoop::MainLoop() : finish(false), deltaTime(0), gameInitialized(false) { }

MainLoop::~MainLoop() {
    for (auto sc : sceneBuffer)
        delete sc;

    sceneBuffer.clear();

    for (auto s : loadedScenes)
        delete s.second;

    loadedScenes.clear();

    DynamicLibraryLoader::freeModule();

    for (Module* mod : modules)
        delete mod;
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

void MainLoop::run() {
    if (loadedScenes.size() == 0 && sceneBuffer.size() == 0) {
        logWarn("MainLoop: No hay escena de inicio del motor. Se va a cerrar la aplicacion.");
        return;
    }

    // Se vuelven a inicializar por si acaso
    finish = false;
    deltaTime = 0;
    gameInitialized = false;
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

        // TEMPORAL
        loadingGame(deltaTime);

        handleDelayedEvents();
        update();
        refresh();
        render();
    }
}

void MainLoop::start() {
    for (auto mod : modules)
        mod->start();
}

void MainLoop::update() {
    // Asegura que todas las escenas se carguen en el mismo
    // ciclo, aunque se creen nuevas escenas al cargar otras
    while (!sceneBuffer.empty()) {
        auto auxBuffer = sceneBuffer;
        sceneBuffer.clear();

        for (auto sc : auxBuffer) {
            auto loadedSc = loadedScenes.insert({sc->getName(), sc});
            if (!loadedSc.second) Tapioca::logError("[MainLoop]: No se ha cargado la escena porque ya existe");
            else {
                sc->awake();
                sc->start();
            }
        }
    }

    for (auto mod : modules)
        mod->update(deltaTime);

    for (auto s : loadedScenes)
        if (s.second->isActive()) s.second->update(deltaTime);
}

void MainLoop::fixedUpdate() {
    for (auto mod : modules)
        mod->fixedUpdate();

    for (auto s : loadedScenes)
        if (s.second->isActive()) s.second->fixedUpdate();
}

void MainLoop::render() {
    for (auto mod : modules)
        mod->render();
}

void MainLoop::refresh() {
    for (auto mod : modules)
        mod->refresh();

    for (Scene* sc : toDelete)
        delete sc;
    toDelete.clear();

    for (auto s : loadedScenes)
        if (s.second->isActive()) s.second->refresh();
}

void MainLoop::handleDelayedEvents() {
    std::vector<Event> aux_events;
    std::swap(delayedEvents, aux_events);
    for (auto& e : aux_events) {
        if (!e.global && e.emisor!=nullptr) {
            e.emisor->handleEvent(e.id, e.info);
        }
        else {
            for (auto s : loadedScenes) {
                if (s.second->isActive()) s.second->handleEvent(e.id, e.info);
            }
        }
    }
    aux_events.clear();
}

void MainLoop::addModule(Module* const m) { modules.push_back(m); }

void MainLoop::pushEvent(Event const& e, bool const delay) {
    if (delay) delayedEvents.emplace_back(e);
    else
        for (auto s : loadedScenes)
            if (s.second->isActive()) s.second->handleEvent(e.id, e.info);
}

std::unordered_map<std::string, Scene*> MainLoop::getLoadedScenes() const { return loadedScenes; }

Scene* MainLoop::getScene(std::string sc) {
    auto aux = loadedScenes.find(sc);
    if (aux != loadedScenes.end()) return aux->second;
    else
        return nullptr;
}

void MainLoop::deleteScene(Scene* const sc) { deleteScene(sc->getName()); }

void MainLoop::deleteScene(std::string const& sc) {
    auto it = loadedScenes.find(sc);
    if (it != loadedScenes.end()) {
        toDelete.push_back(it->second);
        loadedScenes.erase(it);
    }

    if (loadedScenes.size() == 0 && sceneBuffer.size() == 0) {
        finish = true;
        logWarn("MainLoop: No hay escenas en el juego. Se va a cerrar la aplicacion.");
    }
}

void MainLoop::loadScene(Scene* const sc) { sceneBuffer.push_back(sc); }

void MainLoop::loadingGame(uint64_t deltaTime) {
    static uint64_t timeSinceStart = 0;

    if (!gameInitialized) timeSinceStart += deltaTime;
    if (timeSinceStart >= TIME_TO_INITIALIZE_GAME && !gameInitialized) {
        logInfo(("Ya pasaron " + std::to_string(TIME_TO_INITIALIZE_GAME) + " milisegundos").c_str());
        if (!DynamicLibraryLoader::initGame()) finish = true;
        deleteScene(MAIN_SCENE_NAME);
        gameInitialized = true;
    }
}
}
