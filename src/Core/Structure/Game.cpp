#include "Game.h"
#include "Scene.h"
#include "Module.h"
#include <iostream>

namespace Tapioca {
Game::Game() : cont(true), deltaTime(0), fixedOffset(0), startTime(0) {

    // No debería haber más de un objeto Game
    if (instance != nullptr) {
        std::cerr << "Se ha intentado crear un segundo objeto Game.\n";
        return;
    }
    instance = this;
}

Game::~Game() { instance = nullptr; }

void Game::init() {
    for (auto mod : modules)
        mod->init();
}

void Game::run() {
    deltaTime = startTime = 0; // TODO Conseguir tiempo
    fixedOffset = 0;
    cont = true;

    while (cont) {
        deltaTime = 0; // TODO Conseguir tiempo
        fixedOffset += deltaTime;
        
        handleEvents();
        while (fixedOffset >= FIXED_DELTA_TIME) {
            fixedUpdate();
            fixedOffset -= FIXED_DELTA_TIME;
        }
        update();
        render();
    }
}

void Game::initComponents() {
    scenes.top()->initComponent();
}

void Game::update() {
    for (auto mod : modules)
        mod->update();

    scenes.top()->update();
}

void Game::handleEvents() {
    for (auto mod : modules)
        mod->handleEvents();

    scenes.top()->handleEvents();
}

void Game::fixedUpdate() {
    for (auto mod : modules)
        mod->fixedUpdate();

    scenes.top()->fixedUpdate();
}

void Game::render() {
    for (auto mod : modules)
        mod->render();
}

void Game::addScene(Scene* sc) {
    scenes.push(sc); }

void Game::addModule(Module* m) { 
    modules.push_back(m); }

Game* Game::instance = nullptr;
}