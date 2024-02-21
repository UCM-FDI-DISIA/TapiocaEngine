#include "Game.h"
#include "Scene.h"

namespace Tapioca {
Game::Game(std::string const& gameName) : gameName(gameName), module(nullptr), entryPoint(nullptr) {

    // No debería haber más de un objeto Game
    if (instance != nullptr) return;
    instance = this;
}

Game::~Game() { instance = nullptr; }

bool Game::setup() {
#ifdef _DEBUG
    gamePath = "./" + gameName + "_d.dll";
#else
    gamePath = "./" + gameName + ".dll";
#endif

    std::cout << "Cargando " << gamePath << "...\n";

    if ((module = LoadLibraryA(gamePath.c_str())) == nullptr) {
        std::cerr << "Error al cargar la DLL.\n";
        return false;
    }

    if ((entryPoint = (EntryPoint)(GetProcAddress(module, "saluda"))) == nullptr) {
        std::cerr << "No se pudo obtener la dirección de la función.\n";
        FreeLibrary(module);
        return false;
    }

    entryPoint("mundo");   // Hola, mundo

    return true;
}

void Game::init() {

}

void Game::run() {

}

void Game::initComponents() {
    for (auto sc : scenes)
        sc->initComponent();
}

void Game::update() {
    for (auto sc : scenes)
        sc->update();
}

void Game::handleEvents() {
    for (auto sc : scenes)
        sc->handleEvents();
}

Game* Game::instance = nullptr;
}