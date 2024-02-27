#include "Game.h"
#include "Scene.h"
#include "Module.h"
#include <iostream>
#include <chrono>

namespace Tapioca {
Game::Game()
    : finish(false)
    , deltaTime(0) {

    // No deber�a haber m�s de un objeto Game
    if (instance != nullptr) {
        std::cerr << "Se ha intentado crear un segundo objeto Game.\n";
        return;
    }
    instance = this;
}

Game::~Game() {
    instance = nullptr;

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
    for (auto mod : modules)
        mod->update(deltaTime);
}

void Game::handleEvents() {
    for (auto mod : modules)
        mod->handleEvents();
}

void Game::fixedUpdate() {
    for (auto mod : modules)
        mod->fixedUpdate();
}

void Game::render() {
    for (auto mod : modules)
        mod->render();
}

void Game::refresh() {
	for (auto mod : modules)
		mod->refresh();
}

void Game::addModule(Module* m) { modules.push_back(m); }

Game* Game::instance = nullptr;
}