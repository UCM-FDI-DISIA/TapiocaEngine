#include "Game.h"
#include "Scene.h"
#include "Module.h"
#include <chrono>

#ifdef _DEBUG
#include <iostream>
#endif

namespace Tapioca {

Game::Game() : finish(false), deltaTime(0) {

    // No deberia haber mas de un objeto Game
    if (instance != nullptr) {
#ifdef _DEBUG
        std::cerr << "Se ha intentado crear un segundo objeto Game.\n";
#endif
        return;
    }
    instance = this;
}

Game::~Game() {
    instance = nullptr;

    while (!scenes.empty()) {
        delete scenes.top();
        scenes.pop();
    }

    for (Module* mod : modules) delete mod;
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

void Game::run() {
    // se vuelven a inicializar por si acaso
    finish = false;
    deltaTime = 0;

    auto currentTime = std::chrono::high_resolution_clock::now();
    uint64_t lag = 0;

    while (!finish) {
        auto newTime = std::chrono::high_resolution_clock::now();
        deltaTime = (std::chrono::duration_cast<std::chrono::milliseconds>(newTime - currentTime)).count();
        currentTime = newTime;

        lag += deltaTime;

        handleEvents();

        uint64_t numFixedUpdates = 0;
        // se realiza el update cada cierto tiempo determinado
        // se va acumulando el tiempo sobrante (lag) --> varios updates en el mismo frame
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

        render();
    }
}

void Game::update() {
    for (auto mod : modules) mod->update(deltaTime);

    if (!scenes.empty()) scenes.top()->update(deltaTime);
}

void Game::handleEvents() {
    for (auto mod : modules) mod->handleEvents();

    if (!scenes.empty()) scenes.top()->handleEvents();
}

void Game::fixedUpdate() {
    for (auto mod : modules) mod->fixedUpdate();

    if (!scenes.empty()) scenes.top()->fixedUpdate();
}

void Game::render() {
    for (auto mod : modules) mod->render();
}

void Game::refresh() {
	for (auto mod : modules) mod->refresh();

    for (Scene* sc : toDelete) delete sc;
    toDelete.clear();

    if (!scenes.empty()) scenes.top()->refresh();
}

void Game::addModule(Module* m) { modules.push_back(m); }

void Game::pushScene(Scene* sc) {
    scenes.push(sc);
    // TODO: start que se ejecute para componentes que se crean en tiempo de ejecucion
    sc->start();
}

void Game::popScene() {
    toDelete.push_back(scenes.top());
    if (!scenes.empty()) scenes.pop();
    else finish = true;
}

void Game::changeScene(Scene* sc) {
    toDelete.push_back(scenes.top());
    scenes.pop();
    scenes.push(sc);
    sc->start();
}

Game* Game::instance = nullptr;
}
