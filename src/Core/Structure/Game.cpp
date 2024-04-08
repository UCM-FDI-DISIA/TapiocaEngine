#include "Game.h"
#include <chrono>
#include "Scene.h"
#include "Module.h"
#include "DynamicLibraryLoader.h"
#include "checkML.h"

namespace Tapioca {
template class TAPIOCA_API Singleton<Game>;
template<>
Game* Singleton<Game>::instance_ = nullptr;

Game::Game() : finish(false), deltaTime(0), gameInitialized(false) { }

Game::~Game() {
    for (auto s : loadedScenes)
        delete s.second;

    loadedScenes.clear();

    DynamicLibraryLoader::freeModule();

    for (Module* mod : modules)
        delete mod;
}

bool Game::init() {
    bool initialized = true;
    auto mod = modules.begin();

    while (initialized && mod != modules.end()) {
        initialized = (*mod)->init();
        ++mod;
    }
    return initialized;
}

bool Game::initConfig() {
    bool initialized = true;
    auto mod = modules.begin();
    while (initialized && mod != modules.end()) {
        initialized = (*mod)->initConfig();
        ++mod;
    }
    return initialized;
}

void Game::run() {
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

        update();
        refresh();

        if (!gameInitialized) timeSinceStart += deltaTime;
        if (timeSinceStart >= TIME_TO_INITIALIZE_GAME && !gameInitialized) {
#ifdef _DEBUG
            std::cout << "Ya pasaron " << TIME_TO_INITIALIZE_GAME << " milisegundos\n";
#endif
            if (!DynamicLibraryLoader::initGame())
            	finish = true;
            deleteScene(mainScene);
            gameInitialized = true;
        }

        render();
    }
}

void Game::start() {
    for (auto mod : modules)
        mod->start();
}

void Game::update() {
    for (auto mod : modules)
        mod->update(deltaTime);

    for (auto s : loadedScenes)
        if (s.second->isActive()) s.second->update(deltaTime);
}

void Game::fixedUpdate() {
    for (auto mod : modules)
        mod->fixedUpdate();

    for (auto s : loadedScenes)
        if (s.second->isActive()) s.second->fixedUpdate();
}

void Game::render() {
    for (auto mod : modules)
        mod->render();
}

void Game::refresh() {
    for (auto mod : modules)
        mod->refresh();

    for (Scene* sc : toDelete)
        delete sc;
    toDelete.clear();

    for (auto s : loadedScenes)
        if (s.second->isActive()) s.second->refresh();
}

void Game::addModule(Module* const m) { modules.push_back(m); }

void Game::pushEvent(std::string const& id, void* info) {
#ifdef _DEBUG
    //if (id == "ev_ACCEPT") std::cout << "Aceptar\n";
#endif

    for (auto s : loadedScenes)
        if (s.second->isActive()) s.second->handleEvent(id, info);
}

std::unordered_map<std::string, Scene*> Game::getLoadedScenes() const { return loadedScenes; }

Scene* Game::getScene(std::string sc) { return loadedScenes.at(sc); }

void Game::deleteScene(Scene* const sc) { deleteScene(sc->getName()); }

void Game::deleteScene(std::string const& sc) {
    auto it = loadedScenes.find(sc);
    if (it != loadedScenes.end()) {
        toDelete.push_back(it->second);
        loadedScenes.erase(it);
    }

    if (loadedScenes.size() == 0) {
        finish = true;
#ifdef _DEBUG
        std::cout << "No hay escenas en el juego. Se va a cerrar la aplicacion.\n";
#endif
    }
}

void Game::loadScene(Scene* const sc) {
    loadedScenes.insert({sc->getName(), sc});
    // TODO: mejorar awake y start para que se ejecute para componentes que se crean en tiempo de ejecucion
    sc->awake();
    sc->start();
}
}
