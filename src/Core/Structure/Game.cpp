#include "Game.h"
#include "Scene.h"

namespace Tapioca {
Game::Game() {
	// No debería haber más de un objeto Game
    if(instance != nullptr) return;
	instance = this;
}

Game::~Game() {
	instance = nullptr; }

void Game::init() {
	
}

void Game::run() {
	
}

void Game::initComponents() {
	for(auto sc : scenes) sc->initComponent();
}

void Game::update() {
	for(auto sc : scenes) sc->update();
}

void Game::handleEvents() {
	for(auto sc : scenes) sc->handleEvents();
}

Game* Game::instance = nullptr;
}